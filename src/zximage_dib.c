/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_dib - Microsoft BMP(Device Independent Bitmap, DIB) support.
 */

#include <zx11/zximage_dib.h>

/* BMP file format checker
 */
bool zxImageFileIsBMP(char filename[])
{
  return zxImageFileIdent( filename, "BM", 2 );
}

typedef struct{
  FILE *fp;         /* image file pointer */
  uint32_t filesize;   /* file size */
  uint32_t dataoffset; /* file offset to raster data */
  uint32_t width;      /* image width */
  uint32_t height;     /* image height */
  ubyte bitcount;   /* bits per pixel
                     *  1 = monochrome palette num_color = 1
                     *  4 = 4bit palettized num_color = 16
                     *  8 = 8bit palettized num_color = 256
                     * 16 =16bit RGB        num_color = 65536(?)
                     * 24 =24bit RGB        num_color = 16M
                     */
  ubyte comp;       /* type of compression
                     * 0 = BI_RGB : no compression
                     * 1 = BI_RLE8: 8bit RLE encoding
                     * 2 = BI_RLE4: 4bit RLE encoding
                     */
  uint32_t imagesize;  /* image size */
  uint32_t xppm;       /* X pixels/meter */
  uint32_t yppm;       /* Y pixels/meter */
  uint32_t nca;        /* number of catually used colors */
  uint32_t nci;        /* number of important colors */
  uint32_t numcolors;  /* number of colors(palette size) */
  uint32_t bpl;        /* bytes per line */
  ubyte bpp;        /* original byte par pixel */
  zxPixel *palette; /* palette */
} zxBMPInfo;

/* type of compression */
enum{
  BI_RGB = 0, BI_RLE8, BI_RLE4
};

static uint _zxBMPAlignBPL(uint *bpl);

static ulong _zxBMPReadVal(FILE *fp, uint size);
static int _zxBMPReadHeader(zxBMPInfo *info);
static zxPixel *_zxBMPReadPalette(zxBMPInfo *info);
static int _zxBMPDetectMode(zxBMPInfo *info);
static void _zxBMPRead_1bit(zxBMPInfo *info, zxImage *img);
static void _zxBMPRead_4bit(zxBMPInfo *info, zxImage *img);
static void _zxBMPRead_8bit(zxBMPInfo *info, zxImage *img);
static void _zxBMPRead_TrueColor(zxBMPInfo *info, zxImage *img);
static void _zxBMPRead_RLE8(zxBMPInfo *info, zxImage *img);
static void _zxBMPRead_RLE4(zxBMPInfo *info, zxImage *img);
static void _zxBMPRead(zxBMPInfo *info, zxImage *img);

static void _zxBMPWriteVal(FILE *fp, uint size, uint32_t val);
static int _zxBMPWriteHeader(zxBMPInfo *info, zxImage *img);
static void _zxBMPWrite(zxBMPInfo *info, zxImage *img);

#define ZX_BMP_TCBIT 24

/* BPL(Byte Per Pixel) Alignment
 */
uint _zxBMPAlignBPL(uint *bpl)
{
  uint rest;

  if( ( rest = *bpl % 4 ) ) *bpl += 4 - rest;
  return *bpl;
}

/* BMP reader
 */
ulong _zxBMPReadVal(FILE *fp, uint size)
{
  register uint i;
  ulong value = 0;

  for( i=0; i<size; i++ ){
    if( feof( fp ) ){
      ZRUNERROR( "unexpected EOF" );
      return 0;
    }
    value |= (ulong)fgetc( fp ) << (ZX_BIT_PER_BYTE*i);
  }
  return value;
}

int _zxBMPReadHeader(zxBMPInfo *info)
{
#define ZX_BMP_HEADERSIZE     54
#define ZX_BMP_INFOHEADERSIZE 40
  /* header charactor */
  if ( fgetc(info->fp)!='B' || fgetc(info->fp)!='M' ){
    ZRUNERROR( "not BMP file" );
    return 0;
  }
  info->filesize = _zxBMPReadVal( info->fp, 4 ); /* file size */
  if( _zxBMPReadVal( info->fp, 4 ) != 0 ) /* reserved */
    ZRUNWARN( "unexpected reserved value" );
  info->dataoffset = _zxBMPReadVal( info->fp, 4 ); /* data offset */
  /* info header size */
  if( _zxBMPReadVal( info->fp, 4 ) != ZX_BMP_INFOHEADERSIZE ){
    ZRUNERROR( "unsupported BMP(parhaps OS/2 bitmap)" );
    return 0;
  }
  info->width  = _zxBMPReadVal( info->fp, 4 ); /* image width */
  info->height = _zxBMPReadVal( info->fp, 4 ); /* image height */
  if( _zxBMPReadVal( info->fp, 2 ) != 1 ){ /* number of plane */
    ZRUNERROR( "invalid number of planes" );
    return 0;
  }
  info->bitcount = _zxBMPReadVal( info->fp, 2 ); /* bit count */
  /* format detection */
  if( _zxBMPDetectMode( info ) == 0 ) return 0;
  info->comp = _zxBMPReadVal( info->fp, 4 ); /* type of compression */
  switch( info->comp ){
  case BI_RGB: case BI_RLE8: case BI_RLE4: break;
  default:
    ZRUNERROR( "unsupported type of compression" );
    return 0;
  }
  info->imagesize = _zxBMPReadVal( info->fp, 4 ); /* image size */
  info->xppm = _zxBMPReadVal( info->fp, 4 ); /* X pixels/meter */
  info->yppm = _zxBMPReadVal( info->fp, 4 ); /* Y pixels/meter */
  /* number of actually used colors */
  info->nca = _zxBMPReadVal( info->fp, 4 );
  /* number of important color */
  info->nci = _zxBMPReadVal( info->fp, 4 );
  return 1;
}

int _zxBMPDetectMode(zxBMPInfo *info)
{
  switch( info->bitcount ){
  case  1: /* B/W */
  case  4: /* pseudo color */
  case  8: /* pseudo color */
    info->numcolors = 1 << info->bitcount;
    info->bpp = 1;
    info->bpl = info->width / ( ZX_BIT_PER_BYTE / info->bitcount )
      + ( info->width % 2 ? 1 : 0 );
    _zxBMPAlignBPL( &info->bpl );
    break;
  case 24: /* true color */
  case 32: /* true color */
    info->numcolors = 0;
    info->bpp = info->bitcount / ZX_BIT_PER_BYTE;
    info->bpl = info->width * info->bpp;
    _zxBMPAlignBPL( &info->bpl );
    break;
  default:
    ZRUNERROR( "unsupported bit count" );
    return 0;
  }
  return 1;
}

zxPixel *_zxBMPReadPalette(zxBMPInfo *info)
{
  register uint i;
  ubyte r, g, b;
  zxPixelManip pm;

  if( info->numcolors == 0 ) return NULL;
  info->palette = zAlloc( zxPixel, info->numcolors );
  if( info->palette == NULL ){
    ZRUNERROR( "cannot allocate enough memory for color table" );
    return NULL;
  }
  zxPixelManipSet( &pm, ZX_BMP_TCBIT );
  for( i=0; i<info->numcolors; i++ ){
    b = _zxBMPReadVal( info->fp, 1 );
    g = _zxBMPReadVal( info->fp, 1 );
    r = _zxBMPReadVal( info->fp, 1 );
    _zxBMPReadVal( info->fp, 1 ); /* RGBQUAD.reserved */
    info->palette[i] = pm.PixelFromRGB( r, g, b );
  }
  return info->palette;
}

/* B/W */
void _zxBMPRead_1bit(zxBMPInfo *info, zxImage *img)
{
  register int i, j, k;
  zxPixel pixel, p;
  zxPixelManip src, dest;

  zxPixelManipSet( &src, ZX_BMP_TCBIT );
  zxPixelManipSetDefault( &dest );
  for( i=img->height-1; i>=0; i-- )
    for( j=0; j<info->bpl; j++ ){
      pixel = _zxBMPReadVal( info->fp, 1 );
      for( k=0; k<ZX_BIT_PER_BYTE; k++ ){
        p = ( pixel >> (ZX_BIT_PER_BYTE-k-1) ) & 0x1;
        zxImageCellFromPixel( img, j*ZX_BIT_PER_BYTE+k, i,
          zxPixelConv( info->palette[p], &src, &dest ) );
      }
    }
}

/* 16 Pseudo Color */
void _zxBMPRead_4bit(zxBMPInfo *info, zxImage *img)
{
  register int i, j, k;
  ubyte p, p1, p2;
  zxPixelManip src, dest;

  zxPixelManipSet( &src, ZX_BMP_TCBIT );
  zxPixelManipSetDefault( &dest );
  for( i=img->height-1; i>=0; i-- )
    for( j=0; j<info->bpl; j++ ){
      p  = _zxBMPReadVal( info->fp, 1 );
      p1 = ( p >> 4 ) & 0xf;
      p2 =   p        & 0xf;
      k = j * 2;
      if( zxImagePosIsValid( img, k, i ) )
        zxImageCellFromPixel( img, k, i,
          zxPixelConv( info->palette[p1], &src, &dest ) );
      if( zxImagePosIsValid( img, ++k, i ) )
        zxImageCellFromPixel( img, k, i,
          zxPixelConv( info->palette[p2], &src, &dest ) );
    }
}

/* 256 Pseudo Color */
void _zxBMPRead_8bit(zxBMPInfo *info, zxImage *img)
{
  register int i, j;
  zxPixel pixel;
  zxPixelManip src, dest;

  zxPixelManipSet( &src, ZX_BMP_TCBIT );
  zxPixelManipSetDefault( &dest );
  for( i=img->height-1; i>=0; i-- )
    for( j=0; j<info->bpl; j++ ){
      pixel = _zxBMPReadVal( info->fp, 1 );
      zxImageCellFromPixel( img, j, i,
        zxPixelConv( info->palette[pixel], &src, &dest ) );
    }
}

/* True Color */
void _zxBMPRead_TrueColor(zxBMPInfo *info, zxImage *img)
{
  register int i, j;
  zxPixel pixel;
  zxPixelManip src, dest;

  zxPixelManipSet( &src, info->bitcount );
  zxPixelManipSetDefault( &dest );
  for( i=img->height-1; i>=0; i-- ){
    for( j=0; j<info->width; j++ ){
      pixel = _zxBMPReadVal( info->fp, info->bpp );
      zxImageCellFromPixel( img, j, i,
        zxPixelConv( pixel, &src, &dest ) );
    }
    /* alignment */
    for( j=info->bpl-info->width*info->bpp; j>0; j-- )
      _zxBMPReadVal( info->fp, 1 );
  }
}

/* 8bit run length */
void _zxBMPRead_RLE8(zxBMPInfo *info, zxImage *img)
{
  register int i, j, x;
  ubyte num, v;
  zxPixelManip src, dest;

  zxPixelManipSet( &src, ZX_BMP_TCBIT );
  zxPixelManipSetDefault( &dest );
  for( i=img->height-1; i>=0; i-- ){
    for( x=0; ; ){
      num = _zxBMPReadVal( info->fp, 1 );
      if( num == 0x00 ){
        num = _zxBMPReadVal( info->fp, 1 );
        switch( num ){
        case 0x00: goto NEXT; /* EOL */
        case 0x01: return;    /* EOF */
        case 0x02:
          x += _zxBMPReadVal( info->fp, 1 );
          i -= _zxBMPReadVal( info->fp, 1 );
          break;
        default:
          for( j=0; j<num; j++ ){
            v = _zxBMPReadVal( info->fp, 1 );
            zxImageCellFromPixel( img, x++, i,
              zxPixelConv( info->palette[v], &src, &dest ) );
          }
          if( num % 2 )
            _zxBMPReadVal( info->fp, 1 ); /* truncation */
        }
      } else{
        v = _zxBMPReadVal( info->fp, 1 );
        for( j=0; j<num; j++ )
          zxImageCellFromPixel( img, x++, i,
            zxPixelConv( info->palette[v], &src, &dest ) );
      }
    }
  NEXT: ;
  }
}

/* 4bit run length */
void _zxBMPRead_RLE4(zxBMPInfo *info, zxImage *img)
{
  register int i, j, x;
  ubyte num, v, index[2];
  zxPixelManip src, dest;

  zxPixelManipSet( &src, ZX_BMP_TCBIT );
  zxPixelManipSetDefault( &dest );
  for( i=img->height-1; i>=0; i-- ){
    for( x=0; ; ){
      num = _zxBMPReadVal( info->fp, 1 );
      if( num == 0x00 ){
        num = _zxBMPReadVal( info->fp, 1 );
        switch( num ){
        case 0x00: goto NEXT; /* EOL */
        case 0x01: return;    /* EOF */
        case 0x02:
          x += _zxBMPReadVal( info->fp, 1 );
          i -= _zxBMPReadVal( info->fp, 1 );
          break;
        default:
          index[0] = index[1] = 0;
          for( j=0; j<num; j++ ){
            if( j % 2 == 0 ){
              v = _zxBMPReadVal( info->fp, 1 );
              index[0] = ( v >> 4 ) & 0xf;
              index[1] =   v        & 0xf;
            }
            zxImageCellFromPixel( img, x++, i,
              zxPixelConv( info->palette[index[j%2]], &src, &dest ) );
          }
          if( ( num / 2 + num % 2 ) % 2 )
            _zxBMPReadVal( info->fp, 1 ); /* truncation */
        }
      } else{
        v = _zxBMPReadVal( info->fp, 1 );
        index[0] = ( v >> 4 ) & 0xf;
        index[1] =   v        & 0xf;
        for( j=0; j<num; j++ )
          zxImageCellFromPixel( img, x++, i,
            zxPixelConv( info->palette[index[j%2]], &src, &dest ) );
      }
    }
  NEXT: ;
  }
}

void _zxBMPRead(zxBMPInfo *info, zxImage *img)
{
  switch( info->comp ){
  case BI_RGB:
    switch( info->bitcount ){
    case  1: return _zxBMPRead_1bit( info, img );
    case  4: return _zxBMPRead_4bit( info, img );
    case  8: return _zxBMPRead_8bit( info, img );
    case 24:
    case 32: return _zxBMPRead_TrueColor( info, img );
    default: ;
    }
    break;
  case BI_RLE8:
    return _zxBMPRead_RLE8( info, img );
  case BI_RLE4:
    return _zxBMPRead_RLE4( info, img );
  default: ;
  }
}

int zxImageReadBMP(FILE *fp, zxImage *img)
{
  zxBMPInfo info;
  int result = 0;
  uint32_t pos;

  memset( &info, 0, sizeof(zxBMPInfo) ); /* clear BMPInfo instance */
  info.fp = fp;
  pos = ftell( fp );
  info.palette = NULL;
  _zxBMPReadHeader( &info );
  _zxBMPReadPalette( &info );
  if( !zxImageAllocDefault( img, info.width, info.height ) ){
    ZRUNERROR( "cannot allocate enough memory for bitmap" );
    goto TERMINATE;
  }
  fseek( info.fp, pos + info.dataoffset, SEEK_SET );
  _zxBMPRead( &info, img );
  result = 1;

 TERMINATE:
  if( info.palette ) free( info.palette );
  return result;
}

int zxImageReadBMPFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result = 0;

  if( !( fp = fopen( filename, "rb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageReadBMP( fp, img );
  fclose( fp );
  return result;
}

/* BMP writer
 */
void _zxBMPWriteVal(FILE *fp, uint size, uint32_t val)
{
  for( ; size>0; size-- ){
    fputc( (ubyte)( val & 0xff ), fp );
    val >>= ZX_BIT_PER_BYTE;
  }
}

int _zxBMPWriteHeader(zxBMPInfo *info, zxImage *img)
{
#define ZX_BMP_DEFAULT_BPP         3
#define ZX_BMP_DEFAULT_BITCOUNT    ZX_BMP_TCBIT
#define ZX_BMP_DEFAULT_PPM      2834 /* pixels per meter */

  /* header charactor */
  fputc( 'B', info->fp );
  fputc( 'M', info->fp );
  /* image size calculation */
  info->bpp = ZX_BMP_DEFAULT_BPP;
  info->bpl = ( info->width = img->width ) * info->bpp;
  _zxBMPAlignBPL( &info->bpl );
  info->imagesize = info->bpl * ( info->height = img->height );
  /* file size */
  _zxBMPWriteVal( info->fp, 4, ZX_BMP_HEADERSIZE+info->imagesize );
  _zxBMPWriteVal( info->fp, 4, 0 ); /* reserved */
  _zxBMPWriteVal( info->fp, 4, ZX_BMP_HEADERSIZE ); /* data offset */
  /* info header size */
  _zxBMPWriteVal( info->fp, 4, ZX_BMP_INFOHEADERSIZE );
  _zxBMPWriteVal( info->fp, 4, info->width ); /* image width */
  _zxBMPWriteVal( info->fp, 4, info->height ); /* image height */
  _zxBMPWriteVal( info->fp, 2, 1 ); /* number of plane */
  _zxBMPWriteVal( info->fp, 2, ZX_BMP_DEFAULT_BITCOUNT ); /* bit count */
  _zxBMPWriteVal( info->fp, 4, BI_RGB ); /* type of compression */
  _zxBMPWriteVal( info->fp, 4, info->imagesize ); /* image size */
  _zxBMPWriteVal( info->fp, 4, ZX_BMP_DEFAULT_PPM ); /* X pixels/meter */
  _zxBMPWriteVal( info->fp, 4, ZX_BMP_DEFAULT_PPM ); /* Y pixels/meter */
  _zxBMPWriteVal( info->fp, 4, 0 ); /* number of actually used colors */
  _zxBMPWriteVal( info->fp, 4, 0 ); /* number of important color */
  return 1;
}

void _zxBMPWrite(zxBMPInfo *info, zxImage *img)
{
  register int i, j;
  zxPixel pixel;
  zxPixelManip src, dest;

  zxPixelManipSetDefault( &src );
  zxPixelManipSet( &dest, ZX_BMP_DEFAULT_BITCOUNT );
  for( i=img->height-1; i>=0; i-- ){
    for( j=0; j<info->width; j++ ){
      pixel = zxPixelConv( zxImageCellPixel( img, j, i ), &src, &dest );
      _zxBMPWriteVal( info->fp, info->bpp, pixel );
    }
    /* alignment */
    for( j=info->bpl-info->width*info->bpp; j>0; j-- )
      _zxBMPWriteVal( info->fp, 1, 0 );
  }
}

int zxImageWriteBMP(FILE *fp, zxImage *img)
{
  zxBMPInfo info;

  info.fp = fp;
  _zxBMPWriteHeader( &info, img );
  _zxBMPWrite( &info, img );
  return 1;
}

int zxImageWriteBMPFile(zxImage *img, char filename[])
{
  FILE *fp;

  if( !( fp = fopen( filename, "wb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  zxImageWriteBMP( fp, img );
  fclose( fp );
  return 1;
}

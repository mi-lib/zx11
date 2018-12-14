/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_pdt - PDT(graphic format for AVG32) support.
 *
 * AVG32 is a product by versammeln(an online distribution
 * brand of VisualArt's).
 * Special thanks to JAGARL (Mr. Kazunori Ueno).
 */

#include <zx11/zximage_pdt.h>

bool zxImageFileIsPDT(char filename[])
{
  return zxImageFileIdent( filename, "PDT", 3 );
}

#define ZX_PDT_HEADER_SIZE 32
#define ZX_PDT_DEPTH       24
#define ZX_PDT_MASK_DEPTH   8
#define ZX_PDT_BPP          3
#define ZX_PDT_MASK_BPP     1

/* pixel manipulator */
static zxPixelManip _zx_pdt_pm_src, _zx_pdt_pm_dest;

/* format identifier */
enum{ ZX_PDT_INVALID=-1, ZX_PDT10=0, ZX_PDT11 };

/* header */
typedef struct{
#define ZX_PDT_IDENT        8
  char ident[ZX_PDT_IDENT]; /* "PDT10" or "PDT11" */
  uint32_t size;            /* file size */
  uint32_t width;           /* width(640) */
  uint32_t height;          /* height(480) */
  uint32_t x_mask;          /* x of mask(0) */
  uint32_t y_mask;          /* y of mask(0)*/
  uint32_t mask_offset;     /* mask (if it exists) */
} zxPDTInfo;

static ubyte _zxPDTReadByte(FILE *fp);
static void _zxPDTReadBytes(FILE *fp, ubyte *buf, int size);
static int _zxPDTCheck(zxPDTInfo *info);

static bool _zxPDTReadHeader(FILE *fp, zxPDTInfo *info);

static void _zxPDTReadPixel(FILE *fp, zxImage *img, ulong offset);
static void _zxPDTReadMask(FILE *fp, zxImage *img, ulong offset);
/* indices in PDT11 also consists of one-byte values */
static void _zxPDTReadIndex(FILE *fp, zxImage *img, ulong offset);

static void _zxPDTReadData(FILE *fp, zxImage *img, void (* read_pixel)(FILE*,zxImage*,ulong), ubyte psize, void (* size_and_index)(FILE*,zxImage*,ulong*,ulong*,ubyte**));

static void _zxPDT10GetPixelSizeAndIndex(FILE *fp, zxImage *img, ulong *size, ulong *index, ubyte **buf);
static void _zxPDT10GetMaskSizeAndIndex(FILE *fp, zxImage *img, ulong *size, ulong *index, ubyte **buf);
static void _zxPDT11GetPixelSizeAndIndex(FILE *fp, zxImage *img, ulong *size, ulong *index, ubyte **buf);
static void _zxPDT10ReadPixelData(FILE *fp, zxImage *img);
static void _zxPDT10ReadMaskData(FILE *fp, zxImage *img);
static void _zxPDT11ReadPixelData(FILE *fp, zxImage *img);

/* index table for PDT11 */
#define ZX_PDT11_INDEX_SIZE 16
static uint _zx_pdt11_index[ZX_PDT11_INDEX_SIZE];

ubyte _zxPDTReadByte(FILE *fp)
{
  if( feof( fp ) ){
    ZRUNERROR( "unexpected EOF" );
    return 0;
  }
  return fgetc( fp );
}

void _zxPDTReadBytes(FILE *fp, ubyte *buf, int size)
{
  register int i;

  for( i=0; i<size; i++ )
    buf[i] = _zxPDTReadByte( fp );
}

int _zxPDTCheck(zxPDTInfo *info)
{
  if( !strncmp( info->ident, "PDT10", 5 ) ) return ZX_PDT10;
  if( !strncmp( info->ident, "PDT11", 5 ) ) return ZX_PDT11;
  ZRUNWARN( "not a PDT file" );
  return ZX_PDT_INVALID;
}

bool _zxPDTReadHeader(FILE *fp, zxPDTInfo *info)
{
  _zxPDTReadBytes( fp, (ubyte *)info, ZX_PDT_HEADER_SIZE );
  if( _zxPDTCheck( info ) == ZX_PDT_INVALID ) return false;
  zxPixelManipSet( &_zx_pdt_pm_src,  ZX_PDT_DEPTH );
  zxPixelManipSet( &_zx_pdt_pm_dest, zxdepth );
  return true;
}

void _zxPDTReadPixel(FILE *fp, zxImage *img, ulong offset)
{
  zxPixel pixel = 0;

  _zxPDTReadBytes( fp, (ubyte *)&pixel, ZX_PDT_BPP );
  *(zxPixel *)&img->buf[offset]
    = zxPixelConv( pixel, &_zx_pdt_pm_src, &_zx_pdt_pm_dest );
}

void _zxPDTReadMask(FILE *fp, zxImage *img, ulong offset)
{
  img->mask_buf[offset] = _zxPDTReadByte( fp );
}

/* indices in PDT11 also consists of one-byte values */
void _zxPDTReadIndex(FILE *fp, zxImage *img, ulong offset)
{
  img->buf[offset] = _zxPDTReadByte( fp );
}

void _zxPDTReadData(FILE *fp, zxImage *img, void (* read_pixel)(FILE*,zxImage*,ulong), ubyte psize, void (* size_and_index)(FILE*,zxImage*,ulong*,ulong*,ubyte**))
{
  register byte flag = 0, len = 0;
  ulong i, size, index, data_size;
  ubyte *bhead, *bp;

  data_size = img->width * img->height * psize;
  for( i=0; !feof(fp) && i<data_size; ){
    if( --len < 0 ){
      flag = _zxPDTReadByte(fp);
      len = 8;
      continue;
    }
    if( flag & 0x80 ){
      read_pixel( fp, img, i );
      i += psize;
    } else{
      size_and_index( fp, img, &size, &index, &bhead );
      for( bp=&bhead[i]-index; size>0;
           size--, i+=psize, bp+=psize )
        memcpy( &bhead[i], bp, psize );
    }
    flag <<= 1;
  }
}

void _zxPDT10GetPixelSizeAndIndex(FILE *fp, zxImage *img, ulong *size, ulong *index, ubyte **buf)
{
  uint val = 0;

  _zxPDTReadBytes( fp, (ubyte *)&val, 2 );
  *size = ( val & 0xf ) + 1;
  *index = ( ( val >> 4 ) + 1 ) * img->bpp;
  *buf = img->buf;
}

void _zxPDT10GetMaskSizeAndIndex(FILE *fp, zxImage *img, ulong *size, ulong *index, ubyte **buf)
{
  *size = _zxPDTReadByte(fp) + 2;
  *index = ( _zxPDTReadByte(fp) + 1 ) * sizeof(ubyte);
  *buf = img->mask_buf;
}

void _zxPDT11GetPixelSizeAndIndex(FILE *fp, zxImage *img, ulong *size, ulong *index, ubyte **buf)
{
  ubyte val;

  val = _zxPDTReadByte( fp );
  *size = ( val >> 4 ) + 2;
  *index = _zx_pdt11_index[val & 0xf];
  *buf = img->buf;
}

void _zxPDT10ReadPixelData(FILE *fp, zxImage *img)
{
  /* data is compressed by LZ method */
  /* pixel : 24 bit
   * offset: 12 bit
   * size  :  4 bit + 1
   */
  _zxPDTReadData( fp, img, _zxPDTReadPixel, img->bpp, _zxPDT10GetPixelSizeAndIndex );
}

void _zxPDT10ReadMaskData(FILE *fp, zxImage *img)
{
  /* mask is compressed by LZ method */
  /* mask  : 8 bit
   * offset: 8 bit
   * size  : 8 bit + 2
   */
  _zxPDTReadData( fp, img, _zxPDTReadMask, sizeof(ubyte), _zxPDT10GetMaskSizeAndIndex );
}

void _zxPDT11ReadPixelData(FILE *fp, zxImage *img)
{
#define ZX_PDT11_INDEX_OFFSET 1056
  /* data is compressed by LZ method */
  /* pixel : 24 bit
   * size  : 4 bit + 2
   * offset: index_table[4 bit]
   * [Note] the order of size and offset is inversed from PDT10.
   */
  zxImage index_buf;
  register int i, size;
  ubyte *bp;

  /* allocate index array */
  zxImageAlloc( &index_buf, img->width, img->height, ZX_BIT_PER_BYTE );
  /* read index table */
  fseek( fp, ZX_PDT11_INDEX_OFFSET, SEEK_SET );
  _zxPDTReadBytes( fp, (ubyte *)_zx_pdt11_index, ZX_PDT11_INDEX_SIZE*4 );
  /* read index array */
  _zxPDTReadData( fp, &index_buf, _zxPDTReadIndex, sizeof(ubyte), _zxPDT11GetPixelSizeAndIndex );
  /* paste pixel data, refering to the palette */
  size = img->width * img->height;
  for( bp=img->buf, i=0; i<size; i++, bp+=img->bpp ){
    fseek( fp, ZX_PDT_HEADER_SIZE+index_buf.buf[i], SEEK_SET );
    _zxPDTReadBytes( fp, bp, img->bpp );
  }
  zxImageDestroy( &index_buf );
}

bool zxImageReadPDT(FILE *fp, zxImage *img)
{
  zxPDTInfo info;

  if( !_zxPDTReadHeader( fp, &info ) ) return false;
  zxImageAllocDefault( img, info.width, info.height );
  if( _zxPDTCheck( &info ) == ZX_PDT10 )
    _zxPDT10ReadPixelData( fp, img );
  else /* ZX_PDT11 case */
    _zxPDT11ReadPixelData( fp, img );
  /* mask */
  if( info.mask_offset > 0 ){
    if( !zxImageAllocMask(img) ) return false;
    fseek( fp, info.mask_offset, SEEK_SET );
    _zxPDT10ReadMaskData( fp, img );
  }
  return true;
}

int zxImageReadPDTFile(zxImage *img, char filename[])
{
  FILE *fp;

  if( !( fp = fopen( filename, "rb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  zxImageReadPDT( fp, img );
  fclose( fp );
  return 1;
}

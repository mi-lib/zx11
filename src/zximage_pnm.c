/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_pnm - image I/O as Portable Bitmap/Graymap/Pixmap on X11
 */

#include <zx11/zximage_pnm.h>

/* row data processing as Portable Bitmap/Graymap/Pixmap */

/* file format identification */

bool zxImageFileIsPBM(char filename[])
{
  return zxImageFileIdent( filename, "P1", 2 ) ||
         zxImageFileIdent( filename, "P4", 2 );
}

bool zxImageFileIsPGM(char filename[])
{
  return zxImageFileIdent( filename, "P2", 2 ) ||
         zxImageFileIdent( filename, "P5", 2 );
}

bool zxImageFileIsPPM(char filename[])
{
  return zxImageFileIdent( filename, "P3", 2 ) ||
         zxImageFileIdent( filename, "P6", 2 );
}

bool zxImageFileIsPNM(char filename[])
{
  return zxImageFileIsPBM( filename ) ||
         zxImageFileIsPGM( filename ) ||
         zxImageFileIsPPM( filename );
}

/* input of PNM file */

static zxPixelManip _zx_pnm_pm;

/* PBM (Portable Bitmap), ASCII mode */
static void _zxImageReadPBMASCII(FILE *fp, zxImage *img, int x, int y)
{
  ubyte val;

  val = zFInt( fp ) ? 0 : 0xff;
  zxImageCellFromRGB( img, &_zx_pnm_pm, x, y, val, val, val );
}

/* PBM (Portable Bitmap), binary mode */
static ubyte *__zx_pbm_buf = NULL;
static uint __zx_pbm_rowsize = 0;
static bool _zxImageCreatePBMBuf(uint width)
{
  __zx_pbm_rowsize = width / 8 + ( ( width % 8 ) ? 1 : 0 );
  if( !( __zx_pbm_buf = zAlloc( ubyte, __zx_pbm_rowsize ) ) ){
    ZALLOCERROR();
    return false;
  }
  return true;
}

static void _zxImageDestroyPBMBuf(void)
{
  zFree( __zx_pbm_buf );
}

static void _zxImageReadPBMBIN(FILE *fp, zxImage *img, int x, int y)
{
  static uint k=0;
  static ubyte mask=0x80;
  register ubyte val;

  if( x == 0 ){
    k = 0;
    mask = 0x80;
    if( fread( __zx_pbm_buf, 1, __zx_pbm_rowsize, fp ) != 1 );
  }
  val = __zx_pbm_buf[k] & mask ? 0 : 0xff;
  zxImageCellFromRGB( img, &_zx_pnm_pm, x, y, val, val, val );

  if( ( mask >>= 1 ) == 0 ){
    mask = 0x80;
    k++;
  }
}

/* PGM (Portable Graymap), ASCII mode */
static void _zxImageReadPGMASCII(FILE *fp, zxImage *img, int x, int y)
{
  ubyte val;

  val = zFInt( fp );
  zxImageCellFromRGB( img, &_zx_pnm_pm, x, y, val, val, val );
}

/* PGM (Portable Graymap), binary mode */
static void _zxImageReadPGMBIN(FILE *fp, zxImage *img, int x, int y)
{
  ubyte val;

  if( fread( &val, 1, 1, fp ) != 1 );
  zxImageCellFromRGB( img, &_zx_pnm_pm, x, y, val, val, val );
}

/* PPM (Portable Pixmap), ASCII mode */
static void _zxImageReadPPMASCII(FILE *fp, zxImage *img, int x, int y)
{
  ubyte r, g, b;

  r = zFInt( fp );
  g = zFInt( fp );
  b = zFInt( fp );
  zxImageCellFromRGB( img, &_zx_pnm_pm, x, y, r, g, b );
}

/* PPM (Portable Pixmap), binary mode */
static void _zxImageReadPPMBIN(FILE *fp, zxImage *img, int x, int y)
{
  ubyte r, g, b;

  if( fread( &r, 1, 1, fp ) != 1 );
  if( fread( &g, 1, 1, fp ) != 1 );
  if( fread( &b, 1, 1, fp ) != 1 );
  zxImageCellFromRGB( img, &_zx_pnm_pm, x, y, r, g, b );
}

static void (* __zximage_pnm_read_pixel[])(FILE *, zxImage *, int, int) = {
  NULL /* dummy */,
  _zxImageReadPBMASCII,
  _zxImageReadPGMASCII,
  _zxImageReadPPMASCII,
  _zxImageReadPBMBIN,
  _zxImageReadPGMBIN,
  _zxImageReadPPMBIN,
};

static int __zximage_pnm_read_header_str(FILE *fp, char *buf)
{
  do{
    if( feof( fp ) || !fgets( buf, BUFSIZ, fp ) ){
      ZRUNERROR( "unexpected EOF" );
      return 0;
    }
  } while( buf[0] == '#' || buf[0] == '\n' || buf[0] == '\r' );
  return 1;
}

int zxImageReadPNMHeader(FILE *fp, zxImage *img)
{
  char buf[BUFSIZ];
  ubyte type;
  int max_intensity;

  __zximage_pnm_read_header_str( fp, buf );
  if( buf[0] != 'P' ){
    ZRUNERROR( "not a Portable Bitmap/Graymap/Pixmap file" );
    return 0;
  }
  type = buf[1] - '0';
  if( type == 0 || type > 6 ){
    ZRUNERROR( "invalid identifier for a Portable Bitmap/Graymap/Pixmap file" );
    return 0;
  }
  __zximage_pnm_read_header_str( fp, buf );
  sscanf( buf, "%d %d", &img->width, &img->height );
  if( type != 1 && type != 4 ){
    /* graymap/pixmap allows to specify the maximum intensity */
    __zximage_pnm_read_header_str( fp, buf );
    sscanf( buf, "%d", &max_intensity );
    /* however, only 0xff is feasible as the maximum intensity... */
    if( max_intensity != 0xff ){
      ZRUNERROR( "unsupported maximum intensity %d", max_intensity );
      return 0;
    }
  }
  img->bpp = 4;
  return type;
}

int zxImageReadPNM(FILE *fp, zxImage *img)
{
  register int i, j;
  ubyte type;
  void (* read_pixel)(FILE *, zxImage *, int, int);

  zxImageInit( img );
  if( ( type = zxImageReadPNMHeader( fp, img ) ) == ZX_PNM_INVALID ){
    ZRUNERROR( "unknown file format" );
    return 0;
  }
  /* PBM requires raster buffer for each row */
  if( type == ZX_PBM_BIN && !_zxImageCreatePBMBuf( img->width ) ) return 0;

  read_pixel = __zximage_pnm_read_pixel[type];
  if( !( img->buf = zAlloc( ubyte, img->width*img->height*img->bpp ) ) ){
    ZRUNERROR( "cannot allocate enough memory for image buffer" );
    return 0;
  }
  zxPixelManipSetDefault( &_zx_pnm_pm );
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      read_pixel( fp, img, j, i );

  /* freeing raster buffer for PBM */
  if( type == ZX_PBM_BIN )
    _zxImageDestroyPBMBuf();
  return 1;
}

int zxImageReadPNMFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result;

  if( !( fp = fopen( filename, "rb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageReadPNM( fp, img );
  fclose( fp );
  return result;
}

/* output of PNM file */
/* only binary output is supported. */

int zxImageWritePBM(FILE *fp, zxImage *img)
{
  register int i, j;
  register ubyte mask;
  ubyte r, g, b, val;
#define _ZX_PBM_THRESHOLD 0x3f

  fprintf( fp, "P4\n" ); /* magic number for PBM */
  fprintf( fp, "# data generated by ZX11\n" );
  fprintf( fp, "%d %d\n", img->width, img->height );
  zxPixelManipSetDefault( &_zx_pnm_pm );
  for( i=0; i<img->height; i++ ){
    for( mask=0x80, val=0, j=0; j<img->width; j++ ){
      zxImageCellRGB( img, &_zx_pnm_pm, j, i, &r, &g, &b );
      if( r < _ZX_PBM_THRESHOLD && g < _ZX_PBM_THRESHOLD && b < _ZX_PBM_THRESHOLD )
        val |= mask;
      if( ( mask >>= 1 ) == 0 ){
        fwrite( &val, 1, 1, fp );
        mask = 0x80;
        val = 0;
      }
    }
    if( mask != 0x80 )
      fwrite( &val, 1, 1, fp );
  }
  return 1;
}

int zxImageWritePBMFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result;

  if( !( fp = fopen( filename, "wb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageWritePBM( fp, img );
  fclose( fp );
  return result;
}

int zxImageWritePGM(FILE *fp, zxImage *img)
{
  register int i, j;
  ubyte r, g, b, val;

  fprintf( fp, "P5\n" ); /* magic number for PGM */
  fprintf( fp, "# data generated by ZX11\n" );
  fprintf( fp, "%d %d\n", img->width, img->height );
  fprintf( fp, "255\n" );
  zxPixelManipSetDefault( &_zx_pnm_pm );
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      zxImageCellRGB( img, &_zx_pnm_pm, j, i, &r, &g, &b );
      val = ( r + g + b ) / 3;
      fwrite( &val, 1, 1, fp );
    }
  return 1;
}

int zxImageWritePGMFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result;

  if( !( fp = fopen( filename, "wb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageWritePGM( fp, img );
  fclose( fp );
  return result;
}

int zxImageWritePPM(FILE *fp, zxImage *img)
{
  register int i, j;
  ubyte r, g, b;

  fprintf( fp, "P6\n" ); /* magic number for PPM */
  fprintf( fp, "# data generated by ZX11\n" );
  fprintf( fp, "%d %d\n", img->width, img->height );
  fprintf( fp, "255\n" );
  zxPixelManipSetDefault( &_zx_pnm_pm );
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      zxImageCellRGB( img, &_zx_pnm_pm, j, i, &r, &g, &b );
      fwrite( &r, 1, 1, fp );
      fwrite( &g, 1, 1, fp );
      fwrite( &b, 1, 1, fp );
    }
  return 1;
}

int zxImageWritePPMFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result;

  if( !( fp = fopen( filename, "wb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageWritePPM( fp, img );
  fclose( fp );
  return result;
}

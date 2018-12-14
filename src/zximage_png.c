/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_png - PNG support on zxImage(requires libpng).
 */

#include <zx11/zximage_png.h>

bool zxImageFileIsPNG(char filename[])
{
#define ZX_PNG_IDENT_SIZE 4
  const char __zx_png_ident[] = {
    0x89, 0x50, 0x4e, 0x47,
  };
  return zxImageFileIdent( filename, __zx_png_ident, ZX_PNG_IDENT_SIZE );
}

typedef struct{
  png_structp png_ptr;
  png_infop info_ptr, end_ptr;

  int bit_depth, color_type, interlace_type;
  int compression_type, filter_type;
} zxPNG;

static ubyte **__zx_png_buf_alloc(zxImage *img);

static void zxPNGInit(zxPNG *png);
static int zxPNGCreateInfo(zxPNG *png);
static int zxPNGCreateReadInfo(zxPNG *png);
static void zxPNGDestroyRead(zxPNG *png);
static int zxPNGRead(zxImage *img, zxPNG *png, FILE *fp);

static int zxPNGCreateWriteInfo(zxPNG *png);
static void zxPNGDestroyWrite(zxPNG *png);
static int zxPNGWrite(zxImage *img, zxPNG *png, FILE *fp);

ubyte **__zx_png_buf_alloc(zxImage *img)
{
  register int i, j;
  ubyte **buf, *bp;
  ulong byteperrow, destbpp;
  zxPixel pixel;
  zxPixelManip src, dest;

  destbpp = ZX_BIT_PER_BYTE * 3; /* for RGB(ordinally 24bpp) */
  byteperrow = img->width * destbpp;
  buf = zAlloc( ubyte*, img->height );
  if( buf == NULL ){
    ZRUNERROR( "cannot allocate buffer for PNG" );
    return NULL;
  }
  zxPixelManipSetDefault( &src );
  zxPixelManipSet( &dest, destbpp );
  for( i=0; i<img->height; i++ ){
    buf[i] = zAlloc( ubyte, byteperrow );
    if( buf[i] == NULL )
      ZRUNERROR( "cannot allocate buffer for PNG" );
    else
      for( j=0; j<img->width; j++ ){
        pixel = zxPixelConv( zxImageCellPixel( img, j, i ),
          &src, &dest );
        bp = &buf[i][j*3];
        dest.PixelRGB( pixel, bp, bp+1, bp+2 );
      }
  }
  return buf;
}

int zxPNGCheck(FILE *fp)
{
#define ZX_PNG_CHECK_BYTE 8
  ubyte header[ZX_PNG_CHECK_BYTE];

  if( fread( header, sizeof(ubyte), ZX_PNG_CHECK_BYTE, fp ) != ZX_PNG_CHECK_BYTE );
  return !png_sig_cmp( header, 0, ZX_PNG_CHECK_BYTE );
}

int zxPNGCheckFile(const char filename[])
{
  FILE *fp;
  int result;

  fp = fopen( filename, "rb" );
  if( fp == NULL ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxPNGCheck( fp );
  fclose( fp );
  return result;
}

void zxPNGInit(zxPNG *png)
{
  png->png_ptr = NULL;
  png->info_ptr = png->end_ptr = NULL;
}

int zxPNGCreateInfo(zxPNG *png)
{
  png->info_ptr = png_create_info_struct( png->png_ptr );
  if( png->info_ptr == NULL ){
    ZRUNERROR( "cannot allocate PNG info" );
    return 0;
  }
  png->end_ptr = png_create_info_struct( png->png_ptr );
  if( png->end_ptr == NULL ){
    ZRUNERROR( "cannot allocate PNG end info" );
    return 0;
  }
  return 1;
}

/* read
 */
int zxPNGCreateReadInfo(zxPNG *png)
{
  png->png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING,
    NULL, NULL, NULL );
  if( png->png_ptr == NULL ){
    ZRUNERROR( "cannot allocate PNG struct" );
    return 0;
  }
  return zxPNGCreateInfo( png );
}

void zxPNGDestroyRead(zxPNG *png)
{
  png_destroy_read_struct( &png->png_ptr, &png->info_ptr, &png->end_ptr );
}

int zxPNGRead(zxImage *img, zxPNG *png, FILE *fp)
{
  register int i, j, k;
  png_uint_32 width, height;
  int byteperrow, bpp;
  png_bytep buf;
  zxPixel pixel;
  zxPixelManip src, dest;

  /* create info */
  if( zxPNGCreateReadInfo( png ) == 0 )
    return 0; /* fail to create info struct */

  /* read & update info */
  png_init_io( png->png_ptr, fp );
  png_read_info( png->png_ptr, png->info_ptr );
  png_get_IHDR( png->png_ptr, png->info_ptr,
    &width, &height, &png->bit_depth, &png->color_type,
    &png->interlace_type, &png->compression_type, &png->filter_type );
  if( png->bit_depth < 8 )
    png_set_packing( png->png_ptr );
  if( png->color_type == PNG_COLOR_TYPE_PALETTE )
    png_set_palette_to_rgb( png->png_ptr );

  png_read_update_info( png->png_ptr, png->info_ptr );
  width = png_get_image_width( png->png_ptr, png->info_ptr );
  height = png_get_image_height( png->png_ptr, png->info_ptr );
  byteperrow = png_get_rowbytes( png->png_ptr, png->info_ptr );
  bpp = byteperrow / width;

  /* allocate buffer */
  buf = (png_bytep)malloc( byteperrow );
  if( buf == NULL ){
    ZRUNERROR( "cannot allocate buffer for PNG" );
    return 0;
  }
  if( !zxImageAllocDefault( img, width, height ) ){
    ZRUNERROR( "cannot allocate zxImage" );
    free( buf );
    return 0;
  }

  /* read image */
  zxPixelManipSet( &src, bpp * ZX_BIT_PER_BYTE );
  zxPixelManipSetDefault( &dest );
  for( i=0; i<height; i++ ){
    png_read_row( png->png_ptr, buf, NULL );
    for( j=0; j<width; j++ ){
      k = j * bpp;
      pixel = src.PixelFromRGB( buf[k], buf[k+1], buf[k+2] );
      zxImageCellFromPixel( img, j, i,
        zxPixelConv( pixel, &src, &dest ) );
    }
  }
  png_read_end( png->png_ptr, png->info_ptr );

  free( buf );
  return 1;
}

int zxImageReadPNG(FILE *fp, zxImage *img)
{
  zxPNG png;
  ulong pos;

  zxPNGInit( &png );
  pos = ftell( fp );
  if( !zxPNGCheck( fp ) ){
    ZRUNERROR( "not a PNG file" );
    return 0;
  }
  fseek( fp, pos, SEEK_SET );
  zxPNGRead( img, &png, fp );
  zxPNGDestroyRead( &png );
  return 1;
}

int zxImageReadPNGFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result = 0;

  fp = fopen( filename, "rb" );
  if( fp == NULL ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageReadPNG( fp, img );
  fclose( fp );
  return result;
}

/* write
 */
int zxPNGCreateWriteInfo(zxPNG *png)
{
  png->png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING,
    NULL, NULL, NULL );
  if( png->png_ptr == NULL ){
    ZRUNERROR( "cannot allocate PNG struct" );
    return 0;
  }
  return zxPNGCreateInfo( png );
}

void zxPNGDestroyWrite(zxPNG *png)
{
  png_destroy_write_struct( &png->png_ptr, &png->info_ptr );
}

int zxPNGWrite(zxImage *img, zxPNG *png, FILE *fp)
{
  register int i;
  ubyte **buf;

  /* create info */
  if( zxPNGCreateWriteInfo( png ) == 0 )
    return 0; /* fail to create info struct */

  /* allocate buffer */
  buf = __zx_png_buf_alloc( img );
  if( buf == NULL ) return 0;

  /* write image */
  png_init_io( png->png_ptr, fp );
  png_set_IHDR( png->png_ptr, png->info_ptr, img->width, img->height,
    ZX_BIT_PER_BYTE, PNG_COLOR_TYPE_RGB,
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
    PNG_FILTER_TYPE_DEFAULT );
  png_write_info( png->png_ptr, png->info_ptr );
  png_write_image( png->png_ptr, buf );
  png_write_end( png->png_ptr, png->info_ptr );
  zxPNGDestroyWrite( png );

  for( i=0; i<img->height; i++ )
    free( buf[i] );
  free( buf );
  return 1;
}

int zxImageWritePNG(FILE *fp, zxImage *img)
{
  zxPNG png;

  zxPNGInit( &png );
  return zxPNGWrite( img, &png, fp );
}

int zxImageWritePNGFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result = 0;

  fp = fopen( filename, "wb" );
  if( fp == NULL ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageWritePNG( fp, img );
  fclose( fp );
  return result;
}

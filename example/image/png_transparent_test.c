#include <zx11/zxpng.h>

typedef struct{
  png_structp png_ptr;
  png_infop info_ptr, end_ptr;

  int bit_depth, color_type, interlace_type;
  int compression_type, filter_type;
} zxPNG;

static void zxPNGInit(zxPNG *png);
static int zxPNGCreateInfo(zxPNG *png);
static int zxPNGCreateReadInfo(zxPNG *png);
static void zxPNGDestroyRead(zxPNG *png);
static int zxPNGRead(zxImage *img, zxPNG *png, FILE *fp);

int zxPNGCheck(FILE *fp)
{
#define ZX_PNG_CHECK_BITE 8
  ubyte header[ZX_PNG_CHECK_BITE];

  fread( header, sizeof(ubyte), ZX_PNG_CHECK_BITE, fp );
  return !png_sig_cmp( header, 0, ZX_PNG_CHECK_BITE );
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

  /* transparency info */
  {
    png_bytep trans;
    int num_trans;
    png_color_16p trans_values;
    if( png_get_tRNS( png->png_ptr, png->info_ptr, &trans, &num_trans, &trans_values ) ){
      printf( "n=%d\n", num_trans );
    } else
      printf( "no transparency.\n" );
  }

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

int _zxImageReadPNG(FILE *fp, zxImage *img)
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

int _zxImageReadPNGFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result = 0;

  fp = fopen( filename, "rb" );
  if( fp == NULL ){
    ZOPENERROR( filename );
    return 0;
  }
  result = _zxImageReadPNG( fp, img );
  fclose( fp );
  return result;
}

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat;

  zxInit();
  if( _zxImageReadPNGFile( &dat, argv[1] ) == 0 ) exit( 1 );

  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxClose();
  return 0;
} 

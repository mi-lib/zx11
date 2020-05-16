/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_jpeg - JPEG support (requires libjpeg)
 */

#include <zx11/zximage_jpeg.h>
#include <setjmp.h>

bool zxImageFileIsJPEG(char filename[])
{
#define ZX_JPEG_IDENT_SIZE 2
  const char __zx_jpeg_ident[] = {
    /*
    0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46,
    */
    0xff, 0xd8,
  };
  return zxImageFileIdent( filename, __zx_jpeg_ident, ZX_JPEG_IDENT_SIZE );
}

static void _zx_jpg_error_exit(j_common_ptr cinfo);
static void _zx_jpg_conv16(zxPixelManip *pm, ubyte *buf, int w, int x, int y, ubyte rgb[]);
static void _zx_jpg_conv32(zxPixelManip *pm, ubyte *buf, int w, int x, int y, ubyte rgb[]);

void _zx_jpg_error_exit(j_common_ptr cinfo)
{
  sigjmp_buf setjmp_buffer;

  cinfo->err->output_message( cinfo );
  siglongjmp( setjmp_buffer, 1 );
}

void _zx_jpg_conv16(zxPixelManip *pm, ubyte *buf, int w, int x, int y, ubyte rgb[])
{
  ((uint16_t *)buf)[y*w+x] = pm->PixelFromRGB( rgb[0]>>3, rgb[1]>>3, rgb[2]>>3 );
}

void _zx_jpg_conv32(zxPixelManip *pm, ubyte *buf, int w, int x, int y, ubyte rgb[])
{
  ((uint32_t *)buf)[y*w+x] = pm->PixelFromRGB( rgb[0], rgb[1], rgb[2] );
}

int zxImageReadJPEG(FILE *fp, zxImage *img)
{
  register int x, y, i;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr pub;
  JSAMPARRAY jb;
  int colsize, result = 0;
  void (* conv)(zxPixelManip*, ubyte*, int, int, int, ubyte *);
  zxPixelManip pm;

  cinfo.err = jpeg_std_error( &pub );
  pub.error_exit = _zx_jpg_error_exit;
  jpeg_create_decompress( &cinfo );
  jpeg_stdio_src( &cinfo, fp );
  jpeg_read_header( &cinfo, False );
  jpeg_start_decompress( &cinfo );

  colsize = cinfo.output_width * cinfo.output_components;
  jb = (*cinfo.mem->alloc_sarray)( (j_common_ptr)&cinfo, JPOOL_IMAGE, colsize, 1 );
  switch( zxImageBPPDefault() ){
  case 2:
    conv = _zx_jpg_conv16;
    zxPixelManipSet( &pm, 16 );
    break;
  case 4:
    conv = _zx_jpg_conv32;
    zxPixelManipSetDefault( &pm );
    break;
  default:
    ZRUNERROR( "unsupported color depth" );
    goto TERMINATE;
  }
  if( !zxImageAllocDefault( img, cinfo.output_width, cinfo.output_height ) ){
    ZRUNERROR( "cannot allocate enough memory for JPEG image" );
    goto TERMINATE;
  }
  for( y=0; cinfo.output_scanline < (unsigned)img->height; y++ ){
    jpeg_read_scanlines( &cinfo, jb, 1 );
    for( x=0, i=0; i<colsize; i+=cinfo.output_components, x++ )
      (*conv)( &pm, img->buf, img->width, x, y, &jb[0][i] );
  }
  jpeg_finish_decompress( &cinfo );
  result = 1;

 TERMINATE:
  jpeg_destroy_decompress( &cinfo );
  return result;
}

int zxImageReadJPEGFile(zxImage *img, char filename[])
{
  FILE *fp;
  int result;

  if( ( fp = fopen( filename, "rb" ) ) == NULL ){ 
    ZRUNERROR( "invalid JPEG file" );
    return 0;
  }
  result = zxImageReadJPEG( fp, img );
  fclose( fp );
  return result;
}

int zxImageWriteJPEG(FILE *fp, zxImage *img, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPARRAY buf;
  zxPixelManip pm;
  register int i, j;
  int ret = 1;

  jpeg_create_compress( &cinfo );
  cinfo.err = jpeg_std_error( &jerr );
  jpeg_stdio_dest( &cinfo, fp );

  cinfo.image_width  = img->width;
  cinfo.image_height = img->height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults( &cinfo );
  jpeg_set_quality( &cinfo, quality, TRUE );

  jpeg_start_compress( &cinfo, TRUE );

  if( !( buf = (JSAMPARRAY)malloc( sizeof(JSAMPROW)*img->height ) ) ){
    ret = 0;
    goto TERMINATE;
  }
  zxPixelManipSetDefault( &pm );
  for( i=0; i<img->height; i++ )
    if( ( buf[i] = (JSAMPROW)malloc( sizeof(JSAMPLE)*img->width*3 ) ) )
      for( j=0; j<img->width; j++ )
        zxImageCellRGB( img, &pm, j, i, &buf[i][j*3], &buf[i][j*3+1], &buf[i][j*3+2] );
  jpeg_write_scanlines( &cinfo, buf, img->height );
  for( i=0; i<img->height; i++ ) free( buf[i] );

 TERMINATE:
  free( buf );
  jpeg_finish_compress( &cinfo );
  jpeg_destroy_compress( &cinfo );
  return ret;
}

int zxImageWriteJPEGFile(zxImage *img, char filename[], int quality)
{
  FILE *fp;
  int result;

  if( ( fp = fopen( filename, "wb" ) ) == NULL ){ 
    ZOPENERROR( filename );
    return 0;
  }
  result = zxImageWriteJPEG( fp, img, quality );
  fclose( fp );
  return result;
}

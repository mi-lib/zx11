#include <zm/zm.h>
#include <zx11/zximage.h>

void output_result(zxWindow *win, zxImage *src, zxImage *dst, zxImage *cpy, const char *effectname)
{
  zxImageDrawAll( win, src, 0, 0 );
  zxImageDrawAll( win, dst, src->width, 0 );
  zxImageDrawAll( win, cpy, src->width*2, 0 );
  zxFlush();
  printf( "%s ... %s", effectname, zxImageCmp( cpy, dst ) ? "OK" : "fail" );
  getchar();
}

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, cpy, dst;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    return 1;

  zxImageAllocDefault( &cpy, src.width, src.height );
  zxImageAllocDefault( &dst, src.width, src.height );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*3, src.height );

  zxImageGrayscalize( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageGrayscalizeDRC( &cpy );
  output_result( &win, &src, &dst, &cpy, "grayscalization" );

  zxImageNegate( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageNegateDRC( &cpy );
  output_result( &win, &src, &dst, &cpy, "negation" );

  zxImageBrighten( &src, &dst, 1.5 );
  zxImageCopy( &src, &cpy );
  zxImageBrightenDRC( &cpy, 1.5 );
  output_result( &win, &src, &dst, &cpy, "brightness" );

  zxImageContrast( &src, &dst, 1.5 );
  zxImageCopy( &src, &cpy );
  zxImageContrastDRC( &cpy, 1.5 );
  output_result( &win, &src, &dst, &cpy, "contrasting" );

  zxImageCorrectGamma( &src, &dst, 2.0 );
  zxImageCopy( &src, &cpy );
  zxImageCorrectGammaDRC( &cpy, 2.0 );
  output_result( &win, &src, &dst, &cpy, "gamma correction" );

  zxImageNormalize( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageNormalizeDRC( &cpy );
  output_result( &win, &src, &dst, &cpy, "normalization" );

  zxImageEqualize( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageEqualizeDRC( &cpy );
  output_result( &win, &src, &dst, &cpy, "equalization" );

  zxImageDitherBayer( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageDitherBayerDRC( &cpy );
  output_result( &win, &src, &dst, &cpy, "dithering (Bayer pattern)" );

  zxImageDestroy( &src );
  zxImageDestroy( &cpy );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

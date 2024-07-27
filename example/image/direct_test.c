#include <zm/zm.h>
#include <zx11/zximage.h>

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
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxImageDrawAll( &win, &cpy, src.width*2, 0 );
  zxFlush();
  printf( "grayscalization ... %s", zxImageCmp( &cpy, &dst ) ? "OK" : "fail" );
  getchar();

  zxImageNegate( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageNegateDRC( &cpy );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxImageDrawAll( &win, &cpy, src.width*2, 0 );
  zxFlush();
  printf( "negation ... %s", zxImageCmp( &cpy, &dst ) ? "OK" : "fail" );
  getchar();

  zxImageToneDown( &src, &dst, 0.5 );
  zxImageCopy( &src, &cpy );
  zxImageToneDownDRC( &cpy, 0.5 );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxImageDrawAll( &win, &cpy, src.width*2, 0 );
  zxFlush();
  printf( "Downtoning ... %s", zxImageCmp( &cpy, &dst ) ? "OK" : "fail" );
  getchar();

  zxImageNormalize( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageNormalizeDRC( &cpy );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxImageDrawAll( &win, &cpy, src.width*2, 0 );
  zxFlush();
  printf( "Normalization ... %s", zxImageCmp( &cpy, &dst ) ? "OK" : "fail" );
  getchar();

  zxImageEqualize( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageEqualizeDRC( &cpy );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxImageDrawAll( &win, &cpy, src.width*2, 0 );
  zxFlush();
  printf( "Equalization ... %s", zxImageCmp( &cpy, &dst ) ? "OK" : "fail" );
  getchar();

  zxImageDither( &src, &dst );
  zxImageCopy( &src, &cpy );
  zxImageDitherDRC( &cpy );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxImageDrawAll( &win, &cpy, src.width*2, 0 );
  zxFlush();
  printf( "Dithering ... %s", zxImageCmp( &cpy, &dst ) ? "OK" : "fail" );
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &cpy );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

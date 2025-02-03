#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dest;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dest, src.width, src.height );

  zxWindowCreateAndOpen( &win, 0, 0, src.width*4, src.height*2 );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDither( &src, &dest, Bayer  ); zxImageDrawAll( &win, &dest, src.width,   0 );
  zxImageDither( &src, &dest, Net    ); zxImageDrawAll( &win, &dest, src.width*2, 0 );
  zxImageDither( &src, &dest, Spiral ); zxImageDrawAll( &win, &dest, src.width*3, 0 );

  zxImageGrayscalizeDRC( &src );
  zxImageDrawAll( &win, &src, 0, src.height );
  zxImageDither( &src, &dest, Bayer  ); zxImageDrawAll( &win, &dest, src.width,   src.height );
  zxImageDither( &src, &dest, Net    ); zxImageDrawAll( &win, &dest, src.width*2, src.height );
  zxImageDither( &src, &dest, Spiral ); zxImageDrawAll( &win, &dest, src.width*3, src.height );

  zxFlush();
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &dest );
  zxExit();
  return 0;
} 

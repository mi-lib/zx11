#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxImage src, dest;
  zxWindow win;

  zxInit();
  zxImageReadFile( &src, argc > 1 ? argv[1] : "fig/lena.jpg" );
  zxWindowCreateRoot( &win );
  zxFlush();
  zxImageAllocDefault( &dest, zxScreenWidth(), zxScreenHeight() );

  zxImageResize( &src, &dest );
  zxImageDrawAll( &win, &dest, 0, 0 );
  zxFlush();

  zxImageDestroy( &src );
  zxImageDestroy( &dest );
  zxClose();
  return 0;
}

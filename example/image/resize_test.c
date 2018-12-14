#include <zx11/zximage.h>

void draw(zxImage *src, double rx, double ry)
{
  zxImage dest;
  zxWindow win;

  zxImageAllocDefault( &dest, src->width*rx, src->height*ry );
  printf( "%d x %d\n", dest.width, dest.height );
  zxWindowCreateAndOpen( &win, 0, 0, dest.width, dest.height );
  while( zxGetEvent() != Expose );
  zxImageResize( src, &dest );
  zxImageDrawAll( &win, &dest, 0, 0 );
  zxImageDestroy( &dest );
  zxFlush();
  getchar();
  zxWindowClose( &win );
}

int main(int argc, char *argv[])
{
  zxImage src;
  zxWindow win;

  zxInit();
  zxImageReadFile( &src, argc > 1 ? argv[1] : "fig/lena.jpg" );
  zxWindowCreateAndOpen( &win, 0, 0, src.width, src.height );
  while( zxGetEvent() != Expose );

  printf( "%d x %d\n", src.width, src.height );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxFlush();
  getchar();

  draw( &src, 0.5, 2.0 );
  draw( &src, 2.0, 0.5 );
  draw( &src, 0.5, 0.5 );
  draw( &src, 2.0, 2.0 );

  zxImageDestroy( &src );
  zxClose();
  return 0;
}

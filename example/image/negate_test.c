#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dest;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dest, 0.7*src.width, 0.7*src.height );
  zxImageNegate( &src, &dest );

  zxWindowCreateAndOpen( &win, 0, 0, src.width, src.height );
  while( zxGetEvent() != Expose );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxFlush();
  getchar();

  zxImageDrawAll( &win, &dest, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &dest );
  zxClose();
  return 0;
} 

#include <zx11/zximage_pnm.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dest;

  zxInit();
  if( zxImageReadPNMFile( &src, argc > 1 ? argv[1] : "fig/lena.ppm" ) == 0 )
    exit( 1 );

  zxWindowCreateAndOpen( &win, 0, 0, src.width, src.height );
  while( zxGetEvent() != Expose );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxFlush();
  getchar();
  zxWindowDestroy( &win );

  if( !zxImageAllocDefault( &dest, src.width, src.height ) )
    exit( 1 );
  zxImageAntialias( &src, &dest );
  zxWindowCreateAndOpen( &win, 0, 0, dest.width, dest.height );
  while( zxGetEvent() != Expose );
  zxImageDrawAll( &win, &dest, 0, 0 );
  zxFlush();
  getchar();
  zxWindowDestroy( &win );
  zxImageDestroy( &dest );

  if( !zxImageAllocDefault( &dest, src.width*0.5, src.height*0.5 ) )
    exit( 1 );
  zxImageResize( &src, &dest );
  zxWindowCreateAndOpen( &win, 0, 0, dest.width, dest.height );
  while( zxGetEvent() != Expose );
  zxImageDrawAll( &win, &dest, 0, 0 );
  zxFlush();
  getchar();
  zxWindowDestroy( &win );
  zxImageDestroy( &dest );

  zxImageDestroy( &src );
  zxClose();
  return 0;
} 

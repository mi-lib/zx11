#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );

  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, 0, 0, cdat.width, cdat.height );
  while( zxGetEvent() != Expose );

  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  zxImageVertFlip( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  zxImageHorizFlip( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &cdat );
  zxWindowDestroy( &win );

  zxImageAllocDefault( &cdat, dat.height, dat.width );
  zxWindowCreateAndOpen( &win, 0, 0, cdat.width, cdat.height );
  while( zxGetEvent() != Expose );

  zxImageRotRight( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  zxImageRotLeft( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &cdat );
  zxWindowDestroy( &win );
  zxImageDestroy( &dat );
  zxClose();
  return 0;
} 

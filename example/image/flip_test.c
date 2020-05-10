#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*3, dat.height+dat.width );
  zxImageDrawAll( &win, &dat, 0, 0 );

  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxImageVertFlip( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width, 0 );
  zxImageHorizFlip( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width*2, 0 );
  zxImageDestroy( &cdat );

  zxImageAllocDefault( &cdat, dat.height, dat.width );
  zxImageRotRight( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, dat.height );
  zxImageRotLeft( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.height, dat.height );

  zxFlush();
  getchar();
  zxImageDestroy( &cdat );
  zxWindowDestroy( &win );
  zxImageDestroy( &dat );
  zxExit();
  return 0;
} 

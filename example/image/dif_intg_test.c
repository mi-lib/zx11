#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*5, dat.height );

  zxImageDrawAll( &win, &dat, 0, 0 );
  zxImageDiff( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width, 0 );
  zxImageDiff( &cdat, &dat );
  zxImageDrawAll( &win, &dat, dat.width*2, 0 );
  zxImageIntegral( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width*3, 0 );
  zxImageIntegral( &cdat, &dat );
  zxImageDrawAll( &win, &dat, dat.width*4, 0 );
  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxImageDestroy( &cdat );
  zxExit();
  return 0;
} 

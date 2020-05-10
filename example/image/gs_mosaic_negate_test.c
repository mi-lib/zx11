#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*4, dat.height );
  zxImageDrawAll( &win, &dat, 0, 0 );

  zxImageGrayscalize( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width, 0 );
  zxImageDestroy( &cdat );

  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxImageNegate( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width*2, 0 );
  zxImageDestroy( &cdat );

  zxImageClone( &dat, &cdat );
  zxImageMosaic( &cdat,
    0.2*cdat.width, 0.2*cdat.height,
    0.6*cdat.width, 0.6*cdat.height, cdat.width/30, cdat.height/30 );
  zxImageDrawAll( &win, &cdat, cdat.width*3, 0 );
  zxImageDestroy( &cdat );

  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxExit();
  return 0;
} 

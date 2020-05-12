#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*4, dat.height*3 );
  zxImageDrawAll( &win, &dat, 0, 0 );

  zxImageGrayscalize( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width, 0 );

  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxImageNegate( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width*2, 0 );

  zxImageCopy( &dat, &cdat );
  zxImageMosaic( &cdat,
    0.2*cdat.width, 0.2*cdat.height,
    0.6*cdat.width, 0.6*cdat.height, cdat.width/30, cdat.height/30 );
  zxImageDrawAll( &win, &cdat, cdat.width*3, 0 );

  zxImageAllocDefault( &cdat, dat.width, dat.height );
  zxImageToneDown( &dat, &cdat, 0.8 );
  zxImageDrawAll( &win, &cdat, 0, dat.height );
  zxImageToneDown( &dat, &cdat, 0.6 );
  zxImageDrawAll( &win, &cdat, dat.width, dat.height );
  zxImageToneDown( &dat, &cdat, 0.4 );
  zxImageDrawAll( &win, &cdat, dat.width*2, dat.height );
  zxImageToneDown( &dat, &cdat, 0.2 );
  zxImageDrawAll( &win, &cdat, dat.width*3, dat.height );

  zxImageDiff( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, dat.height*2 );
  zxImageDiff( &cdat, &dat );
  zxImageDrawAll( &win, &dat, dat.width, dat.height*2 );
  zxImageIntegral( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, dat.width*2, dat.height*2 );
  zxImageIntegral( &cdat, &dat );
  zxImageDrawAll( &win, &dat, dat.width*3, dat.height*2 );

  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxImageDestroy( &cdat );
  zxExit();
  return 0;
} 

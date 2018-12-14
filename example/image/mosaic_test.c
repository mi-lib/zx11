#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  while( zxGetEvent() != Expose );

  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  zxImageMosaic( &dat,
    0.2*dat.width, 0.2*dat.height,
    0.6*dat.width, 0.6*dat.height, dat.width/30, dat.height/30 );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxClose();
  return 0;
} 

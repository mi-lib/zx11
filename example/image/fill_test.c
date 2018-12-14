#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat;
  Pixmap bg;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );

  zxWindowCreateAndOpen( &win, 0, 0, dat.width+100, dat.height+100 );
  bg = zxImageCreatePixmap( &win, &dat );
  zxImageDestroy( &dat );
  while( zxGetEvent() != Expose );
  zxWindowFill( &win, bg );
  zxFlush();
  getchar();

  zxClose();
  return 0;
} 

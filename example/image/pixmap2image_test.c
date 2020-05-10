#include <zx11/zximage_png.h>

int main(void)
{
  zxWindow win;
  zxImage img;

  zxInit();
  zxWindowCreate( &win, 0, 0, 640, 480 );

  zxWindowDoubleBufferEnable( &win );
  zxWindowSetBGColorByName( &win, "blue" );
  zxWindowSetColorByName( &win, "red" );
  zxDrawFillRect( &win, 100, 100, 300, 300 );
  zxWindowSetColorByName( &win, "yellow" );
  zxDrawFillRect( &win, 300, 200, 250, 240 );

  zxImageFromPixmap( &img, zxWindowCanvas(&win), 640, 480 );
  zxImageWritePNGFile( &img, "hoge.png" );
  zxExit();
  return 0;
}

#include <zx11/zximage_png.h>

int main(void)
{
  zxWindow win;
  zxImage img;

  zxInit();
  zxWindowCreate( &win, 0, 0, 640, 480 );

  zxDoubleBufferEnable( &win );
  zxSetBGColor( &win, zxGetColor( &win, "blue" ) );
  zxClear( &win );
  zxSetColor( &win, "red" );
  zxDrawFillRect( &win, 100, 100, 300, 300 );
  zxSetColor( &win, "yellow" );
  zxDrawFillRect( &win, 300, 200, 250, 240 );

  zxImageFromPixmap( &img, zxCanvas(&win), 640, 480 );
  zxImageWritePNGFile( &img, "hoge.png" );

  zxClose();
  return 0;
}

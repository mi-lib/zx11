#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win;

  zxInit();
  zxWindowCreateAndOpen( &win, 100, 100, 500, 500 );
  getchar();

  zxDoubleBufferEnable( &win );
  zxSetBGColor( &win, zxGetColor( &win, "blue" ) );
  zxClear( &win );
  zxSetColor( &win, "red" );
  zxDrawFillRect( &win, 100, 100, 300, 300 );
  zxDoubleBufferAppear( &win );
  zxFlush();
  getchar();
  zxClose();

  return 0;
}

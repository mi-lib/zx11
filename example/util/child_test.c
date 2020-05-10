#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win, cowin;

  zxInit();
  zxWindowCreateAndOpen( &win, 0, 0, 1000, 800 );
  zxWindowSetBGColorByName( &win, "navy" );
  zxWindowCreateAndOpenChild( &cowin, &win, 500, 300, 300, 300 );
  zxWindowSetColorByName( &cowin, "white" );
  zxDrawRect( &cowin, 0, 0, 300, 300 );
  zxWindowSetColorByName( &cowin, "red" );
  zxDrawFillRect( &cowin, 100, 100, 300, 300 );
  zxFlush();
  getchar();
  zxExit();
  return 0;
}

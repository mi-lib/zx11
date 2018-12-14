#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 500, 300 );
  zxWidgetInit( &win );
  zxWindowSetBG( &win, "lightgray" );

  zxwSepHoriz( &win, 20, 20, 460 );
  zxwSepVert(  &win, 20, 30, 260 );
  zxwSepBoxRelief(   &win, 30, 30, 460, 260 );
  zxwSepBoxLower( &win, 40, 40, 440, 240 );
  zxwSepBoxRaise( &win, 80, 80, 360, 160 );

  zxFlush();
  getchar();
  zxClose();
  return 0;
}

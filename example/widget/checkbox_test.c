#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;
  zxwCheckBox b1, b2, b3, b4;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 320, 80 );
  zxWindowSetBG( &win, "lightgray" );
  zxWidgetInit( &win );

  zxwCheckBoxCreate( &b1,  50, 16 );
  zxwCheckBoxCreate( &b2, 100, 16 );
  zxwCheckBoxCreate( &b3, 150, 16 );
  zxwCheckBoxCreate( &b4, 200, 16 );

  zxwPress( &b1 );
  zxwDisable( &b3 );
  zxwPress( &b4 );
  zxwDisable( &b4 );

  zxwCheckBoxDraw( &win, &b1 );
  zxwCheckBoxDraw( &win, &b2 );
  zxwCheckBoxDraw( &win, &b3 );
  zxwCheckBoxDraw( &win, &b4 );
  zxFlush();
  getchar();

  zxClose();
  return 0;
}

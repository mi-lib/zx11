#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;
  zxwRadioButton b1, b2, b3, b4;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 320, 80 );
  zxWidgetInit( &win );

  zxwRadioButtonCreate( &b1,  50, 16 );
  zxwRadioButtonCreate( &b2, 100, 16 );
  zxwRadioButtonCreate( &b3, 150, 16 );
  zxwRadioButtonCreate( &b4, 200, 16 );

  zxwPress( &b1 );
  zxwDisable( &b3 );
  zxwPress( &b4 );
  zxwDisable( &b4 );

  zxwRadioButtonDraw( &win, &b1 );
  zxwRadioButtonDraw( &win, &b2 );
  zxwRadioButtonDraw( &win, &b3 );
  zxwRadioButtonDraw( &win, &b4 );
  zxFlush();
  getchar();

  zxExit();
  return 0;
}

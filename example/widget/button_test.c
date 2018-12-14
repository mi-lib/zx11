#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;
  zxwButton btn[3];

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 320, 80 );
  zxWindowSetBG( &win, "lightgray" );
  zxWidgetInit( &win );

  zxwButtonCreate( &btn[0], 20, 20, 80, 24, "hello" );
  zxwButtonCreate( &btn[1], 120, 20, 80, 24, "this button has a too long label to be completely shown." );
  zxwButtonCreate( &btn[2], 220, 20, 80, 24, "good-bye" );
  btn[2].enable = false;

  zxwButtonDrawRaise( &win, &btn[0] );
  zxwButtonDrawRaise( &win, &btn[1] );
  zxwButtonDrawRaise( &win, &btn[2] );
  zxFlush();
  getchar();
  zxwButtonDrawLower( &win, &btn[0] );
  zxFlush();
  getchar();

  btn[0].enable = false;
  btn[2].enable = true;
  zxwButtonDrawRaise( &win, &btn[0] );
  zxwButtonDrawRaise( &win, &btn[2] );
  zxFlush();
  getchar();
  zxwButtonDrawLower( &win, &btn[2] );
  zxFlush();
  getchar();

  zxwButtonDestroy( &btn[0] );
  zxwButtonDestroy( &btn[1] );
  zxwButtonDestroy( &btn[2] );
  zxClose();

  return 0;
}

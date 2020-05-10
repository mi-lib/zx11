#include <zx11/zxwidget.h>

typedef struct{
  ZXW_BOX_CLASS;
} box_t;

int main(void)
{
  zxWindow win;
  box_t b1, b2, b3, b4;

  zxInit();
  zxWindowCreateAndOpen( &win, 1000, 50, 320, 360 );
  zxWidgetInit( &win );

  zxwBoxSetRegion( &b1, 24,  24, 120, 60 );
  zxwBoxSetRegion( &b2, 24,  96, 120, 60 );
  zxwBoxSetRegion( &b3, 24, 172, 120, 60 );
  zxwBoxSetRegion( &b4, 24, 238, 120, 60 );

  zxwBoxDraw( &win, &b1, zxGetColor(&win,"darkblue") );
  zxwBoxDrawRaise( &win, &b2.reg, zxw_front_color, 2 );
  zxwBoxDrawLower( &win, &b3.reg, zxw_front_color, 2 );
  zxwBoxDrawRelief( &win, &b4.reg, zxw_front_color, 2 );
  zxFlush();
  getchar();

  zxExit();
  return 0;
}

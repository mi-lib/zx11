#include <zx11/zxwidget.h>

void *fA(void){ printf( "selected tab A.\n" ); return NULL; }
void *fB(void){ printf( "selected tab B.\n" ); return NULL; }
void *fC(void){ printf( "selected tab C.\n" ); return NULL; }
void *fD(void){ printf( "selected tab D.\n" ); return NULL; }

int main(void)
{
  zxWindow win;
  zxwTabGroup tg;
  bool judge;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 320, 80 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWidgetInit( &win );

  zxwTabGroupInit( &tg, 20, 20, 280 );
  zxwTabGroupAdd( &tg, "tab A", fA );
  zxwTabGroupAdd( &tg, "tab B", fB );
  zxwTabGroupAdd( &tg, "tab C", fC );
  zxwTabGroupAdd( &tg, "tab D", fD );

  zxwTabGroupDraw( &win, &tg );
  zxFlush();
  while( 1 ){
    switch( zxGetEvent() ){
    case Expose:
      zxwTabGroupDraw( &win, &tg );
      zxFlush();
      break;
    case KeyPress:
      if( zxKeySymbol() == XK_q ) goto END;
    case ButtonPress:
      zxwTabGroupPress( &win, &tg, &judge );
      if( judge ) zxFlush();
      break;
    default: ;
    }
  }
 END:
  zxwTabGroupDestroy( &tg );
  zxExit();
  return 0;
}

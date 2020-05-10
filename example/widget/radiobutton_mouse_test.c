#include <zx11/zxwidget.h>

void draw(zxWindow *win, zxwRadioButtonGroup *bg1, zxwRadioButtonGroup *bg2)
{
  zxwRadioButtonGroupDraw( win, bg1 );
  zxwRadioButtonGroupDraw( win, bg2 );
  zxFlush();
}

int main(void)
{
  zxWindow win;
  zxwRadioButtonGroup bg1, bg2;
  bool judge;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 320, 160 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWidgetInit( &win );

  zxwRadioButtonGroupInit( &bg1, 50, 20, 200, 48 );
  zxwRadioButtonGroupAdd( &bg1,  16, 16 );
  zxwRadioButtonGroupAdd( &bg1,  64, 16 );
  zxwRadioButtonGroupAdd( &bg1, 112, 16 );
  zxwRadioButtonGroupAdd( &bg1, 160, 16 );
  zxwRadioButtonGroupActivate( &bg1, 0 );

  zxwRadioButtonGroupInit( &bg2, 50, 80, 200, 48 );
  zxwRadioButtonGroupAdd( &bg2,  16, 16 );
  zxwRadioButtonGroupAdd( &bg2,  64, 16 );
  zxwRadioButtonGroupAdd( &bg2, 112, 16 );
  zxwRadioButtonGroupAdd( &bg2, 160, 16 );
  zxwRadioButtonGroupActivate( &bg2, 0 );
  zxwGroupSetToggle( &bg2 );

  draw( &win, &bg1, &bg2 );
  while( 1 ){
    switch( zxGetEvent() ){
    case Expose:
      draw( &win, &bg1, &bg2 );
      break;
    case KeyPress:
      if( zxKeySymbol() == XK_q ) goto END;
    case ButtonPress:
      zxwRadioButtonGroupPress( &win, &bg1, &judge );
      if( judge ){
        eprintf( "group#1 selected = %d\n", bg1.item_selected );
        break;
      }
      zxwRadioButtonGroupPress( &win, &bg2, &judge );
      if( judge ){
        eprintf( "group#2 selected = %d\n", bg2.item_selected );
        break;
      }
      break;
    default: ;
    }
  }

 END:
  zxwRadioButtonGroupDestroy( &bg1 );
  zxwRadioButtonGroupDestroy( &bg2 );
  zxExit();
  return 0;
}

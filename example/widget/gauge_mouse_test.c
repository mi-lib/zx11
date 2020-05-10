#include <unistd.h>
#include <zx11/zxwidget.h>

void draw(zxWindow *win, zxwGauge *gauge, zxwEditBox *eb)
{
  static char buf[BUFSIZ];

  zxwGaugeDraw( win, gauge );
  sprintf( buf, "%f", zxwGaugeValue( gauge ) );
  zxwEditBoxSetString( eb, buf );
  zxwEditBoxDraw( win, eb );
  zxFlush();
}

int main(void)
{
  zxWindow win;
  zxwGauge gauge[2];
  zxwEditBox eb[2];

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 400, 100 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWidgetInit( &win );

  zxwGaugeCreate( &gauge[0], 20, 20, 200,  0, -2, 2 );
  zxwGaugeCreate( &gauge[1], 20, 50, 200, 10, -2, 2 );

  zxwEditBoxCreate( &eb[0], 0, 240, 20, 80, 0 );
  zxwAlignRight( &eb[0] );
  zxwAlignFlowRight( &eb[0] );

  zxwEditBoxCreate( &eb[1], 0, 240, 50, 80, 0 );
  zxwAlignRight( &eb[1] );
  zxwAlignFlowRight( &eb[1] );

  draw( &win, &gauge[0], &eb[0] );
  draw( &win, &gauge[1], &eb[1] );
  zxFlush();

  while( 1 ){
    switch( zxGetEvent() ){
    case KeyPress:
      if( zxKeySymbol() == XK_q ) goto END;
    case ButtonPress:
      zxwNobTryGrab( &gauge[0], zxMouseX, zxMouseY );
      zxwNobTryGrab( &gauge[1], zxMouseX, zxMouseY );
      break;
    case ButtonRelease:
      zxwUngrab( &gauge[0] );
      zxwUngrab( &gauge[1] );
      break;
    case MotionNotify:
      if( zxwIsGrabbed(&gauge[0]) ){
        zxwGaugeSetX( &gauge[0], zxMouseX );
        draw( &win, &gauge[0], &eb[0] );
      }
      if( zxwIsGrabbed(&gauge[1]) ){
        zxwGaugeSetX( &gauge[1], zxMouseX );
        draw( &win, &gauge[1], &eb[1] );
      }
      break;
    default: ;
    }
  }

 END:
  zxwEditBoxDestroy( &eb[0] );
  zxwEditBoxDestroy( &eb[1] );
  zxExit();
  return 0;
}

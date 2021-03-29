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
  usleep( 10000 );
}

int main(void)
{
  zxWindow win;
  zxwGauge gauge[2];
  zxwEditBox eb[2];
  double val;

  zxInit();
  zxWindowCreateAndOpen( &win, 1000, 50, 400, 120 );
  zxWidgetInit( &win );

  zxwGaugeCreate( &gauge[0], 20, 20, 200, 0, -2, 2 );
  zxwGaugeCreate( &gauge[1], 20, 50, 200, 0, -2, 2 );
  zxwEditBoxCreate( &eb[0], 0, 240, 20, 80, 0 );
  zxwEditBoxCreate( &eb[1], 0, 240, 50, 80, 0 );
  zxwAlignRight( &eb[0] );
  zxwAlignFlowRight( &eb[0] );
  zxwAlignRight( &eb[1] );
  zxwAlignFlowRight( &eb[1] );
  for( val=-2.2; val<=2.2; val+=0.04 ){
    zxwGaugeSetValue( &gauge[0], val );
    draw( &win, &gauge[0], &eb[0] );
  }
  for( ; val>=-2.2; val-=0.04 ){
    zxwGaugeSetValue( &gauge[0], val );
    draw( &win, &gauge[0], &eb[0] );
  }
  for( ; zxwKnobX(&gauge[1])<=gauge[1].guide.x+gauge[1].x_range-1; zxwKnobX(&gauge[1])+=2 )
    draw( &win, &gauge[1], &eb[1] );
  for( ; zxwKnobX(&gauge[1])>=gauge[1].guide.x; zxwKnobX(&gauge[1])-=2 )
    draw( &win, &gauge[1], &eb[1] );
  zxwGaugeMove( &gauge[0], 20, 80 );
  zxwBoxSetPos( &eb[0], 240, 80 );
  draw( &win, &gauge[0], &eb[0] );

  zxFlush();
  getchar();

  zxwEditBoxDestroy( &eb[0] );
  zxwEditBoxDestroy( &eb[1] );
  zxExit();
  return 0;
}

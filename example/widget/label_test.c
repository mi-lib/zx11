#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;
  zxwLabel label;
  int h;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 200, 240 );
  zxWidgetInit( &win );

  h = zxwLabelHeight( NULL, NULL );

  zxwLabelInit( &label );
  zxwLabelSet( &label, "hello" );
  zxwBoxSetRegion( &label, 0, 0, 100, h );

  zxwBoxSetPos( &label, 50, 40 );
  zxwAlignTop( &label ); zxwAlignLeft( &label );
  zxwLabelDrawColor( &win, &label, "black" );

  zxwBoxSetPos( &label, 50, 40+h );
  zxwAlignTop( &label ); zxwAlignCenter( &label );
  zxwLabelDrawColor( &win, &label, "red" );

  zxwBoxSetPos( &label, 50, 40+h*2 );
  zxwAlignTop( &label ); zxwAlignRight( &label );
  zxwLabelDrawColor( &win, &label, "blue" );

  zxwBoxSetPos( &label, 50, 40+h*3 );
  zxwAlignBottom( &label ); zxwAlignLeft( &label );
  zxwLabelDrawColor( &win, &label, "green" );

  zxwBoxSetPos( &label, 50, 40+h*4 );
  zxwAlignBottom( &label ); zxwAlignCenter( &label );
  zxwLabelDrawColor( &win, &label, "yellow" );

  zxwBoxSetPos( &label, 50, 40+h*5 );
  zxwAlignBottom( &label ); zxwAlignRight( &label );
  zxwLabelDrawColor( &win, &label, "white" );

  zxwBoxSetPos( &label, 50, 40+h*6 );
  zxwAlignMiddle( &label ); zxwAlignCenter( &label );
  zxwLabelDraw( &win, &label );

  zxwLabelDestroy( &label );

  zxFlush();
  getchar();
  zxExit();
  return 0;
}

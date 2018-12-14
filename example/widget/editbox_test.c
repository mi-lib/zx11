#include <unistd.h>
#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;
  zxwEditBox eb;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 400, 100 );
  zxKeyEnable( &win );
  zxWindowSetBG( &win, "lightgray" );
  zxWidgetInit( &win );

  zxwEditBoxCreate( &eb, BUFSIZ, 20, 20, 180, 0 );
  zxwEditBoxDraw( &win, &eb );
  zxFlush();
  while( 1 ){
    if( zxCheckEvent( KeyPress ) ){
      if( zxwEditBoxKeyPress( &win, &eb ) ) break;
      zxFlush();
    }
    usleep( 5000 );
  }
  zxwEditBoxDestroy( &eb );
  zxWindowClose( &win );
  zxClose();
  return 0;
}

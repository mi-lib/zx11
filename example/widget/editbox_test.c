#include <unistd.h>
#include <zx11/zxwidget.h>

int main(void)
{
  zxWindow win;
  zxwEditBox eb;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 400, 100 );
  zxWindowKeyEnable( &win );
  zxWidgetInit( &win );

  zxwEditBoxCreate( &eb, BUFSIZ, 20, 20, 180, 0 );
  zxwCursorVisible( &eb );
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
  zxWindowDestroy( &win );
  zxExit();
  return 0;
}

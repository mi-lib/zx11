#include <unistd.h>
#include <zx11/zxwidget.h>

#define STR1 "Hello world!"
#define STR2 "Nagai Mojiretsu wo Hyouji suru Test nari."

int main(void)
{
  zxWindow win;
  zxwEditBox tb;
  register int n;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 400, 100 );
  zxWindowSetBG( &win, "lightgray" );
  zxWidgetInit( &win );

  zxwEditBoxCreate( &tb, BUFSIZ, 20, 20, 180, 0 );
  strcpy( tb.label, STR1 );

  zxwEditBoxDraw( &win, &tb );
  zxFlush();
  getchar();

  tb.enable = false;
  zxwEditBoxDraw( &win, &tb );
  zxwEditBoxDestroy( &tb );

  zxwEditBoxCreate( &tb, BUFSIZ, 20, 50, 180, 0 );
  for( n=0; n<=strlen(STR2); n++ ){
    strncpy( tb.label, STR2, n );
    tb.str_cur = n;
    zxwEditBoxDraw( &win, &tb );
    zxFlush();
    usleep( 50000 );
  }
  getchar();

  zxwEditBoxFit( &win, &tb );
  zxwCursorInit( &tb );
  zxwEditBoxDraw( &win, &tb );
  zxwEditBoxDestroy( &tb );
  zxFlush();
  getchar();
  zxClose();

  return 0;
}

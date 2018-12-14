#include <zx11/zxinput.h>

zxWindow win;

void draw(void)
{
  Status status;
  KeySym keysym;
  wchar_t wbuf[BUFSIZ], str[BUFSIZ];
  int cnt, len, i;
  static int x = 0, y = 0;
  zxRegion ta;

  cnt = zxICGetString( wbuf, sizeof(wbuf), &keysym, &status );
  printf( "keysym=%X, status=%d\n", (uint)keysym, status );

  if( status == XLookupChars || status == XLookupBoth ){
    switch( keysym ){
    case XK_Return: case 0x6D: /* newline */
      if( ( y += zxFontHeight() ) >= zxWindowHeight(&win) )
        y = 0;
      x = 0;
      zxWindowClearArea( &win, 0, y, zxWindowWidth(&win), zxFontHeight() );
      break;
    case XK_BackSpace: case 0x68:/* backspace */
      if( x > 0 ) x -= zxFontWidth();
      zxWindowClearArea( &win, x, y, zxFontWidth(), zxFontHeight() );
      break;
    default: ;
      for( len=0, i=0; i<cnt; i++ )
        if( iswprint( wbuf[i] ) ) str[len++] = wbuf[i];
      str[len] = '\0';
      zxDrawNStringWC( &win, x, y + zxFontAscent(), str, len );
      zxTextAreaWC( str, &ta );
      x += ta.width;
    }
  }
}

void loop(void)
{
  while( 1 ){
    zxNextEvent();
    if( XFilterEvent( &zxevent, None ) ) continue;
    switch( zxevent.type ) {
      case KeyPress: draw();        break;
      case FocusIn:  zxICFocus();   break;
      case FocusOut: zxICUnfocus(); break;
      case ClientMessage:
        if( zxDeleteWindowEvent() ) return;
        break;
    }
  }
}

int main(void)
{
  if( zxSetLocale() == -1 ) return 1;
  zxInit();
  zxFontSetCreate( "-*-fixed-medium-r-normal--14-*-*-*" );
  zxWindowCreate( &win, 0, 0, zxFontWidth()*30, zxFontHeight()*20 );
  zxWindowSetFG( &win, "black" );
  zxWindowSetBG( &win, "white" );

  zxDeleteWindowEnable( &win );
  zxKeyEnable( &win );

  if( zxIMInit(&win) == -1 ) return 1;
  zxWindowOpen( &win );

  loop();

  zxIMClose();
  zxWindowDestroy( &win );
  zxClose();
  return 0;
}

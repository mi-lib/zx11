#include <zx11/zxinput.h>

void draw(zxWindow *win)
{
  Status status;
  KeySym keysym;
  wchar_t wbuf[BUFSIZ], str[BUFSIZ];
  int cnt, len, i;
  static int x = 0, y = 0;
  zxRegion ta;

  cnt = zxWindowICGetString( win, wbuf, sizeof(wbuf), &keysym, &status );
  printf( "keysym=%X, status=%d\n", (uint)keysym, status );

  if( status == XLookupChars || status == XLookupBoth ){
    switch( keysym ){
    case XK_Return: case 0x6D: /* newline */
      if( ( y += zxFontHeight() ) >= zxWindowHeight(win) )
        y = 0;
      x = 0;
      zxWindowClearArea( win, 0, y, zxWindowWidth(win), zxFontHeight() );
      break;
    case XK_BackSpace: case 0x68:/* backspace */
      if( x > 0 ) x -= zxFontWidth();
      zxWindowClearArea( win, x, y, zxFontWidth(), zxFontHeight() );
      break;
    default: ;
      for( len=0, i=0; i<cnt; i++ )
        if( iswprint( wbuf[i] ) ) str[len++] = wbuf[i];
      str[len] = '\0';
      zxDrawNStringWC( win, x, y + zxFontAscent(), str, len );
      zxTextAreaWC( str, &ta );
      x += ta.width;
    }
  }
}

void loop(zxWindow *win)
{
  while( 1 ){
    zxNextEvent();
    if( XFilterEvent( &zxevent, None ) ) continue;
    switch( zxevent.type ) {
      case KeyPress: draw( win );                 break;
      case FocusIn:  zxWindowSetICFocus( win );   break;
      case FocusOut: zxWindowUnsetICFocus( win ); break;
      case ClientMessage:
        if( zxWindowIsReceivedDeleteMsg( win ) ) return;
        break;
    }
  }
}

int main(void)
{
  zxWindow win;

  if( zxSetLocale() == -1 ) return 1;
  zxInit();
  zxFontSetCreate( "-*-fixed-medium-r-normal--14-*-*-*" );
  zxWindowCreate( &win, 0, 0, zxFontWidth()*30, zxFontHeight()*20 );
  zxWindowSetColorByName( &win, "black" );
  zxWindowSetBGColorByName( &win, "white" );
  zxWindowKeyEnable( &win );
  if( zxIMInit() == -1 ) return 1;
  if( zxWindowIMEnable(&win) == -1 ) return 1;
  zxWindowOpen( &win );
  loop( &win );
  zxIMExit();
  zxWindowDestroy( &win );
  zxExit();
  return 0;
}

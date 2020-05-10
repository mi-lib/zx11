#include <unistd.h>
#include <zx11/zxinput.h>

static char fontname[] =
"-misc-fixed-*-r-normal--14-130-75-75-c-70-iso8859-1";

int main(int argc, char *argv[])
{
  zxWindow win;
  KeySym key;
  bool quit_flag = false;
  byte modkey_prev = ZX_MODKEY_NONE;
  char text[BUFSIZ], string[BUFSIZ];
  int p = -1;

  zxInit();
  zxWindowCreateAndOpen( &win, 100, 100, 500, 300 );
  zxWindowSetBGColorByName( &win, "navy" );
  zxWindowKeyEnable( &win );
  zxWindowSetColorByName( &win, "white" );
  zxWindowSetFont( &win, fontname );
  while( !quit_flag ){
    zxFlush();
    switch( zxDequeueEvent() ){
    case KeyPress:
      zxKeySymbolAndString( string, sizeof(string), &key );
      switch( key ){
      case XK_Return: quit_flag = true; break;
      case XK_BackSpace: if( p >= 0 ) p--; break;
      case XK_Shift_R:
      case XK_Shift_L: break;
      default:
        if( !IsCursorKey( key ) && !IsFunctionKey( key ) )
          text[++p] = string[0];
      }
      zxWindowClear( &win );
      text[p+1] = '\0';
      zxDrawString( &win, 10, 40, text );
      zxFlush();
      if( !zxModkeyOn( zxKeySymbol() ) )
        printf( "#%X key hit!\n", zxKeyCode() );
      break;
    case KeyRelease:
      if( !zxModkeyOff( ( key = zxKeySymbol() ) ) )
        printf( "#%X:%lX key released!\n", zxKeyCode(), key );
      switch( key ){
      case XK_q: case XK_Q: case XK_Escape:
        quit_flag = true;
        break;
      }
    }
    if( zxmodkey != modkey_prev ){
      printf( "Ctrl=%s, Shift=%s, Alt=%s\n",
        zxModkeyCtrlIsOn() ? "on " : "off",
        zxModkeyShiftIsOn() ? "on " : "off",
        zxModkeyAltIsOn() ? "on " : "off" );
      modkey_prev = zxmodkey;
    }
    usleep( 500 );
  }
  zxWindowDestroy( &win );
  zxExit();
  return 0;
} 

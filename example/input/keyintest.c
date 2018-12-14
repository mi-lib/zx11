#include <zx11/zxinput.h>

static char fontname[] =
"-misc-fixed-*-r-normal--14-130-75-75-c-70-iso8859-1";

int main(void)
{
  zxWindow win;
  KeySym key;
  char text[BUFSIZ], string[BUFSIZ];
  int p = 0, isend = False;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 300, 100 );
  zxSetFGColor( &win, zxGetColor( &win, "white" ) );
  zxWindowSetBG( &win, "navy" );
  zxSetFont( &win, fontname );

  zxKeyEnable( &win );

  while( 1 ){
    if( zxNextEvent() == KeyPress ){
      zxKeySymbolAndString( string, sizeof(string), &key );
      switch( key ){
      case XK_Return: isend = True; break;
      case XK_BackSpace: p--; break;
      case XK_Shift_R:
      case XK_Shift_L: break;
      default:
        if( !IsCursorKey( key ) && !IsFunctionKey( key ) )
          text[p++] = string[0];
      }
      zxWindowClear( &win );
      text[p+1] = '\0';
      zxDrawString( &win, 10, 40, text );
    }
    if( isend ) break;
  }
  zxWindowDestroy( &win );
  zxClose();
  return 0;
}

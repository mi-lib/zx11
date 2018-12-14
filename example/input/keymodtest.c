#include <unistd.h>
#include <zx11/zxinput.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  KeySym key;
  bool quit_flag = false;

  zxInit();
  zxWindowCreateAndOpen( &win, 100, 100, 500, 300 );
  zxKeyEnable( &win );
  while( !quit_flag ){
    zxFlush();
    switch( zxDequeueEvent() ){
    case KeyPress:
      if( !zxModkeyOn( zxKeySymbol() ) )
        printf( "#%X key hit!\n", zxevent.xkey.keycode );
      break;
    case KeyRelease:
      if( !zxModkeyOff( ( key = zxKeySymbol() ) ) )
        printf( "#%X:%lX key released!\n", zxevent.xkey.keycode, key );
      switch( key ){
      case XK_q: case XK_Q: case XK_Escape:
        quit_flag = true;
        break;
      }
    }
    printf( "Ctrl=%s, Shift=%s, Alt=%s\n",
      zxModkeyCtrlIsOn() ? "on " : "off",
      zxModkeyShiftIsOn() ? "on " : "off",
      zxModkeyAltIsOn() ? "on " : "off" );
    usleep( 500 );
  }
  zxWindowClose( &win );
  zxClose();
  return 0;
} 

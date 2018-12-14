#include <unistd.h>
#include <zx11/zxinput.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  bool quit_flag = false;

  zxInit();
  zxWindowCreateAndOpen( &win, 100, 100, 500, 300 );
  zxKeyEnable( &win );
  while( !quit_flag ){
    zxFlush();
    switch( zxDequeueEvent() ){
    case KeyPress:
      printf( "#%X key hit!\n", zxevent.xkey.keycode );
      break;
    case KeyRelease:
      printf( "#%X key released!\n", zxevent.xkey.keycode );
      switch( zxKeySymbol() ){
      case XK_q: case XK_Q: case XK_Escape:
        quit_flag = true;
        break;
      }
      break;
    }
    usleep( 500 );
  }
  zxWindowClose( &win );
  zxClose();
  return 0;
} 

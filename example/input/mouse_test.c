#include <unistd.h>
#include <zx11/zxinput.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  bool quit_flag = false;

  zxInit();
  zxWindowCreateAndOpen( &win, 100, 100, 500, 300 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  while( !quit_flag ){
    zxFlush();
    switch( zxGetEvent() ){
    case KeyPress:
      printf( "key pressed!\n" );
      break;
    case KeyRelease:
      printf( "key released!\n" );
      if( zxKeySymbol() == XK_q ) quit_flag = true;
      break;
    case ButtonPress:
      printf( "mouse clicked at (%d,%d)\n", zxMouseX, zxMouseY );
      break;
    case ButtonRelease:
      printf( "mouse released at (%d,%d)\n", zxMouseX, zxMouseY );
      break;
    case MotionNotify:
      printf( "mouse moved to (%d,%d)\n", zxMouseX, zxMouseY );
      break;
    default:
      usleep( 500 );
    }
  }
  zxWindowDestroy( &win );
  zxExit();
  return 0;
} 

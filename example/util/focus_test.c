#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win;
  Window focused, oldfocused;
  zxRegion reg;
  int count = 0;

  zxInit();
  zxWindowCreate( &win, 100, 100, 50, 50 );
  oldfocused = zxrootwindow;
  while( count < 10 ){
    focused = zxGetInputFocus();
    if( focused != oldfocused && focused != zxWindowBody(&win) ){
      zxSync();
      if( zxGetGeometry( focused, &reg ) ){
        printf( "<%d> %ld/%ld: %dx%d+%d+%d\n", count++, focused, zxWindowBody(&win), reg.width, reg.height, reg.x, reg.y );
        if( reg.x >= 0 && reg.y >= 0 ){
          zxWindowClose( &win );
          zxWindowSetPos( &win, reg.x, reg.y );
          zxWindowOpen( &win );
          oldfocused = focused;
        }
      }
    }
  }
  zxExit();
  return 0;
}

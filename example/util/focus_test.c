#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win;
  Window focused, oldfocused;
  zxRegion reg;

  zxInit();
  zxWindowCreate( &win, 100, 100, 50, 50 );
  oldfocused = zxrootwindow;
  while( 1 ){
    focused = zxGetInputFocus();
    if( focused != oldfocused && focused != zxWindowBody(&win) ){
      zxSync();
      if( zxGetGeometry( focused, &reg ) ){
        printf( "%dx%d+%d+%d\n", reg.width, reg.height, reg.x, reg.y );
        if( reg.x >= 0 && reg.y >= 0 ){
          zxWindowClose( &win );
          zxWindowSetPos( &win, reg.x, reg.y );
          zxWindowOpen( &win );
          oldfocused = focused;
        }
      }
    }
  }
  return 0;
}

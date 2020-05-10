#include <unistd.h>
#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win;
  register int w=150, h=100, x=8, y=20;

  zxInit();
  zxWindowCreate( &win, x, y, w, h );
  zxWindowOverrideRedirectEnable( &win );
  zxWindowMove( &win, x, y );
  zxWindowOpen( &win );
  for( ; w<=300; w+=3, h+=2 ){
    zxWindowResize( &win, w, h );
    usleep( 10000 );
    zxFlush();
  }
  for( ; x<=100; x++, y++ ){
    zxWindowMove( &win, x, y );
    usleep( 10000 );
    zxFlush();
  }
  getchar();
  return 0;
}

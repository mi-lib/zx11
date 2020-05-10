#include <zx11/zxwidget.h>
#include <unistd.h>

int main(void)
{
  zxWindow win;
  zxwProgressBar pb;
  register int i;

  zRandInit();
  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 320, 120 );
  zxWidgetInit( &win );

  zxwProgressBarInit( &pb, 40, 40, 240 );
  for( i=-10; i<=110; i++ ){
    zxwProgressBarSetDegree( &pb, i );
    zxwProgressBarDraw( &win, &pb );
    zxFlush();
    usleep( 10000 * zRandI(1,10) );
  }
  getchar();
  zxExit();
  return 0;
}

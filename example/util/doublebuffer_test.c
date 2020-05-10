#include <liw/liw_debug.h>
#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win;
  long cl[2];

  zxInit();
  zxWindowCreateAndOpen( &win, 1000, 100, 500, 500 );
  zxWindowDoubleBufferEnable( &win );
  MEASURE_EXEC_CLOCK( &cl[0],
    zxWindowSetBGColorByName( &win, "blue" );
    zxWindowClear( &win );
    zxWindowSetColorByName( &win, "red" );
    zxDrawFillRect( &win, 100, 100, 300, 300 );
  );
  printf( "Hit enter key to expose a picture." );
  getchar();
  MEASURE_EXEC_CLOCK( &cl[1],
    zxWindowDoubleBufferAppear( &win );
    zxFlush();
  );
  printf( "clock for processes:\n pixmap->%ld window->%ld\n", cl[0], cl[1] );
  getchar();
  zxExit();
  return 0;
}

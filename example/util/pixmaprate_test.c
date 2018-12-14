#include <liw/liw_debug.h>
#include <zx11/zxutil.h>

int main(void)
{
  zxWindow win;
  long cl[2];

  zxInit();
  zxWindowCreateAndOpen( &win, 100, 100, 500, 500 );

  zxDoubleBufferEnable( &win );
  MEASURE_EXEC_CLOCK( &cl[0],
    zxSetBGColor( &win, zxGetColor( &win, "blue" ) );
    zxClear( &win );
    zxSetColor( &win, "red" );
    zxDrawFillRect( &win, 100, 100, 300, 300 );
  );
  MEASURE_EXEC_CLOCK( &cl[1],
    zxDoubleBufferAppear( &win );
    zxFlush();
  );
  printf( "pixmap->%ld window->%ld\n", cl[0], cl[1] );
  zxClose();

  return 0;
}

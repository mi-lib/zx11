#include <zx11/zxfont.h>

static char fontname[] =
"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso8859-1";

int main(void)
{
  register int i;
  zxWindow win;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 500, 500 );
  zxSetFGColor( &win, zxGetColor( &win, "white" ) );
  zxWindowSetBG( &win, "navy" );

  zxSetFont( &win, fontname );
  for( i=0; i<10; i++ )
    zxDrawString( &win, 100, 100+i*16, "A big black bug bit a big black bear." );
  zxFlush();
  getchar();
  zxClose();

  return 0;
}

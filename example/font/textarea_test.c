#include <zx11/zxfont.h>

static char fontname[] =
"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso8859-1";

#define TEXT1 "A big black bug bit a big black bear."
#define TEXT2 "ascwvn"

int main(void)
{
  zxWindow win;
  zxRegion reg;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 500, 150 );
  zxWindowSetBGColorByName( &win, "navy" );

  zxWindowSetFont( &win, fontname );

  zxWindowSetColorByName( &win, "white" );
  zxDrawString( &win, 100, 60, TEXT1 );
  zxTextArea( TEXT1, 100, 60, &reg );
  zxWindowSetColorByName( &win, "red" );
  zxDrawRegionRect( &win, &reg );

  zxWindowSetColorByName( &win, "white" );
  zxDrawString( &win, 100, 100, TEXT2 );
  zxTextArea( TEXT2, 100, 100, &reg );
  zxWindowSetColorByName( &win, "red" );
  zxDrawRegionRect( &win, &reg );

  zxFlush();
  getchar();
  zxExit();
  return 0;
}

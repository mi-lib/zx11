#include <zx11/zxfont.h>

static char fontname1[] =
"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso8859-1";

static char fontname2[] =
"-adobe-helvetica-medium-r-normal--14-140-75-75-p-77-iso8859-1";

static const char *str = "A big black bug bit a big black bear.";

int main(int argc, char *argv[])
{
  zxWindow win;
  char *c;
  XCharStruct *cs;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 500, 500 );
  zxSetFGColor( &win, zxGetColor( &win, "white" ) );
  zxWindowSetBG( &win, "navy" );

  zxSetFont( &win, argc == 1 ? fontname1 : fontname2 );
  zxDrawString( &win, 100, 100, str );
  for( c=(char *)str; *c; c++ ){
    if( !( cs = zxCharStruct( *c ) ) ) continue;
    printf( "%c: %dx(%d+%d)\n", *c, cs->width, cs->ascent, cs->descent );
  }
  zxFlush();
  getchar();
  zxClose();

  return 0;
}

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
  register int i;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 500, 500 );
  zxWindowSetBGColorByName( &win, "navy" );

  zxWindowSetColorByName( &win, "white" );
  zxWindowSetFont( &win, argc == 1 ? fontname1 : fontname2 );
  for( i=0; i<10; i++ )
    zxDrawString( &win, 100, 100+i*16, str );
  for( c=(char *)str; *c; c++ ){
    if( !( cs = zxCharStruct( *c ) ) ) continue;
    printf( "%c: %dx(%d+%d)\n", *c, cs->width, cs->ascent, cs->descent );
  }
  zxFlush();
  getchar();
  zxExit();
  return 0;
}

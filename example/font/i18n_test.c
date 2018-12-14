#include <zx11/zxfont.h>

static char *fontname[] = {
  "-misc-fixed-medium-r-normal--14-*-*-*-*-*-*-*",
  "-misc-mincho-*-*-normal--14-*-*-*-*-*-*-*",
  "-shinonome-mincho-*-*-*--*-*-*-*-*-*-*-*",
  "-nagamin-*-*-*-*--17-*-*-*-*-*-*-*",
  "-nagamaru-*-*-*-*--17-*-*-*-*-*-*-*",
  "-ayu-*-*-*-*--*-*-*-*-*-*-*-*",
  "-mnkaname-*-*-*-*--*-*-*-*-*-*-*-*",
  "-kappa-mincho-*-*-*--*-*-*-*-*-*-*-*",
  "-jis-*-*-*-*--*-*-*-*-*-*-*-*",
  "-sony-*-*-*-*--*-*-*-*-*-*-*-*",
};

int main (int argc, char* argv [])
{
  zxWindow win;
  const char *text = "日本語表示テスト";
  zxRegion area;
  int i, n;

  zxInit();
  if( zxSetLocale() != 0 ) exit( 1 );
  n = sizeof(fontname)/sizeof(char*);
  if( ( i = argc > 1 ? atoi( argv[1] ) : 0 ) >= n )
    i = 0;
  printf( "ID : %d/%d\n", i, n );
  if( !zxFontSetCreate( fontname[i] ) ) exit( 1 );

  printf( "text       : %s\n", text );
  printf( "font name  : %s\n", zxFontName() );
  printf( "locale name: %s\n", zxLocaleName() );

  zxTextAreaMB( text, &area );
  zxWindowCreateAndOpen( &win, 0, 0, area.width, area.height );
  zxWindowSetFG( &win, "black" );
  zxWindowSetBG( &win, "white" );
  zxWindowClear( &win );

  zxDrawStringMB( &win, area.x, area.y, text );
  zxFlush();
  getchar();

  return 0;
}

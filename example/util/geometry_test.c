#include <zx11/zxutil.h>

void parse(char *str)
{
  zxRegion reg;
  uint mask;
  int x, y;
  uint w, h;

  printf( "\"%s\" -> ", str );
  fflush( stdout );
  if( !zxParseGeometry( str, &reg ) )
    return;
  printf( "%dx%d+%d+%d\n", reg.width, reg.height, reg.x, reg.y );
  mask = XParseGeometry( str, &x, &y, &w, &h );
  printf( "%dx%d+%d+%d\n", w, h, x, y );
  printf( "mask = 0x%X\n", mask );
}

int main(void)
{
  zxInit();

  parse( "300" );
  parse( "300x" );
  parse( "300x500" );
  parse( "300x500+" );
  parse( "300x500+10" );
  parse( "300x500+10+" );
  parse( "300x500+10+20" );
  parse( "300x500-" );
  parse( "300x500-10" );
  parse( "300x500-10-" );
  parse( "300x500-10-20" );
  parse( "+" );
  parse( "+10" );
  parse( "+10+" );
  parse( "+10+20" );
  parse( "-10-20" );
  parse( "hogehogehoge" );
  return 0;
}

#include <zx11/zxutil.h>

bool parse_test_valid(char *str, uint wa, uint ha, int xa, int ya)
{
  zxRegion reg;

  if( !zxParseGeometry( str, &reg ) ) return false;
#ifdef TEST
  printf( "\"%s\" -> %dx%d+%d+%d\n", str, reg.width, reg.height, reg.x, reg.y );
#endif
  return reg.x == xa && reg.y == ya && reg.width == wa && reg.height == ha;
}

bool parse_test_invalid(char *str)
{
  zxRegion reg;

  if( zxParseGeometry( str, &reg ) ) return false;
#ifdef TEST
  printf( "\"%s\" -> %dx%d+%d+%d\n", str, reg.width, reg.height, reg.x, reg.y );
#endif
  return true;
}

void assert_parse(void)
{
  zAssert( zxParseGeometry (valid cases),
    parse_test_valid( "300x", 300, 0, 0, 0 ) &&
    parse_test_valid( "300x500", 300, 500, 0, 0 ) &&
    parse_test_valid( "300x500+10", 300, 500, 10, 0 ) &&
    parse_test_valid( "300x500+10+20", 300, 500, 10, 20 ) &&
    parse_test_valid( "300x500-10", 300, 500, -10, 0 ) &&
    parse_test_valid( "300x500-10-20", 300, 500, -10, -20 ) &&
    parse_test_valid( "x500", 0, 500, 0, 0 ) &&
    parse_test_valid( "x500+10", 0, 500, 10, 0 ) &&
    parse_test_valid( "x500+10+20", 0, 500, 10, 20 ) &&
    parse_test_valid( "300x+10", 300, 10, 0, 0 ) &&
    parse_test_valid( "+10", 0, 0, 10, 0 ) &&
    parse_test_valid( "+10+20", 0, 0, 10, 20 ) &&
    parse_test_valid( "-10-20", 0, 0, -10, -20 ) );
  zAssert( zxParseGeometry (invalid cases),
    parse_test_invalid( "300" ) &&
    parse_test_invalid( "300x500+" ) &&
    parse_test_invalid( "300x500+10+" ) &&
    parse_test_invalid( "300x500x10" ) &&
    parse_test_invalid( "300x500-" ) &&
    parse_test_invalid( "300x500-10-" ) &&
    parse_test_invalid( "+" ) &&
    parse_test_invalid( "x" ) &&
    parse_test_invalid( "x+" ) &&
    parse_test_invalid( "x10+" ) &&
    parse_test_invalid( "+10+" ) &&
    parse_test_invalid( "foo" ) );
}

int main(void)
{
  assert_parse();
  return 0;
}

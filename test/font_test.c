#include <zx11/zxfont.h>

void assert_set_unset(void)
{
  const char *fontname = "-*-*-*-*-*-*-*-*-*-*-*-*-*-*";
  bool result = true;

  zxFontSet( fontname );
  if( !zxfontstruct ) result = false;
  zxFontUnset();
  zxFontSet( fontname );
  if( !zxfontstruct ) result = false;
  zxFontUnset();
  zxFontSet( NULL );
  if( !zxfontstruct ) result = false;
  zxFontUnset();
  zAssert( zxFontSet + zxFontUnset, result );
}

int main(void)
{
  zxInit();
  assert_set_unset();
  zxExit();
  return 0;
}

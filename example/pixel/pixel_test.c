#include <zx11/zximage.h>

void test(int bit)
{
  zxPixelManip pm;
  zxPixel pixel;
  ubyte r, g, b;

  zxPixelManipSet( &pm, bit );
  pixel = pm.PixelFromRGB(0xa,0xb,0xc);
  pm.PixelRGB( pixel, &r, &g, &b );
  printf( "(%d-bit test)\n", bit );
  printf( "original pixel = %lX\n", pixel );
  printf( "get pixel = %X : %X : %X\n", r, g, b );
}

int main(void)
{
  test( 16 );
  test( 24 );
  test( 32 );
  return 0;
}

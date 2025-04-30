#include <zx11/zximage.h>

bool pixel_test(int bit)
{
  zxPixelManip *pm;
  zxPixel pixel;
  ubyte r, g, b, rc, gc, bc;

  r = 0xa;
  g = 0xb;
  b = 0xc;
  pm = zxPixelManipFind( bit );
  pixel = pm->PixelFromRGB( r, g, b );
  pm->PixelRGB( pixel, &rc, &gc, &bc );
  return rc == r && gc == g && bc == b;
}

void assert_pixelmanip(void)
{
  zAssert( zxPixelManip (16bit), pixel_test( 16 ) );
  zAssert( zxPixelManip (24bit), pixel_test( 24 ) );
  zAssert( zxPixelManip (32bit), pixel_test( 32 ) );
}

void assert_screenpixel(void)
{
  zxPixel pixel;
  ubyte r, g, b, rc, gc, bc;
  zxImage dat;
  zxPixelManip *pm;

  r = 0x11;
  g = 0x22;
  b = 0x33;
  pm = zxPixelManipFind( zxdepth );
  pixel = pm->PixelFromRGB( r, g, b );
  /* copy to image structure */
  zxImageAllocDefault( &dat, 1, 1 );
  zxImageCellFromPixel( &dat, 0, 0, pixel );
  zAssert( zxImageCellFromPixel, pixel == zxImageCellPixel(&dat,0,0) );

  zxImageCellRGB( &dat, 0, 0, &rc, &gc, &bc );
  zAssert( zxImageCellRGB, rc == r && gc == g && bc == b );
  zxImageDestroy( &dat );
}

int main(void)
{
  zxInit();
  assert_pixelmanip();
  assert_screenpixel();
  zxExit();
  return 0;
}

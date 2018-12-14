#include <zx11/zximage.h>

int main(void)
{
  zxPixel pixel;
  ubyte r, g, b;
  zxImage dat;
  zxPixelManip pm;

  zxInit();
  printf( "screen depth:%d\n", zxdepth );
  zxPixelManipSet( &pm, zxdepth );

  pixel = pm.PixelFromRGB(0x11,0x22,0x33);
  printf( "original pixel:%lX\n", pixel );

  /* copy to image structure */
  zxImageAllocDefault( &dat, 1, 1 );
  zxImageCellFromPixel( &dat, 0, 0, pixel );
  printf( "original pixel:%lX\n", pixel );
  printf( "%lX\n", zxImageCellPixel(&dat,0,0) );
  zxImageCellRGB( &dat, &pm, 0, 0, &r, &g, &b );
  printf( "(abstract)\n" );
  printf( "R:%X G:%X B:%X\n", r, g, b );
  zxImageDestroy( &dat );

  return 0;
}

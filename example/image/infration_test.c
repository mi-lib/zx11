#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zBitmap src, dest1, dest2;
  zxImage img;

  zxInit();
  zxImageAllocDefault( &img, 160, 120 );
  zxImagePutRect( &img,  30,  15,  20,  90, 0xff, 0xff, 0xff );
  zxImagePutRect( &img,  70,  15,  20,  60, 0xff, 0xff, 0xff );
  zxImagePutRect( &img, 110,  15,  20,  60, 0xff, 0xff, 0xff );
  zxImagePutRect( &img,  10,  30, 140,  15, 0xff, 0xff, 0xff );
  zxImagePutRect( &img,  70,  60,  60,  15, 0xff, 0xff, 0xff );
  zxImagePutRect( &img,  30,  90, 100,  15, 0xff, 0xff, 0xff );
  zBitmapAlloc( &src, img.width, img.height );
  zBitmapAlloc( &dest1, img.width, img.height );
  zBitmapAlloc( &dest2, img.width, img.height );
  zxImageToBitmap( &img, &src, 0x7f );

  zBitmapInfrate( &src, &dest1, 3 );
  zBitmapDefrate( &src, &dest2, 3 );

  zxWindowCreateAndOpen( &win, 0, 0, img.width*3, img.height );
  zBitmapToImage( &src, &img );
  zxImageDrawAll( &win, &img, 0, 0 );
  zBitmapToImage( &dest1, &img );
  zxImageDrawAll( &win, &img, img.width, 0 );
  zBitmapToImage( &dest2, &img );
  zxImageDrawAll( &win, &img, img.width*2, 0 );
  zxFlush();
  getchar();
  zxImageDestroy( &img );
  zBitmapDestroy( &src );
  zBitmapDestroy( &dest1 );
  zBitmapDestroy( &dest2 );
  zxExit();
  return 0;
}

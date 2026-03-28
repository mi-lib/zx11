#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dest1, dest2;
  zBitmap bitmap;

  zxInit();
  zxImageReadFile( &src, "../fig/lena_mini.jpg" );
  zxImageAllocDefault( &dest1, src.width, src.height );
  zxImageAllocDefault( &dest2, src.width, src.height );
  zBitmapAlloc( &bitmap, src.width, src.height );
  zxImageBinarize( &src, &dest1, 0x7f );
  zxImageToBitmap( &src, &bitmap, 0x7f );
  zBitmapToImage( &bitmap, &dest2 );

  zxWindowCreateAndOpen( &win, 0, 0, src.width*3, src.height );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dest1, src.width, 0 );
  zxImageDrawAll( &win, &dest2, src.width*2, 0 );
  zxFlush();
  getchar();
  zxImageDestroy( &src );
  zxImageDestroy( &dest1 );
  zxImageDestroy( &dest2 );
  zBitmapDestroy( &bitmap );
  zxExit();
  return 0;
}

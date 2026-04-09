#include <zx11/zximage.h>

#define FONTFILE "/usr/share/fonts/truetype/vlgothic/VL-Gothic-Regular.ttf"

int main(int argc, char *argv[])
{
  zxWindow win;
  zBitmap src, dest;
  zxImage img;
  zxFTImage font;

  if( !zxFTImageInitLibrary() ) return 1;
  if( !zxFTImageLoadFont( &font, FONTFILE, 48, 100 ) ) return 1;

  zxInit();
  zxImageAllocDefault( &img, 160, 120 );
  zBitmapAlloc( &src, img.width, img.height );
  zBitmapAlloc( &dest, img.width, img.height );

  zxWindowCreateAndOpen( &win, 0, 0, img.width*2, img.height*2 );

  zxImageDrawString( &img, &font, 10, 40, u"龍虎", 1.0, 1.0, 1.0 );
  zxImageToBitmap( &img, &src, 0x7f );
  zBitmapSkeletonize( &src, &dest );
  zBitmapToImage( &src, &img );
  zxImageDrawAll( &win, &img, 0, 0 );
  zBitmapToImage( &dest, &img );
  zxImageDrawAll( &win, &img, 0, img.height );

  zxImageFill( &img, 0 );
  zxImagePutRect( &img,   0,  40, 160,  40, 0xff, 0xff, 0xff );
  zxImagePutRect( &img,  60,   0,  40, 120, 0xff, 0xff, 0xff );
  zxImageToBitmap( &img, &src, 0x7f );
  zBitmapSkeletonize( &src, &dest );
  zBitmapToImage( &src, &img );
  zxImageDrawAll( &win, &img, img.width, 0 );
  zBitmapToImage( &dest, &img );
  zxImageDrawAll( &win, &img, img.width, img.height );

  zxFlush();
  getchar();
  zxImageDestroy( &img );
  zBitmapDestroy( &src );
  zBitmapDestroy( &dest );
  zxFTImageUnloadFont( &font );
  zxFTImageExitLibrary();
  zxExit();
  return 0;
}

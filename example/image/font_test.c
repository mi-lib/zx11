#include <zx11/zximage.h>

#define WIDTH   640
#define HEIGHT  480

#define FONTFILE "/usr/share/fonts/truetype/ipafont-nonfree-uigothic/ipagui.ttf"

const uword *STRING = u"Hello world!こんにちは世界";

int width, height;

int main(int argc, char **argv)
{
  zxWindow win;
  zxImage img;
  zxFTImage font;

  if( !zxFTImageInitLibrary() ) return 1;
  if( !zxFTImageLoadFont( &font, FONTFILE, 32, 100 ) ) return 1;

  zxInit();
  zxWindowCreateAndOpen( &win, 0, 0, WIDTH, HEIGHT );
  zxWindowSetTitle( &win, "FreeType test" );
  zxWindowSetBGColorByName( &win, "blue" );
  zxWindowClear( &win );
  zxWindowSetColorByName( &win, "dark red" );
  zxDrawFillTriangle( &win, 100, 300, 300, 50, 550, 400 );

  zxImageAllocDefault( &img, zxWindowWidth(&win), zxWindowHeight(&win) );
  zxImageAllocMask( &img );
  zxImageDrawString( &img, &font, 10, HEIGHT-240, STRING, 0.4, 1.0, 0.8 );
  zxImageDrawAll( &win, &img, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &img );
  zxWindowClose( &win );
  zxFTImageUnloadFont( &font );
  zxFTImageExitLibrary();
  zxExit();
  return 0;
}

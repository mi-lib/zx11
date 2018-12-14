#include <zx11/zximage_png.h>

int main(int argc, char *argv[])
{
  zxImage dat;

  zxInit();
  zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" );
  zxImageWritePNGFile( &dat, "hoge.png" );
  zxImageDestroy( &dat );
  zxClose();
  return 0;
}

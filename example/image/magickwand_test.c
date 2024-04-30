#include <zx11/zximage.h>

void usage(const char *arg)
{
  eprintf( "Usage: %s <input filename> [output filename]\n", arg );
  exit( EXIT_SUCCESS );
}

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img;

  if( argc < 2 ) usage( argv[0] );
  zxInit();
  MagickWandGenesis();
  zxImageReadFileMagickWand( &img, argv[1] );
  zxWindowCreateAndOpen( &win, 0, 0, img.width, img.height );
  zxImageDrawAll( &win, &img, 0, 0 );
  zxFlush();
  getchar();
  if( argc > 2 ){
    eprintf( "output %s.\n", argv[2] );
    zxImageWriteFileMagickWand( &img, argv[2] );
  }
  MagickWandTerminus();
  zxImageDestroy( &img );
  zxWindowDestroy( &win );
  zxExit();
  return EXIT_SUCCESS;
}

#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img;

  if( argc == 1 ){
    eprintf( "image file unspecified.\n" );
    return 1;
  }
  zxInit();
  if( zxImageReadFile( &img, argv[1] ) == 0 )
    return 1;

  zxWindowCreateAndOpen( &win, 0, 0, img.width, img.height );
  zxImageDrawAll( &win, &img, 0, 0 );
  zxFlush();
  getchar();
  zxWindowDestroy( &win );
  zxImageDestroy( &img );
  zxClose();
  return 0;
} 

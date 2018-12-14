#include <zx11/zximage_pdt.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat;

  zxInit();
  while( *++argv ){
    if( zxImageReadPDTFile( &dat, *argv ) == 0 ) exit( 1 );

    zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
    zxWindowSetBG( &win, "lemon chiffon" );
    zxImageDraw( &win, &dat, 70, 40, 500, 400, 0, 0 );
    zxFlush();
    getchar();

    zxImageDestroy( &dat );
    zxWindowDestroy( &win );
  }
  zxClose();
  return 0;
} 

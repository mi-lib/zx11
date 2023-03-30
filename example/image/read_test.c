#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat;

  if( argc < 2 ){
    eprintf( "Usage: %s <image file>\n", argv[0] );
    exit( 0 );
  }
  zxInit();
  if( zxImageReadFile( &dat, argv[1] ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxExit();
  return 0;
} 

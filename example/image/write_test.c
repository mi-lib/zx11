#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxImage dat;

  if( argc < 2 ){
    eprintf( "Usage: %s <image file>\n", argv[0] );
    exit( 0 );
  }
  zxInit();
  if( zxImageReadFile( &dat, argv[1] ) != 0 ){
    zxImageWriteFile( &dat, "output.bmp" );
    zxImageWriteFile( &dat, "output.png" );
    zxImageWriteFile( &dat, "output.jpeg" );
    zxImageWriteFile( &dat, "output.tiff" );
    zxImageWriteFile( &dat, "output.ppm" );
  }
  zxImageDestroy( &dat );
  zxExit();
  return 0;
} 

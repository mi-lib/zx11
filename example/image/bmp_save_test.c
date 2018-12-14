#include <zx11/zximage_dib.h>

int main(int argc, char *argv[])
{
  zxImage dat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxImageWriteBMPFile( &dat, argc > 2 ? argv[2] : "hoge.bmp" );
  zxClose();
  return 0;
}

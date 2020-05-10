#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;
  int i;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*5, dat.height );
  zxImageDrawAll( &win, &dat, 0, 0 );
  for( i=1; i<=4; i++ ){
    zxImageClone( &dat, &cdat );
    zxImageAntialias( &cdat, &dat );
    zxImageDrawAll( &win, &dat, dat.width*i, 0 );
    zxImageDestroy( &cdat );
  }
  zxImageDestroy( &dat );
  zxFlush();
  getchar();
  zxExit();
  return 0;
} 

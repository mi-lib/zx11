#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;
  int i;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  while( zxGetEvent() != Expose );

  printf( "original image" );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  for( i=1; i<=5; i++ ){
    zxImageClone( &dat, &cdat );
    printf( "antialiasing x %d", i );
    zxImageAntialias( &cdat, &dat );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
    getchar();
    zxImageDestroy( &cdat );
  }
  zxImageDestroy( &dat );
  zxClose();
  return 0;
} 

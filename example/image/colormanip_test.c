#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, cdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &cdat, dat.width-20, dat.height-20 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  while( zxGetEvent() != Expose );

  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  printf( "grayscalize\n" );
  zxImageGrayscalize( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  printf( "differentiation\n" );
  zxImageDiff( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  printf( "differentiation\n" );
  zxImageDiff( &cdat, &dat );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  printf( "integration\n" );
  zxImageIntegral( &dat, &cdat );
  zxImageDrawAll( &win, &cdat, 0, 0 );
  zxFlush();
  getchar();

  printf( "integration\n" );
  zxImageIntegral( &cdat, &dat );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxImageDestroy( &cdat );
  zxClose();
  return 0;
} 

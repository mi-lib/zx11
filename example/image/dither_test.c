#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, ddat, gdat, gddat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );

  /* original image */
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*2, dat.height*2 );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxImageAllocDefault( &ddat, dat.width, dat.height );
  zxImageDither( &dat, &ddat );
  zxImageDrawAll( &win, &ddat, dat.width, 0 );
  zxImageAllocDefault( &gdat, dat.width, dat.height );
  zxImageGrayscalize( &dat, &gdat );
  zxImageDrawAll( &win, &gdat, 0, dat.height );
  zxImageAllocDefault( &gddat, dat.width, dat.height );
  zxImageDither( &gdat, &gddat );
  zxImageDrawAll( &win, &gddat, dat.width, dat.height );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxImageDestroy( &ddat );
  zxImageDestroy( &gdat );
  zxImageDestroy( &gddat );
  zxExit();
  return 0;
} 

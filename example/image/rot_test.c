#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, rdat, fdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &rdat, dat.width, dat.height );
  zxImageAllocDefault( &fdat, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  while( zxGetEvent() != Expose );

  printf( "original image\n" );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  printf( "simple rotation\n" );
  zxImageRot( &dat, &rdat, dat.width/2, dat.height/2, dat.width/2, dat.height/2, zDeg2Rad(30) );
  zxImageAntialias( &rdat, &fdat );
  zxImageDrawAll( &win, &rdat, 0, 0 );
  zxFlush();
  getchar();

  printf( "filtered rotation\n" );
  zxImageRotFilt( &dat, &rdat, dat.width/2, dat.height/2, dat.width/2, dat.height/2, zDeg2Rad(30) );
  zxImageDrawAll( &win, &rdat, 0, 0 );
  zxFlush();
  getchar();

  printf( "simple rotation & antialiasing\n" );
  zxImageDrawAll( &win, &fdat, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxImageDestroy( &rdat );
  zxImageDestroy( &fdat );
  zxClose();

  return 0;
} 

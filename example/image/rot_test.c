#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, rdat, fdat, adat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &rdat, dat.width, dat.height );
  zxImageAllocDefault( &fdat, dat.width, dat.height );
  zxImageAllocDefault( &adat, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*2, dat.height*2 );

  zxImageRot( &dat, &rdat, dat.width/2, dat.height/2, dat.width/2, dat.height/2, zDeg2Rad(30) );
  zxImageRotFilt( &dat, &fdat, dat.width/2, dat.height/2, dat.width/2, dat.height/2, zDeg2Rad(30) );
  zxImageAntialias( &rdat, &adat );

  zxImageDrawAll( &win, &dat, 0, 0 );
  zxImageDrawAll( &win, &rdat, dat.width, 0 );
  zxImageDrawAll( &win, &fdat, dat.width, dat.height );
  zxImageDrawAll( &win, &adat, 0, dat.height );

  printf( "(left top) original image\n" );
  printf( "(right top) simple rotation\n" );
  printf( "(left bottom) filtered rotation\n" );
  printf( "(right bottom) simple rotation & antialiasing\n" );
  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxImageDestroy( &rdat );
  zxImageDestroy( &fdat );
  zxImageDestroy( &adat );
  zxExit();
  return 0;
} 

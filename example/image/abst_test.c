#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat, rdat, gdat, bdat;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &rdat, dat.width, dat.height );
  zxImageAllocDefault( &gdat, dat.width, dat.height );
  zxImageAllocDefault( &bdat, dat.width, dat.height );

  zxWindowCreateAndOpen( &win, 0, 0, dat.width*2, dat.height*2 );
  zxImageAbstRGB( &dat, &rdat, &gdat, &bdat );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxImageDrawAll( &win, &rdat, dat.width, 0 );
  zxImageDrawAll( &win, &gdat, dat.width, dat.height );
  zxImageDrawAll( &win, &bdat, 0, dat.height );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxImageDestroy( &rdat );
  zxImageDestroy( &gdat );
  zxImageDestroy( &bdat );
  zxExit();
  return 0;
} 

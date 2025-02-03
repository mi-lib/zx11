#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat1, dat2;

  zxInit();
  if( zxImageReadFile( &dat1, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dat2, dat1.width, dat1.height );
  zxWindowCreateAndOpen( &win, 0, 0, dat1.width*5, dat1.height );
  zxImageDrawAll( &win, &dat1, 0, 0 );

  zxImageDiff( &dat1, &dat2 );
  zxImageDrawAll( &win, &dat2, dat1.width, 0 );
  zxImageDiff( &dat2, &dat1 );
  zxImageDrawAll( &win, &dat1, dat1.width*2, 0 );
  zxImageIntegral( &dat1, &dat2 );
  zxImageDrawAll( &win, &dat2, dat2.width*3, 0 );
  zxImageIntegral( &dat2, &dat1 );
  zxImageDrawAll( &win, &dat1, dat1.width*4, 0 );

  zxFlush();
  getchar();
  zxImageDestroy( &dat1 );
  zxImageDestroy( &dat2 );
  zxExit();
  return 0;
} 

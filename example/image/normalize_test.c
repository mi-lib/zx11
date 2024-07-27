#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, mid, dst;

  if( argc < 2 ){
    eprintf( "Usage: %s <image file>\n", argv[0] );
    return 0;
  }
  zxInit();
  if( zxImageReadFile( &src, argv[1] ) == 0 ) return 1;
  zxImageAllocDefault( &dst, src.width, src.height );
  zxImageAllocDefault( &mid, src.width, src.height );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*3, src.height );

  zxImageToneDown( &src, &mid, 0.5 );
  zxImageNormalize( &mid, &dst );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &mid, src.width, 0 );
  zxImageDrawAll( &win, &dst, src.width*2, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &mid );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

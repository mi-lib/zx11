#include <zx11/zximage.h>

#define N 6

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img[N];
  int i;

  zxInit();
  if( zxImageReadFile( &img[0], argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, img[0].width*N, img[0].height );
  zxImageDrawAll( &win, &img[0], 0, 0 );
  for( i=1; i<N; i++ ){
    zxImageAllocDefault( &img[i], img[0].width, img[0].height );
    zxImageSmoothBilateral( &img[i-1], &img[i] );
    zxImageDrawAll( &win, &img[i], img[0].width*i, 0 );
  }
  zxFlush();
  getchar();
  for( i=1; i<N; i++ )
    zxImageDestroy( &img[i] );
  zxExit();
  return 0;
}

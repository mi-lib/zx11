#include <zx11/zximage.h>

#define N 4

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage org, src, dat, cdat;
  int i;

  zxInit();
  if( zxImageReadFile( &org, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &src, org.width*0.8, org.height*0.8 );
  zxImageResize( &org, &src );

  zxWindowCreateAndOpen( &win, 0, 0, src.width*N, src.height*5 );
  for( i=0; i<5; i++ ){
    zxImageDrawAll( &win, &src, 0, src.height*i );
  }
  zxImageClone( &src, &dat );
  for( i=1; i<N; i++ ){
    zxImageClone( &dat, &cdat );
    zxImageAntialias( &cdat, &dat );
    zxImageDrawAll( &win, &dat, dat.width*i, 0 );
    zxImageDestroy( &cdat );
  }
  zxImageCopy( &src, &dat );
  for( i=1; i<N; i++ ){
    zxImageClone( &dat, &cdat );
    zxImageSmoothGaussian( &cdat, &dat );
    zxImageDrawAll( &win, &dat, dat.width*i, dat.height );
    zxImageDestroy( &cdat );
  }
  for( i=1; i<N; i++ ){
    zxImageSmoothMedian( &src, &dat, 2*i+1 );
    zxImageDrawAll( &win, &dat, dat.width*i, dat.height*2 );
  }
  for( i=1; i<N; i++ ){
    zxImageSmoothMin( &src, &dat, 2*i+1 );
    zxImageDrawAll( &win, &dat, dat.width*i, dat.height*3 );
  }
  for( i=1; i<N; i++ ){
    zxImageSmoothMax( &src, &dat, 2*i+1 );
    zxImageDrawAll( &win, &dat, dat.width*i, dat.height*4 );
  }
  zxImageDestroy( &dat );
  zxImageDestroy( &src );
  zxImageDestroy( &org );
  zxFlush();
  getchar();
  zxExit();
  return 0;
} 

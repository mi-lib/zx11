#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dat, cdat;
  int i;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*5, src.height*3 );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &src, 0, src.height );
  zxImageDrawAll( &win, &src, 0, src.height*2 );
  zxImageClone( &src, &dat );
  for( i=1; i<=4; i++ ){
    zxImageClone( &dat, &cdat );
    zxImageAntialias( &cdat, &dat );
    zxImageDrawAll( &win, &dat, dat.width*i, 0 );
    zxImageDestroy( &cdat );
  }
  zxImageCopy( &src, &dat );
  for( i=1; i<=4; i++ ){
    zxImageClone( &dat, &cdat );
    zxImageGaussian( &cdat, &dat );
    zxImageDrawAll( &win, &dat, dat.width*i, dat.height );
    zxImageDestroy( &cdat );
  }
  for( i=1; i<=4; i++ ){
    zxImageMedian( &src, &dat, 2*i+1 );
    zxImageDrawAll( &win, &dat, dat.width*i, dat.height*2 );
  }
  zxImageDestroy( &dat );
  zxImageDestroy( &src );
  zxFlush();
  getchar();
  zxExit();
  return 0;
} 

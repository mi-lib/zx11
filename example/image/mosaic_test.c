#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dst;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dst, src.width, src.height );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*2, src.height );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageCopy( &src, &dst );
  zxImageMosaic( &dst,
    0.2*dst.width, 0.2*dst.height,
    0.6*dst.width, 0.6*dst.height, dst.width/30, dst.height/30 );
  zxImageDrawAll( &win, &dst, dst.width, 0 );

  zxFlush();
  getchar();
  zxImageDestroy( &src );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

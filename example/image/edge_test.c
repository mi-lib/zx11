#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, gsc, dst;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*2, src.height*3 );
  zxImageAllocDefault( &gsc, src.width, src.height );
  zxImageGrayscalize( &src, &gsc );
  zxImageAllocDefault( &dst, src.width, src.height );
  zxImageDrawAll( &win, &gsc, 0, 0 );
  zxImagePrewittV( &gsc, &dst );
  zxImageDrawAll( &win, &dst, 0, src.height );
  zxImagePrewittH( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width, src.height );
  zxImageSobelV( &gsc, &dst );
  zxImageDrawAll( &win, &dst, 0, src.height*2 );
  zxImageSobelH( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width, src.height*2 );
  zxImageLaplacian( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &gsc );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

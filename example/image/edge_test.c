#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, gsc, dst;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*3, src.height*3 );
  zxImageAllocDefault( &gsc, src.width, src.height );
  zxImageAllocDefault( &dst, src.width, src.height );

  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageGrayscalize( &src, &gsc );
  zxImageDrawAll( &win, &gsc, src.width, 0 );
  zxImageLaplacian( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width*2, 0 );

  zxImagePrewittV( &gsc, &dst );
  zxImageDrawAll( &win, &dst, 0, src.height );
  zxImagePrewittH( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width, src.height );
  zxImagePrewitt( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width*2, src.height );

  zxImageSobelV( &gsc, &dst );
  zxImageDrawAll( &win, &dst, 0, src.height*2 );
  zxImageSobelH( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width, src.height*2 );
  zxImageSobel( &gsc, &dst );
  zxImageDrawAll( &win, &dst, src.width*2, src.height*2 );
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &gsc );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

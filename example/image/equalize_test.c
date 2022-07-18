#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dst;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dst, src.width, src.height );
  zxImageEqualize( &src, &dst );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*2, src.height );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &dst, src.width, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

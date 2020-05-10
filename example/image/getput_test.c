#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, dest;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dest, src.width, src.height );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*2, src.height );

  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageGet( &src, &dest, 0, 0 );
  zxImageDrawAll( &win, &dest, src.width, 0 );
  zxImagePut( &src, &dest, 100, 100 );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxFlush();
  getchar();
  zxImageDestroy( &src );
  zxImageDestroy( &dest );
  zxExit();
  return 0;
} 

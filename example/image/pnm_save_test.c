#include <zx11/zximage_pnm.h>

int main(int argc, char *argv[])
{
  zxImage src, dest;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &dest, src.width, src.height );
  zxImageAntialias( &src, &dest );

  zxImageWritePBMFile( &dest, "hoge.pbm" );
  zxImageWritePGMFile( &dest, "hoge.pgm" );
  zxImageWritePPMFile( &dest, "hoge.ppm" );

  zxImageDestroy( &dest );
  zxImageDestroy( &src );
  zxClose();

  return 0;
} 

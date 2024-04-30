#include <unistd.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat1, dat2, blend;
  double alpha;
  char *file1 = argc > 1 ? argv[1] : "../fig/lena.jpg";
  char *file2 = argc > 2 ? argv[2] : "../fig/lena_flop.jpg";

  zxInit();
  zxImageReadFile( &dat1, file1 );
  zxImageReadFile( &dat2, file2 );
  zxImageClone( &dat1, &blend );

  zxWindowCreateAndOpen( &win, 0, 0, blend.width, blend.height );
  for( alpha=0; alpha<=1; alpha+=0.1 ){
    printf( "alpha = %f\n", alpha );
    zxImagePutAlphaBlend( &blend, &dat1, &dat2, 0, 0, alpha );
    zxImageDrawAll( &win, &blend, 0, 0 );
    zxFlush();
    sleep( 1 );
  }
  zxImageDestroy( &dat1 );
  zxImageDestroy( &dat2 );
  zxImageDestroy( &blend );
  zxExit();
  return 0;
}

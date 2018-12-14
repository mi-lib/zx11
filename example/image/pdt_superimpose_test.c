#include <zx11/zxpdt.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img1, img2;

  if( argc < 3 ) exit( 1 );

  zxInit();
  zxImageInit( &img2 );
  if( zxImageReadFile( &img1, argv[1] ) == 0 ) exit( 1 );
  if( zxImageReadPDTFile( &img2, argv[2] ) == 0 ) exit( 1 );

  zxWindowCreateAndOpen( &win, 20, 20, img1.width, img1.height );
  while( zxGetEvent() != Expose );
  if( img2.mask_buf != NULL )
    zxImagePutSuperimpose( &img1, &img2, 0, 0 );
  zxImageDrawAll( &win, &img1, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &img1 );
  zxImageDestroy( &img2 );
  zxClose();
  return 0;
}

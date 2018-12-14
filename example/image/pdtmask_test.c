#include <zx11/zximage_pdt.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img1, img2;
  register int i, j;
  ubyte val, *mp;
  zxPixelManip pm;

  if( argc < 2 ) exit( 1 );

  zxInit();
  if( zxImageReadPDTFile( &img1, argv[1] ) == 0 ) exit( 1 );
  zxWindowCreateAndOpen( &win, 20, 20, img1.width, img1.height );
  zxImageDrawAll( &win, &img1, 0, 0 );
  zxFlush();
  getchar();

  zxImageInit( &img2 );
  if( ( mp = img1.mask_buf ) ){
    zxImageAllocDefault( &img2, img1.width, img1.height );
    zxPixelManipSetDefault( &pm );
    for( i=0; i<img1.height; i++ )
      for( j=0; j<img1.width; j++ ){
        val = *mp++;
        zxImageCellFromRGB( &img2, &pm, j, i, val, val, val );
      }
    zxImageDrawAll( &win, &img2, 0, 0 );
    zxFlush();
    getchar();
  }

  zxImageDestroy( &img1 );
  zxImageDestroy( &img2 );
  zxClose();
  return 0;
}

#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat1, dat2, blend;
  zxPixel mask;
  int x, y;
  double alpha;

  if( argc < 4 ){
    printf( "Usage: alphablendmasktest <bgfile> <fgfile1> <fgfile2>\n" );
    exit( 1 );
  }
  zxInit();
  zxImageReadFile( &blend, argv[1] );
  zxImageReadFile( &dat1, argv[2] );
  zxImageReadFile( &dat2, argv[3] );
  printf( "enter mask pixel pos:\n" );
  printf( "x>" ); if( scanf( "%d", &x ) != 1 ) exit( 1 );
  printf( "y>" ); if( scanf( "%d", &y ) != 1 ) exit( 1 );
  mask = zxImageCellPixel( &dat1, x, y );
  printf( "%X\n", mask );
  zxWindowCreateAndOpen( &win, 0, 0, blend.width, blend.height );
  for( alpha=0; alpha<=1; alpha+=0.1 ){
    printf( "alpha = %f\n", alpha );
    zxImagePutAlphaBlendMasked( &blend, &dat2, &dat1, 0, 0, alpha, mask );
    zxImageDrawAll( &win, &blend, 0, 0 );
    zxFlush();
    getchar();
  }
  zxImageDestroy( &dat1 );
  zxImageDestroy( &dat2 );
  zxImageDestroy( &blend );
  zxWindowClose( &win );
  zxExit();
  return 0;
}

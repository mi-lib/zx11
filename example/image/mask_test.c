#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage back, front;
  zxPixel mask;
  int x, y;

  if( argc < 3 ){
    printf( "Usage: masktest <bgfile> <fgfile>\n" );
    exit( 1 );
  }

  zxInit();
  if( !zxImageReadFile( &back, argv[1] ) ||
      !zxImageReadFile( &front, argv[2]  ) )
    exit( 1 );

  printf( "enter mask pixel pos:\n" );
  printf( "x = " ); scanf( "%d", &x );
  printf( "y = " ); scanf( "%d", &y );
  mask = zxImageCellPixel( &front, x, y );
  printf( "mask = %lX\n", mask );
  zxImagePutMasked( &back, &front, 0, 0, mask );

  zxWindowCreateAndOpen( &win, 0, 0, back.width, back.height );
  zxImageDrawAll( &win, &back, 0, 0 );
  zxFlush();
  fflush( stdout );
  while( getchar() != EOF );
  zxImageDestroy( &back );
  zxImageDestroy( &front );
  zxWindowClose( &win );
  zxClose();
  return 0;
}

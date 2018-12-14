#include <zx11/zximage_pnm.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage back, front;
  zxPixelManip pm;

  if( argc < 3 ){
    printf( "Usage: masktest <bgfile> <fgfile>\n" );
    exit( 1 );
  }

  zxInit();
  if( !zxImageReadFile( &back, argv[1] ) ||
      !zxImageReadPNMFile( &front, argv[2]  ) )
    exit( 1 );
  zxWindowCreate( &win, 0, 0, back.width, back.height );
  while( zxGetEvent() != Expose );

  zxPixelManipSetDefault( &pm );
  zxImageCreateMask( &front, BlackPixel( zxdisplay, 0 ) );
  zxImageFill( &front, pm.PixelFromRGB(0xff,0xff,0) );
  zxImageDrawAll( &win, &back, 0, 0 );
  zxImageDrawAll( &win, &front, 0, 0 );
  zxWindowOpen( &win );
  zxFlush();
  getchar();

  zxImageDestroy( &back );
  zxImageDestroy( &front );
  zxWindowClose( &win );
  zxClose();
  return 0;
}

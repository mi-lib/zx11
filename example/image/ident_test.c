#define __ZX11_USE_XEXT
#define __ZX11_USE_XPM
#define __ZX11_USE_PNG
#define __ZX11_USE_JPEG
#define __ZX11_USE_BMP
#define __ZX11_USE_MAG
#define __ZX11_USE_PDT

#include <zx11/zximage.h>
#include <zx11/zximage_supported.h>

int main(int argc, char *argv[])
{
  if( argc <= 1 ){
    eprintf( "Usage: %s <test file>\n", argv[0] );
    exit( 1 );
  }
  if( zxImageFileIsPNM( argv[1] ) )
    printf( "PNM\n" );
  else
  if( zxImageFileIsBMP( argv[1] ) )
    printf( "BMP\n" );
  else
  if( zxImageFileIsJPEG( argv[1] ) )
    printf( "JPEG\n" );
  else
  if( zxImageFileIsPNG( argv[1] ) )
    printf( "PNG\n" );
  else
  if( zxImageFileIsMAG( argv[1] ) )
    printf( "MAG\n" );
  else
  if( zxImageFileIsPDT( argv[1] ) )
    printf( "PDT\n" );
  else
    printf( "unknown.\n" );
  return 0;
}

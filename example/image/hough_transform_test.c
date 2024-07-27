#include <zm/zm.h>
#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src;
  zxHoughBinList bin_list;
  zxHoughBinListCell *cp;
  double s, c;
  int i, n = 20, theta_div = 360, dist_div = 150;

  if( argc < 2 ){
    eprintf( "Usage: %s <image file>\n", argv[0] );
    return 0;
  }
  zxInit();
  if( zxImageReadFile( &src, argv[1] ) == 0 ) return 1;
  zxWindowCreateAndOpen( &win, 0, 0, src.width, src.height );
  zxImageDrawAll( &win, &src, 0, 0 );
  zxFlush();

  zxImageHoughLines( &bin_list, &src, theta_div, dist_div );
  zxWindowSetColorByName( &win, "green" );
  for( cp=zListTail(&bin_list), i=0; i<n; i++, cp=zListCellNext(cp) ){
    printf( "theta = %g, dist = %g\n", cp->data.theta, cp->data.dist );
    s = sin( cp->data.theta );
    c = cos( cp->data.theta );
    if( fabs(s) > fabs(c) ){
      zxDrawLine( &win, 0, cp->data.dist / s, src.width, ( cp->data.dist - src.width * c ) / s );
    } else{
      zxDrawLine( &win, cp->data.dist / c, 0, ( cp->data.dist - src.height * s ) / c, src.height );
    }
  }
  zxFlush();
  getchar();

  zxImageDestroy( &src );
  zxExit();
  return 0;
} 

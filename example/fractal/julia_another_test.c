#include <zx11/zximage.h>
#include <math.h>

#define K 180
#define A 0.11031
#define B -0.670371
#define W 500

int main(int argc, char *argv[])
{
  zxWindow c;
  zxImage img;
  int i, j;
  double x, y, zx, zy, r, t, u, v;

  zxInit();
  zxImageAllocDefault( &img, 700, 500 );
  x = y = 0;
  for( i=1; i<=W; i++ ){
    for( j=1; j<=W; j++ ){
      zx = x - A;
      zy = y - B;
      r = sqrt( sqrt( zx*zx + zy*zy ) );
      t = 0.5 * atan2(zy,zx);
      if( rand() % 2 == 1 ) r = -r;
      x = r * cos( t );
      y = r * sin( t );
      u = ( x + 4 ) * K - 350;
      v = ( 2 - y ) * K - 100;
      zxImageCellFromRGB( &img, u, v, 0xff, 0xff, 0xff );
    }
  }
  zxWindowCreateAndOpen( &c, 100, 100, 700, 500 );
  zxWindowSetTitle( &c, "Julia series" );
  zxImageDrawAll( &c, &img, 0, 0 );
  zxFlush();
  getchar();
  zxExit();
  return 1;
}

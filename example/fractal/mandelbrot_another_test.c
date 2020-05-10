#include <zx11/zximage.h>

#define A -0.745429
#define B 0.113009
#define W 0.00001
#define H 198
#define N 400

int main(int argc, char *argv[])
{
  zxWindow c;
  zxImage img;
  zxPixelManip pm;
  register int i, j, l;
  float cf;
  double x, y, zx, zy, u, v, mx;

  zxInit();
  zxImageAllocDefault( &img, 400, 400 );
  zxPixelManipSetDefault( &pm );
  for( i=-H; i<=H; i++ ){
    u = W * i / H + A;
    for( j=-H; j<=H; j++ ){
      v = W * j / H + B;
      x = y = 0;
      for( l=1; l<=N; l++ ){
        zx = x*x - y*y + u;
        zy = 2*x*y     + v;
        x = zx;
        y = zy;
        if( ( mx = x*x + y*y ) >= 20.0 ) break;
      }
      cf = 0xff * ( (float)l / N );
      zxImageCellFromRGB( &img, &pm, 200+i, 200-j, 0, cf, cf );
    }
  }
  zxWindowCreateAndOpen( &c, 100, 100, 400, 400 );
  zxWindowSetTitle( &c, "Mandelbrot series" );
  zxImageDrawAll( &c, &img, 0, 0 );
  zxFlush();
  getchar();
  zxExit();
  return 1;
}

/*
 * Mandelbrot series
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000.10.27. Created.
 */

#include <stdio.h>
#include <zx11/zxutil.h>

#define A -0.745429
#define B 0.113009
#define W 0.00001
#define H 198
#define N 400

int main(int argc, char *argv[])
{
  zxWindow c;
  register int i, j, l;
  float cf;
  double x, y, zx, zy, u, v, mx;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 400, 400 );
  zxWindowSetTitle( &c, "Mandelbrot series" );

  printf( "*** Mandelbrot series on X11 ***\n" );
  zxWindowClear( &c );

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
      cf = 0xffff * ( (float)l / N );
      zxSetColorMap( &c, zxGetRGBColor( &c, 0, cf, cf ) );
      zxDrawPoint( &c, 200+i, 200-j );
    }
  }

  zxFlush();
  getchar();

  zxClose();
  return 1;
}

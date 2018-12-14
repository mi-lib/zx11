/*
  * Mandelbrot Series
 * (C)Copyright, Zhidao since 2000.
 *
 * 2001. 7.21. Created.
 * 2008. 1.18. Last updated.
 */

#include <math.h>
#include <zx11/zximage.h>

#define WIDTH_X 200
#define WIDTH_Y 200

#ifndef PI
#define PI 3.14159265359
#endif

double ratio_x=1.0/WIDTH_X, ratio_y=1.0/WIDTH_Y;
int ox=WIDTH_X*2, oy=WIDTH_Y*1.5;
int wx=WIDTH_X*3, wy=WIDTH_Y*3;

zxWindow c;

#define C_NUM 50
static zxPixel c_pix[C_NUM];

int mandelbrot_pixel(zxImage *img, int x, int y)
{
  double cx, cy, sx, sy, zx=0, zy=0;
  register int i;

  cx = ( x - ox ) * ratio_x;
  cy = ( y - oy ) * ratio_y;
  for( i=0; i<C_NUM; i++ ){
    sx = zx*zx;
    sy = zy*zy;
    if( ( sx + sy ) >= 4 ) break;
    zy = 2*zx*zy + cy;
    zx = sx - sy + cx;
  }
  zxImageCellFromPixel( img, x, y, c_pix[i] );
  return i;
}

void mandelbrot(zxImage *img)
{
  register int i, j;

  for( i=0; i<=wx; i++ )
    for( j=0; j<=wy; j++ )
      mandelbrot_pixel( img, i, j );
}

void color_init(void)
{
  register zxPixel r, g, b;
  register int i;
  zxPixelManip pm;

  zxPixelManipSetDefault( &pm );
  for( i=0; i<C_NUM; i++ ){
    r = 0xff * exp(-(double)i/C_NUM*10);
    g = 0xff * 0.5*( 1 - cos( 2*PI*pow((double)i/C_NUM,0.7) ) );
    b = 0xff * zMin(1.2*(double)i/C_NUM,1);
    c_pix[i] = pm.PixelFromRGB( r, g, b );
  }
}

int main(int argc, char *argv[])
{
  zxImage img;

  printf( "*** Mandelbrot Series on X11 ***\n" );

  zxInit();
  color_init();
  zxImageAllocDefault( &img, wx+1, wy+1 );
  mandelbrot( &img );

  zxWindowCreate( &c, 100, 100, wx, wy );
  zxWindowSetTitle( &c, "Mandelbrot Series" );
  zxWindowOpen( &c );
  zxImageDrawAll( &c, &img, 0, 0 );
  zxFlush();
  getchar();

  zxClose();
  return 0;
}

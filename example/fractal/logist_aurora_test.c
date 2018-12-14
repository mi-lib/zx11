/*
 * logistic map
 * (C)Copyright, Zhidao since 2003
 */

#include <zx11/zximage.h>

#define LOOP 30
#define W 640
#define H 480
#define R_MIN 3.0
#define R_MAX 4.0
#define R_STEP 0.0005
#define X_MIN 0.0
#define X_MAX 1.0
#define X_STEP 0.0005

#define DENS_D 0.0005

double dens[H][W];

double logistic_func(double r, double x)
{
  return r * x * ( 1 - x );
}

double logistic_put(double r, double x)
{
  register int i, j, k;

  for( i=0; i<LOOP; i++ ){
    x = logistic_func( r, x );
    j = (x-X_MIN) * (H-1) / (X_MAX-X_MIN);
    k = (r-R_MIN) * (W-1) / (R_MAX-R_MIN);
    dens[j][k] += DENS_D;
  }
  return x;
}

void logistic(void)
{
  double r, x;
  double prg, prg_scale = 10;

  for( r=R_MIN; r<=R_MAX; r+=R_STEP ){
    if( ( prg = 100 * (r-R_MIN) / (R_MAX-R_MIN) ) > prg_scale ){
      printf( "%f %%\n", prg );
      prg_scale += 10;
    }
    for( x=X_MIN; x<=X_MAX; x+=X_STEP )
      logistic_put( r, x );
  }
}

void density_to_image(zxImage *img)
{
  register int i, j;
  ubyte p1, p2;
  zxPixelManip pm;

  zxPixelManipSetDefault( &pm );
  for( i=0; i<H; i++ )
    for( j=0; j<W; j++ ){
      p1 = zMin( dens[i][j] * 0xff, 0xff );
      p2 = zMin( 0.8*dens[i][j] * 0xff, 0xff );
      zxImageCellFromRGB( img, &pm, j, i, p2, p1, p2 );
    }
}

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img;

  zxInit();
  zxImageAllocDefault( &img, W, H );
  logistic();
  density_to_image( &img );

  zxWindowCreateAndOpen( &win, 0, 0, W, H );
  zxWindowSetTitle( &win, "logistic map" );
  zxWindowClear( &win );
  zxImageDrawAll( &win, &img, 0, 0 );
  zxFlush();
  getchar();

  zxClose();
  return 1;
}

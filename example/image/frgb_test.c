#include <unistd.h>
#include <zx11/zximage.h>

double myabs(double x){ return x >= 0 ? x : -x; }

void brightR(zxPixelManip *pm, zxImage *img, double brightness)
{
  register int i, j;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      zxImageCellFromPixel( img, j, i,
        pm->PixelFromFRGB( brightness, 0, 0 ) );
}

void brightG(zxPixelManip *pm, zxImage *img, double brightness)
{
  register int i, j;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      zxImageCellFromPixel( img, j, i,
        pm->PixelFromFRGB( 0, brightness, 0 ) );
}

void brightB(zxPixelManip *pm, zxImage *img, double brightness)
{
  register int i, j;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      zxImageCellFromPixel( img, j, i,
        pm->PixelFromFRGB( 0, 0, brightness ) );
}

void bright(zxPixelManip *pm, zxImage *img, double brightness)
{
  register int i, j;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      zxImageCellFromPixel( img, j, i, zxPixelFromFGS( pm, brightness ) );
}

#define DIV 10

int main(void)
{
  zxWindow win;
  zxImage dat;
  register int i;
  double alpha;
  zxPixelManip pm;

  zxInit();
  zxImageAllocDefault( &dat, 300, 300 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );

  zxPixelManipSet( &pm, zxdepth );
  for( i=0; i<DIV; i++ ){
    alpha = 1 - 2 * myabs( (double)i / DIV - 0.5 );
    brightR( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
    usleep( 100000 );
  }
  for( i=0; i<DIV; i++ ){
    alpha = 1 - 2 * myabs( (double)i / DIV - 0.5 );
    brightG( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
    usleep( 100000 );
  }
  for( i=0; i<DIV; i++ ){
    alpha = 1 - 2 * myabs( (double)i / DIV - 0.5 );
    brightB( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
    usleep( 100000 );
  }
  for( i=0; i<DIV; i++ ){
    alpha = 1 - 2 * myabs( (double)i / DIV - 0.5 );
    bright( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
    usleep( 100000 );
  }
  getchar();
  zxImageDestroy( &dat );
  zxExit();
  return 0;
}

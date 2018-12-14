#include <zx11/zximage.h>

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

int main(void)
{
  zxWindow win;
  zxImage dat;
  register double alpha;
  zxPixelManip pm;

  zxInit();
  zxImageAllocDefault( &dat, 300, 300 );
  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );

  zxPixelManipSet( &pm, zxdepth );
  for( alpha=1.0; alpha>0; alpha-=0.1 ){
    brightR( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
  }
  for( alpha=1.0; alpha>0; alpha-=0.1 ){
    brightG( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
  }
  for( alpha=1.0; alpha>0; alpha-=0.1 ){
    brightB( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
  }
  for( alpha=1.0; alpha>0; alpha-=0.1 ){
    bright( &pm, &dat, alpha );
    zxImageDrawAll( &win, &dat, 0, 0 );
    zxFlush();
  }
  getchar();
  zxImageDestroy( &dat );
  zxWindowClose( &win );
  zxClose();
  return 0;
}

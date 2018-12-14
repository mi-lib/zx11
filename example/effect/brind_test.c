#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

void zxPixmapDrawBrindUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, j;

  for( i=step; i<d; i+=step ){
    for( j=h-i; j>=0; j-=d )
      zxPixmapDrawStrip( xc, pmap, mask, 0, j, w, step, x, y+j );
    if( j + step > 0 )
      zxPixmapDrawStrip( xc, pmap, mask, 0, 0, w, step+j, x, y );
    zxSleep( 1 );
  }
  if( ( step = i - d + 1 ) > 0 ){
    for( j=h-d; j>=0; j-=d )
      zxPixmapDrawStrip( xc, pmap, mask, 0, j, w, step, x, y+j );
    if( j + step > 0 )
      zxPixmapDrawStrip( xc, pmap, mask, 0, 0, w, step+j, x, y );
    zxSleep( 1 );
  }
}

void zxPixmapDrawBrindDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, j;
  int y_max;

  y_max = h - step;
  for( i=0; i<d; i+=step ){
    for( j=i; j<y_max; j+=d )
      zxPixmapDrawStrip( xc, pmap, mask, 0, j, w, step, x, y+j );
    if( h > j )
      zxPixmapDrawStrip( xc, pmap, mask, 0, j, w, h-j, x, y+j );
    zxSleep( 1 );
  }
}

void zxPixmapDrawBrindToRight(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, j;
  int x_max;

  x_max = w - step;
  for( i=0; i<d; i+=step ){
    for( j=i; j<x_max; j+=d )
      zxPixmapDrawStrip( xc, pmap, mask, j, 0, step, h, x+j, y );
    if( w > j )
      zxPixmapDrawStrip( xc, pmap, mask, j, 0, w-j, h, x+j, y );
    zxSleep( 1 );
  }
}

void zxPixmapDrawBrindToLeft(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, j;

  for( i=step; i<d; i+=step ){
    for( j=w-i; j>=0; j-=d )
      zxPixmapDrawStrip( xc, pmap, mask, j, 0, step, h, x+j, y );
    if( j + step > 0 )
      zxPixmapDrawStrip( xc, pmap, mask, 0, 0, step+j, h, x, y );
    zxSleep( 1 );
  }
  if( ( step = i - d + 1 ) > 0 ){
    for( j=w-d; j>=0; j-=d )
      zxPixmapDrawStrip( xc, pmap, mask, j, 0, step, h, x+j, y );
    if( j + step > 0 )
      zxPixmapDrawStrip( xc, pmap, mask, 0, 0, step+j, h, x, y );
    zxSleep( 1 );
  }
}

void back(zxContents *xc, Pixmap bmap, int w, int h)
{
  zxWindowClear( xc );
  if( bmap != None )
    zxPixmapDraw( xc, bmap, 0, 0, w, h, 10, 10 );
  zxDrawRect( xc, 9, 9, w+1, h+1 );
}

int main(int argc, char *argv[])
{
  zxContents xc;
  zxImage img, bg;
  int w, h;
  Pixmap pmap, mask, bmap = None;

  zxInit();
  zxImageInit( &img );
  if( zxImageReadFile( &img, argv[1] ) == 0 ) exit( 1 );
  w = img.width;
  h = img.height;
  zxWindowCreateAndOpen( &xc, 0, 0, w+20, h+20 );
  pmap = zxImageCreatePixmap( &xc, &img );
  mask = zxImageCreatePixmapMask( &xc, &img );
  zxImageDestroy( &img );
  if( argc > 2 ){
    if( zxImageReadFile( &bg, argv[2] ) == 0 ) exit( 1 );
    bmap = zxImageCreatePixmap( &xc, &bg );
    zxImageDestroy( &bg );
  }
  zxSetColor( &xc, "red" );

  /* brind up */
  back( &xc, bmap, w, h );
  zxPixmapDrawBrindUp( &xc, pmap, mask, 10, 10, w, h, 25, 2 );
  getchar();

  /* brind down */
  back( &xc, bmap, w, h );
  zxPixmapDrawBrindDown( &xc, pmap, mask, 10, 10, w, h, 25, 2 );
  getchar();

  /* brind to right */
  back( &xc, bmap, w, h );
  zxPixmapDrawBrindToRight( &xc, pmap, mask, 10, 10, w, h, 25, 2 );
  getchar();

  /* brind to left */
  back( &xc, bmap, w, h );
  zxPixmapDrawBrindToLeft( &xc, pmap, mask, 10, 10, w, h, 25, 2 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

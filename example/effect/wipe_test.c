#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

void zxPixmapDrawWipeUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;

  for( i=h-step; i>0; i-=step ){
    zxPixmapDrawStrip( xc, pmap, mask, 0, i, w, step, x, y+i );
    zxSleep( 1 );
  }
  zxPixmapDrawStrip( xc, pmap, mask, 0, 0, w, i+step, x, y );
}

void zxPixmapDrawWipeDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;
  int y_max;

  y_max = h - step;
  for( i=0; i<y_max; i+=step ){
    zxPixmapDrawStrip( xc, pmap, mask, 0, i, w, step, x, y+i );
    zxSleep( 1 );
  }
  zxPixmapDrawStrip( xc, pmap, mask, 0, i, w, h-i, x, y+i );
}

void zxPixmapDrawWipeToRight(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;
  int x_max;

  x_max = w - step;
  for( i=0; i<x_max; i+=step ){
    zxPixmapDrawStrip( xc, pmap, mask, i, 0, step, h, x+i, y );
    zxSleep( 1 );
  }
  zxPixmapDrawStrip( xc, pmap, mask, i, 0, w-i, h, x+i, y );
}

void zxPixmapDrawWipeToLeft(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;

  for( i=w-step; i>0; i-=step ){
    zxPixmapDrawStrip( xc, pmap, mask, i, 0, step, h, x+i, y );
    zxSleep( 1 );
  }
  zxPixmapDrawStrip( xc, pmap, mask, 0, 0, i+step, h, x, y );
}

void zxPixmapDrawWipeInVert(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  int my;

  my = h / 2;
  for( i=0, j=h-step; i<=my; i+=step, j-=step ){
    zxPixmapDrawStrip( xc, pmap, mask, 0, i, w, step, x, y+i );
    zxPixmapDrawStrip( xc, pmap, mask, 0, j, w, step, x, y+j );
    zxSleep( 1 );
  }
}

void zxPixmapDrawWipeOutVert(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  int my;

  my = h / 2;
  for( i=0, j=h-step; i<=my; i+=step, j-=step );
  for( i-=step, j+=step; i>=0; i-=step, j+=step ){
    zxPixmapDrawStrip( xc, pmap, mask, 0, i, w, step, x, y+i );
    zxPixmapDrawStrip( xc, pmap, mask, 0, j, w, step, x, y+j );
    zxSleep( 1 );
  }
}

void zxPixmapDrawWipeInHoriz(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  int mx;

  mx = w / 2;
  for( i=0, j=w-step; i<=mx; i+=step, j-=step ){
    zxPixmapDrawStrip( xc, pmap, mask, i, 0, step, h, x+i, y );
    zxPixmapDrawStrip( xc, pmap, mask, j, 0, step, h, x+j, y );
    zxSleep( 1 );
  }
}

void zxPixmapDrawWipeOutHoriz(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  int mx;

  mx = w / 2;
  for( i=0, j=w-step; i<=mx; i+=step, j-=step );
  for( i-=step, j+=step; i>=0; i-=step, j+=step ){
    zxPixmapDrawStrip( xc, pmap, mask, i, 0, step, h, x+i, y );
    zxPixmapDrawStrip( xc, pmap, mask, j, 0, step, h, x+j, y );
    zxSleep( 1 );
  }
}

void zxPixmapDrawWipeCrossCW(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j, k, l;
  int w1, w2, h1, h2, dx, dy;
  int x_max, y_max;

  w1 = w / 2;
  w2 = w - w1;
  h1 = h / 2;
  h2 = h - h1;
  dx = step;
  dy = step * h / w;

  y_max = h - dy;
  x_max = w - dx;
  for( i=h1-dy, j=h1, k=w1, l=w1-dx;
       i>0 && j<y_max && k<x_max && l>0;
       i-=dy, j+=dy, k+=dx, l-=dx ){
    zxPixmapDrawStrip( xc, pmap, mask, 0, i, w1, dy, x, y+i );
    zxPixmapDrawStrip( xc, pmap, mask, w1, j, w2, dy, x+w1, y+j );
    zxPixmapDrawStrip( xc, pmap, mask, k, 0, dx, h1, x+k, y );
    zxPixmapDrawStrip( xc, pmap, mask, l, h1, dx, h2, x+l, y+h1 );
    zxSleep( 1 );
  }
  zxPixmapDrawStrip( xc, pmap, mask, 0, 0, w, h, x, y );
}

void zxPixmapDrawWipeCrossCCW(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j, k, l;
  int w1, w2, h1, h2, dx, dy;
  int x_max, y_max;

  w1 = w / 2;
  w2 = w - w1;
  h1 = h / 2;
  h2 = h - h1;
  dx = step;
  dy = step * h / w;

  y_max = h - dy;
  x_max = w - dx;
  for( i=h1-dy, j=h1, k=w1, l=w1-dx;
       i>0 && j<y_max && k<x_max && l>0;
       i-=dy, j+=dy, k+=dx, l-=dx ){
    zxPixmapDrawStrip( xc, pmap, mask, w1, i, w2, dy, x+w1, y+i );
    zxPixmapDrawStrip( xc, pmap, mask, 0, j, w1, dy, x, y+j );
    zxPixmapDrawStrip( xc, pmap, mask, k, h1, dx, h2, x+k, y+h1 );
    zxPixmapDrawStrip( xc, pmap, mask, l, 0, dx, h1, x+l, y );
    zxSleep( 1 );
  }
  zxPixmapDrawStrip( xc, pmap, mask, 0, 0, w, h, x, y );
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

  /* wipe up */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeUp( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* wipe down */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeDown( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* wipe to right */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeToRight( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* wipe to left */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeToLeft( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* vertically wipe in */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeInVert( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* vertically wipe out */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeOutVert( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* horizontally wipe in */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeInHoriz( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* horizontally wipe out */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeOutHoriz( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* clockwise wipe */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeCrossCW( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  /* counterclockwise wipe */
  back( &xc, bmap, w, h );
  zxPixmapDrawWipeCrossCCW( &xc, pmap, mask, 10, 10, w, h, 18 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

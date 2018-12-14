#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

void zxPixmapDrawCurtainUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, s, dy, sy, j;
  bool chk;

  for( s=d-step; ; s-=step ){
    for( chk=true, j=0, i=h-d;
         ( sy = i+(dy=s+j) ) >= 0; i-=d, j+=step )
      if( dy >= 0 ){
        chk = false;
        if( dy <= d )
          zxPixmapDrawStrip( xc, pmap, mask, 0, sy, w, step, x, y+sy );
      }
    if( dy <= d && sy > -step )
      zxPixmapDrawStrip( xc, pmap, mask, 0, 0, w, step+sy, x, y );
    if( chk ) break;
    zxSleep( 1 );
  }
}

void zxPixmapDrawCurtainDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, s, dy, sy, j;
  int y_max;
  bool chk;

  y_max = h - step;
  for( s=0; ; s+=step ){
    for( chk=true, j=0, i=0; (sy=i+(dy=s+j))<=y_max; i+=d, j-=step )
      if( dy <= d ){
        chk = false;
        if( dy >= 0 )
          zxPixmapDrawStrip( xc, pmap, mask, 0, sy, w, step, x, y+sy );
      }
    if( dy >= 0 && sy > y_max && sy < h )
      zxPixmapDrawStrip( xc, pmap, mask, 0, sy, w, h-sy, x, y+sy );
    if( chk ) break;
    zxSleep( 1 );
  }
}

void zxPixmapDrawCurtainToRight(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, s, dx, sx, j;
  int x_max;
  bool chk;

  x_max = w - step;
  for( s=0; ; s+=step ){
    for( chk=true, j=0, i=0; (sx=i+(dx=s+j))<=x_max; i+=d, j-=step )
      if( dx <= d ){
        chk = false;
        if( dx >= 0 )
          zxPixmapDrawStrip( xc, pmap, mask, sx, 0, step, h, x+sx, y );
      }
    if( dx >= 0 && sx > x_max && sx < w )
      zxPixmapDrawStrip( xc, pmap, mask, sx, 0, w-sx, h, x+sx, y );
    if( chk ) break;
    zxSleep( 1 );
  }
}

void zxPixmapDrawCurtainToLeft(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int d, int step)
{
  register int i, s, dx, sx, j;
  bool chk;

  for( s=d-step; ; s-=step ){
    for( chk=true, j=0, i=w-d; (sx=i+(dx=s+j))>=0; i-=d, j+=step )
      if( dx >= 0 ){
        chk = false;
        if( dx <= d )
          zxPixmapDrawStrip( xc, pmap, mask, sx, 0, step, h, x+sx, y );
      }
    if( dx <= d && sx > -step )
      zxPixmapDrawStrip( xc, pmap, mask, 0, 0, step+sx, h, x, y );
    if( chk ) break;
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

  /* curtain up */
  back( &xc, bmap, w, h );
  zxPixmapDrawCurtainUp( &xc, pmap, mask, 10, 10, w, h, 20, 2 );
  getchar();

  /* curtain down */
  back( &xc, bmap, w, h );
  zxPixmapDrawCurtainDown( &xc, pmap, mask, 10, 10, w, h, 20, 2 );
  getchar();

  /* curtain to right */
  back( &xc, bmap, w, h );
  zxPixmapDrawCurtainToRight( &xc, pmap, mask, 10, 10, w, h, 20, 2 );
  getchar();

  /* curtain to left */
  back( &xc, bmap, w, h );
  zxPixmapDrawCurtainToLeft( &xc, pmap, mask, 10, 10, w, h, 20, 2 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

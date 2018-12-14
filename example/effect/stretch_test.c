#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

void zxPixmapDrawStretchUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<h; i+=step ){
    for( j=1; j<=i; j++ )
      zxPixmapDrawCromachy( xc, pmap, mask, 0, h * (1-(double)j/i),
        bg, 0, h-j, w, 1, x, y+h-j );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
}

void zxPixmapDrawStretchDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<h; i+=step ){
    for( j=0; j<i; j++ )
      zxPixmapDrawCromachy( xc, pmap, mask, 0, h * (double)j/i,
        bg, 0, j, w, 1, x, y+j );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
}

void zxPixmapDrawStretchToRight(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<w; i+=step ){
    for( j=0; j<i; j++ )
      zxPixmapDrawCromachy( xc, pmap, mask, w * (double)j/i, 0,
        bg, j, 0, 1, h, x+j, y );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
}

void zxPixmapDrawStretchToLeft(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<w; i+=step ){
    for( j=0; j<i; j++ )
      zxPixmapDrawCromachy( xc, pmap, mask, w * (1-(double)j/i), 0,
        bg, w-j-1, 0, 1, h, x+w-j-1, y );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
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
  zxImage bg, img;
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

  /* stretch up */
  back( &xc, bmap, w, h );
  zxPixmapDrawStretchUp( &xc, pmap, mask, 10, 10, w, h, 50 );
  getchar();

  /* stretch down */
  back( &xc, bmap, w, h );
  zxPixmapDrawStretchDown( &xc, pmap, mask, 10, 10, w, h, 50 );
  getchar();

  /* stretch to right */
  back( &xc, bmap, w, h );
  zxPixmapDrawStretchToRight( &xc, pmap, mask, 10, 10, w, h, 50 );
  getchar();

  /* stretch to left */
  back( &xc, bmap, w, h );
  zxPixmapDrawStretchToLeft( &xc, pmap, mask, 10, 10, w, h, 50 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

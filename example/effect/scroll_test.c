#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

void zxPixmapDrawScrollUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<h; i+=step ){
    zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, h-i,
      w, i, x, y+h-i );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
}

void zxPixmapDrawScrollDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<h; i+=step ){
    zxPixmapDrawCromachy( xc, pmap, mask, 0, h-i, bg, 0, 0,
      w, i, x, y );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
}

void zxPixmapDrawScrollToRight(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<w; i+=step ){
    zxPixmapDrawCromachy( xc, pmap, mask, w-i, 0, bg, 0, 0,
      i, h, x, y );
    zxSleep( 1 );
  }
  zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, 0, 0, w, h, x, y );
  zxFlush();
  zxPixmapDestroy( bg );
}

void zxPixmapDrawScrollToLeft(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i;
  Pixmap bg;

  bg = zxPixmapClone( xc, zxCanvas(xc), x, y, w, h );
  for( i=step; i<w; i+=step ){
    zxPixmapDrawCromachy( xc, pmap, mask, 0, 0, bg, w-i, 0,
      i, h, x+w-i, y );
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

  /* scroll up */
  back( &xc, bmap, w, h );
  zxPixmapDrawScrollUp( &xc, pmap, mask, 10, 10, w, h, 20 );
  getchar();

  /* scroll down */
  back( &xc, bmap, w, h );
  zxPixmapDrawScrollDown( &xc, pmap, mask, 10, 10, w, h, 20 );
  getchar();

  /* scroll to right */
  back( &xc, bmap, w, h );
  zxPixmapDrawScrollToRight( &xc, pmap, mask, 10, 10, w, h, 20 );
  getchar();

  /* scroll to left */
  back( &xc, bmap, w, h );
  zxPixmapDrawScrollToLeft( &xc, pmap, mask, 10, 10, w, h, 20 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

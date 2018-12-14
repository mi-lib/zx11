#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

void zxPixmapDrawRollVert(zxWindow *win, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j, s;

  if( ( j = h ) % 2 == 0 ) j--;
  for( s=step, i=0; i<h; i+=2, j-=2 ){
    zxPixmapDrawStrip( win, pmap, mask, 0, i, w, 1, x, y+i );
    if( j >= 0 )
      zxPixmapDrawStrip( win, pmap, mask, 0, j, w, 1, x, y+j );
    if( --s == 0 ){
      zxSleep( 1 );
      s = step;
    }
  }
}

void zxPixmapDrawRollHoriz(zxWindow *win, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, j, s;

  if( ( j = w ) % 2 == 0 ) j--;
  for( s=step, i=0; i<w; i+=2, j-=2 ){
    zxPixmapDrawStrip( win, pmap, mask, i, 0, 1, h, x+i, y );
    if( j >= 0 )
      zxPixmapDrawStrip( win, pmap, mask, j, 0, 1, h, x+j, y );
    if( --s == 0 ){
      zxSleep( 1 );
      s = step;
    }
  }
}

void back(zxWindow *win, Pixmap bmap, int w, int h)
{
  zxWindowClear( win );
  if( bmap != None )
    zxPixmapDraw( win, bmap, 0, 0, w, h, 10, 10 );
  zxDrawRect( win, 9, 9, w+1, h+1 );
}

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img, bg;
  int w, h;
  Pixmap pmap, mask, bmap = None;

  zxInit();
  if( zxImageReadFile( &img, argv[1] ) == 0 ) exit( 1 );
  w = img.width;
  h = img.height;
  zxWindowCreateAndOpen( &win, 0, 0, w+20, h+20 );
  pmap = zxImageCreatePixmap( &win, &img );
  mask = zxImageCreatePixmapMask( &win, &img );
  zxImageDestroy( &img );
  if( argc > 2 ){
    if( zxImageReadFile( &bg, argv[2] ) == 0 ) exit( 1 );
    bmap = zxImageCreatePixmap( &win, &bg );
    zxImageDestroy( &bg );
  }
  zxSetColor( &win, "red" );

  /* vertical roll */
  back( &win, bmap, w, h );
  zxPixmapDrawRollVert( &win, pmap, mask, 10, 10, w, h, 3 );
  getchar();

  back( &win, bmap, w, h );
  zxPixmapDrawRollVert( &win, pmap, mask, 10, 10, w, h, 10 );
  getchar();

  /* horizontal roll */
  back( &win, bmap, w, h );
  zxPixmapDrawRollHoriz( &win, pmap, mask, 10, 10, w, h, 3 );
  getchar();

  back( &win, bmap, w, h );
  zxPixmapDrawRollHoriz( &win, pmap, mask, 10, 10, w, h, 10 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

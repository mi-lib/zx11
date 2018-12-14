#include <unistd.h>
#include <sys/param.h>
#include <zximage.h>

#define zxSleep(t) usleep( (t) * 1000000 / HZ )

#define __zx_pixmap_draw_split_align(x0,w,step,x,b) {\
  if( (x0) <=-(step) || (x0) >= (w) ) continue;\
  if( (x0) < 0 ){\
    (x) = 0;\
    (b) = (step) + (x0);\
  } else{\
    (x) = (x0);\
    (b) = (step);\
  }\
  if( (x) + (b) > (w) ) (b) = (w) - (x);\
}

#define __zx_pixmap_draw_split_alloc_buf(b,n) \
  if( !( (b) = zAlloc( int, (n) ) ) ){\
    ZALLOCERROR( "segments" );\
    return;\
  }

void zxPixmapDrawSplitRightUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, b;
  int *x0, x1;

  __zx_pixmap_draw_split_alloc_buf( x0, h );
  for( i=h-1; i>=0; i-- ) x0[i] = i - h;

  while( x0[0] < w ){
    for( i=h-1; i>=0; x0[i--] += step ){
      __zx_pixmap_draw_split_align( x0[i], w, step, x1, b );
      zxPixmapDrawStrip( xc, pmap, mask, x1, i, b, 1, x+x1, y+i );
    }
    zxSleep( 1 );
  }
  zFree( x0 );
}

void zxPixmapDrawSplitRightDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, b;
  int *x0, x1;

  __zx_pixmap_draw_split_alloc_buf( x0, h );
  for( i=0; i<h; i++ ) x0[i] = -i;

  while( x0[h-1] < w ){
    for( i=0; i<h; x0[i++] += step ){
      __zx_pixmap_draw_split_align( x0[i], w, step, x1, b );
      zxPixmapDrawStrip( xc, pmap, mask, x1, i, b, 1, x+x1, y+i );
    }
    zxSleep( 1 );
  }
  zFree( x0 );
}

void zxPixmapDrawSplitLeftUp(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, b;
  int *x0, x1;

  __zx_pixmap_draw_split_alloc_buf( x0, h );
  for( i=0; i<h; i++ ) x0[i] = w + h - i;

  while( x0[0] >= -step ){
    for( i=h-1; i>=0; x0[i--] -= step ){
      __zx_pixmap_draw_split_align( x0[i], w, step, x1, b );
      zxPixmapDrawStrip( xc, pmap, mask, x1, i, b, 1, x+x1, y+i );
    }
    zxSleep( 1 );
  }
  zFree( x0 );
}

void zxPixmapDrawSplitLeftDown(zxContents *xc, Pixmap pmap, Pixmap mask, int x, int y, int w, int h, int step)
{
  register int i, b;
  int *x0, x1;

  __zx_pixmap_draw_split_alloc_buf( x0, h );
  for( i=0; i<h; i++ ) x0[i] = w + i - step;

  while( x0[h-1] >= -step ){
    for( i=0; i<h; x0[i++] -= step ){
      __zx_pixmap_draw_split_align( x0[i], w, step, x1, b );
      zxPixmapDrawStrip( xc, pmap, mask, x1, i, b, 1, x+x1, y+i );
    }
    zxSleep( 1 );
  }
  zFree( x0 );
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

  /* split right up */
  back( &xc, bmap, w, h );
  zxPixmapDrawSplitRightUp( &xc, pmap, mask, 10, 10, w, h, 60 );
  getchar();

  /* split right down */
  back( &xc, bmap, w, h );
  zxPixmapDrawSplitRightDown( &xc, pmap, mask, 10, 10, w, h, 60 );
  getchar();

  /* split left up */
  back( &xc, bmap, w, h );
  zxPixmapDrawSplitLeftUp( &xc, pmap, mask, 10, 10, w, h, 60 );
  getchar();

  /* split left down */
  back( &xc, bmap, w, h );
  zxPixmapDrawSplitLeftDown( &xc, pmap, mask, 10, 10, w, h, 60 );
  getchar();

  zxPixmapDestroy( pmap );
  if( mask != None ) zxPixmapDestroy( mask );
  if( bmap != None ) zxPixmapDestroy( bmap );
  zxClose();

  return 0;
} 

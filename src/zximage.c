/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage - image manipulation on X11
 */

#include <zx11/zximage.h>
#include <math.h>

#include <zx11/zximage_supported.h>

/* ********************************************************** */
/* CLASS: zxImage
 * image buffer
 * ********************************************************** */

/* pixel manipulation */

zxPixel zxImageCellPixel(zxImage *img, uint x, uint y)
{
  zxPixel pixel = 0;

  memcpy( &pixel, zxImageAddr(img,x,y), img->bpp );
  return pixel;
}

zxPixel zxImageCellPixelCheck(zxImage *img, uint x, uint y)
{
  return zxImagePosIsValid( img, x, y ) ?
    zxImageCellPixel( img, x, y ) : 0;
}

void zxImageCellFromPixel(zxImage *img, uint x, uint y, zxPixel pixel)
{
  memcpy( zxImageAddr(img,x,y), &pixel, img->bpp );
}

void zxImageCellFromPixelCheck(zxImage *img, uint x, uint y, zxPixel pixel)
{
  if( zxImagePosIsValid( img, x, y ) )
    zxImageCellFromPixel( img, x, y, pixel );
}

ubyte zxImageBPP(ubyte depth)
{
  switch( depth ){
  case 8:
    return sizeof(ubyte);
  case 16:
    return sizeof(uword);
  case 24: case 32:
    return sizeof(uint32_t);
  default:
    ZRUNERROR( "unsupported color depth %d", depth );
    return 0;
  }
}

/* buffer manipulation */

zxImage *zxImageInit(zxImage *img)
{
  img->width = img->height = 0;
  img->bpp = 0;
  img->buf = NULL;
  img->mask_buf = NULL;
  return img;
}

zxImage *zxImageAlloc(zxImage *img, uint width, uint height, ubyte depth)
{
  img->width = width;
  img->height = height;
  img->mask_buf = NULL;
  if( ( img->bpp = zxImageBPP( depth ) ) == 0 ) return NULL;
  if( !( img->buf = zAlloc( ubyte, img->width*img->height*img->bpp ) ) ){
    ZRUNERROR( "cannot allocate enough memory for image buffer" );
    return NULL;
  }
  return img;
}

zxImage *zxImageAllocMask(zxImage *img)
{
  if( !( img->mask_buf = zAlloc( ubyte, img->width*img->height ) ) ){
    ZRUNERROR( "cannot allocate enough memory for mask buffer" );
    return NULL;
  }
  return img;
}

zxImage *zxImageCreateMask(zxImage *img, zxPixel mask)
{
  uint i, j;

  if( !zxImageAllocMask( img ) ) return NULL;
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      if( zxImageCellPixel( img, j, i ) == mask )
        zxImageSetMask( img, j, i, 0xff );
  return img;
}

zxImage *zxImageDestroy(zxImage *img)
{
  free( img->buf );
  zFree( img->mask_buf );
  return zxImageInit( img );
}

zxImage *zxImageClear(zxImage *img)
{
  memset( img->buf, 0, img->width*img->height*img->bpp );
  if( img->mask_buf )
    memset( img->mask_buf, 0, img->width*img->height );
  return img;
}

zxImage *zxImageFill(zxImage *img, zxPixel pixel)
{
  uint i, j;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      zxImageCellFromPixel( img, j, i, pixel );
  return img;
}

int zxImageSizeEqual(zxImage *i1, zxImage *i2)
{
  if( i1->width  != i2->width ||
      i1->height != i2->height ||
      i1->bpp    != i2->bpp ){
    ZRUNERROR( "size mismatch of two image buffers" );
    return 0;
  }
  return 1;
}

zxImage *zxImageCopy(zxImage *src, zxImage *dest)
{
  memcpy( dest->buf, src->buf, src->width*src->height*src->bpp );
  return dest;
}

zxImage *zxImageClone(zxImage *src, zxImage *dest)
{
  if( !zxImageAlloc( dest, src->width, src->height, src->bpp<<3 ) )
    return NULL;
  zxImageCopy( src, dest );
  if( !src->mask_buf ) return dest;
  /* mask cloning */
  if( !zxImageAllocMask( dest ) ) return NULL;
  memcpy( dest->mask_buf, src->mask_buf, src->width*src->height );
  return dest;
}

/* image pasting */

void zxImageCanvasRange(zxImage *canvas, zxImage *img, uint x, uint y, uint *w, uint *h)
{
  uint dx, dy;

  dx = x >= canvas->width  ? 0 : canvas->width  - x;
  dy = y >= canvas->height ? 0 : canvas->height - y;
  *w = zMin( dx, img->width );
  *h = zMin( dy, img->height );
}

zxImage *zxImagePut(zxImage *canvas, zxImage *img, uint x, uint y)
{
  int w, h;

  zxImageCanvasRange( canvas, img, x, y, (uint *)&w, (uint *)&h );
  for( h--; h>=0; h-- )
    memcpy( zxImageAddr( canvas, x, y+h ),
      zxImageAddr( img, 0, h ), w*canvas->bpp );
  return canvas;
}

zxImage *zxImagePutMasked(zxImage *canvas, zxImage *img, uint x, uint y, zxPixel mask)
{
  uint i, j;
  uint w, h;
  zxPixel pixel;

  zxImageCanvasRange( canvas, img, x, y, &w, &h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      if( ( pixel = zxImageCellPixel( img, j, i ) ) != mask )
        zxImageCellFromPixel( canvas, x+j, y+i, pixel );
  return canvas;
}

zxImage *zxImagePutAlphaBlend(zxImage *canvas, zxImage *img1, zxImage *img2, uint x, uint y, double alpha)
{
  uint i, j;
  uint w, h;
  zxPixelManip pm;

  if( !zxImageSizeEqual( img1, img2 ) ) return NULL;
  zxPixelManipSetDefault( &pm );
  zxImageCanvasRange( canvas, img1, x, y, &w, &h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( canvas, x+j, y+i,
        zxPixelAlphaBlend( &pm, zxImageCellPixel( img1, j, i ),
          zxImageCellPixel( img2, j, i ), alpha ) );
  return canvas;
}

zxImage *zxImagePutAlphaBlendMasked(zxImage *canvas, zxImage *img1, zxImage *img2, uint x, uint y, double alpha, zxPixel mask)
{
  uint i, j;
  uint w, h;
  zxPixel p1, p2;
  zxPixelManip pm;

  if( !zxImageSizeEqual( img1, img2 ) ) return NULL;
  zxPixelManipSetDefault( &pm );
  zxImageCanvasRange( canvas, img1, x, y, &w, &h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      p1 = zxImageCellPixel( img1, j, i );
      p2 = zxImageCellPixel( img2, j, i );
      zxImageCellFromPixel( canvas, x+j, y+i, p2 == mask ?
        p1 : zxPixelAlphaBlend( &pm, p1, p2, alpha ) );
    }
  return canvas;
}

zxImage *zxImagePutSuperimpose(zxImage *canvas, zxImage *img, uint x, uint y)
{
  uint i, j;
  uint w, h;
  zxPixel p1, p2;
  zxPixelManip pm;
  double alpha;

  if( !img->mask_buf )
    return zxImagePut( canvas, img, 0, 0 );
  zxImageCanvasRange( canvas, img, x, y, &w, &h );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      p1 = zxImageCellPixel( canvas, x+j, y+i );
      p2 = zxImageCellPixel( img, j, i );
      alpha = (double)zxImageMask( img, j, i ) / 0xff;
      zxImageCellFromPixel( canvas, x+j, y+i,
        zxPixelAlphaBlend( &pm, p1, p2, alpha ) );
    }
  return canvas;
}

zxImage *zxImageGet(zxImage *src, zxImage *dest, uint x, uint y)
{
  int w, h;

  zxImageCanvasRange( src, dest, x, y, (uint *)&w, (uint *)&h );
  for( h--; h>=0; h-- )
    memcpy( zxImageAddr( dest, 0, h ),
      zxImageAddr( src, x, y+h ), w*dest->bpp );
  return dest;
}

/* image manipulation */

zxImage *zxImageVertFlip(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i),
        zxImageAddr(src,w-j-1,i), src->bpp );
  return dest;
}

zxImage *zxImageHorizFlip(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i),
        zxImageAddr(src,j,src->height-i-1), src->bpp );
  return dest;
}

zxImage *zxImageRotRight(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  w = zMin( src->height, dest->width );
  h = zMin( src->width, dest->height );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i),
        zxImageAddr(src,i,src->height-j-1), src->bpp );
  return dest;
}

zxImage *zxImageRotLeft(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  w = zMin( src->height, dest->width );
  h = zMin( src->width, dest->height );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i),
        zxImageAddr(src,src->width-i-1,j), src->bpp );
  return dest;
}

zxImage *zxImageRot(zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double theta)
{
  uint i, j, k;
  double x0, x1, x2, y0, y1, y2;
  uint px, py;
  double ct, st;

  ct = cos( theta );
  st = sin( theta );
  x0 = ox - (double)x*ct - (double)y*st;
  y0 = oy + (double)x*st - (double)y*ct;

  for( i=0; i<dest->height; i++ ){
    x1 = x0 + (double)i*st;
    y1 = y0 + (double)i*ct;
    x2 = x1 + (double)(dest->width-1)*ct;
    y2 = y1 - (double)(dest->width-1)*st;
    for( j=0; j<dest->width; j++ ){
      k = dest->width - j;
      px = ( x1 * k + x2 * j ) / dest->width;
      py = ( y1 * k + y2 * j ) / dest->width;
      if( px >=0 && py >= 0 && zxImagePosIsValid( src, px, py ) )
        memcpy( zxImageAddr(dest,j,i),
          zxImageAddr(src,px,py), src->bpp );
      else
        memset( zxImageAddr(dest,j,i), 0, src->bpp );
    }
  }
  return dest;
}

zxImage *zxImageRotFilt(zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double theta)
{
  uint i, j, k;
  double x0, x1, x2, y0, y1, y2, px[3], py[3];
  double d[4], w[4], wt;
  double ct, st;
  zxPixel p[4];
  zxPixelManip pm;

  ct = cos( theta );
  st = sin( theta );
  x0 = ox - (double)x*ct - (double)y*st;
  y0 = oy + (double)x*st - (double)y*ct;

  zxPixelManipSetDefault( &pm );
  for( i=0; i<dest->height; i++ ){
    x1 = x0 + (double)i*st;
    y1 = y0 + (double)i*ct;
    x2 = x1 + (double)(dest->width-1)*ct;
    y2 = y1 - (double)(dest->width-1)*st;
    for( j=0; j<dest->width; j++ ){
      k = dest->width - j;
      px[0] = ( x1 * k + x2 * j ) / dest->width;
      py[0] = ( y1 * k + y2 * j ) / dest->width;
      if( px[0] >=0 && py[0] >= 0 &&
          zxImagePosIsValid( src, px[0], py[0] ) ){
        /* blending */
        px[2] = ( px[1] = floor( px[0] ) ) + 1;
        py[2] = ( py[1] = floor( py[0] ) ) + 1;
        d[0] = px[0] - px[1]; d[0] = d[0]*d[0];
        d[1] = px[0] - px[2]; d[1] = d[1]*d[1];
        d[2] = py[0] - py[1]; d[2] = d[2]*d[2];
        d[3] = py[0] - py[2]; d[3] = d[3]*d[3];
        w[0] = 1.0/sqrt( d[0] + d[2] );
        w[1] = 1.0/sqrt( d[1] + d[2] );
        w[2] = 1.0/sqrt( d[0] + d[3] );
        w[3] = 1.0/sqrt( d[1] + d[3] );
        wt = w[0] + w[1] + w[2] + w[3];
        w[0] /= wt;
        w[1] /= wt;
        w[2] /= wt;
        w[3] /= wt;
        p[0] = zxImageCellPixel( src, px[1], py[1] );
        p[1] = zxImageCellPixel( src, px[2], py[1] );
        p[2] = zxImageCellPixel( src, px[1], py[2] );
        p[3] = zxImageCellPixel( src, px[2], py[2] );
        zxImageCellFromPixel( dest, j, i,
          zxPixelBlend( &pm, p, w, 4 ) );
      } else
        memset( zxImageAddr(dest,j,i), 0, src->bpp );
    }
  }
  return dest;
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

static double __zx11_resize_filter(double x, double y);
double __zx11_resize_filter(double x, double y){
  if( x == 0 )
    x = 1.0;
  else{
    x *= M_PI;
    x = sin(x)/x;
  }
  if( y == 0 )
    y = 1.0;
  else {
    y *= M_PI;
    y = sin(y)/y;
  }
  return x * x * y * y;
}

zxImage *zxImageResize(zxImage *src, zxImage *dest)
{
  uint i, j;
  ubyte rs, gs, bs;
  double k, l;
  double x0, xs, xe, y0, ys, ye, xl, yl, xrate, yrate;
  double u, ut, r, g, b;
  zxPixelManip pm;

  zxPixelManipSetDefault( &pm );
  xrate = (double)src->width / dest->width;
  yrate = (double)src->height / dest->height;
  for( i=0; i<dest->height; i++ ){
    if( ( y0 = i * yrate ) > src->height ) break;
    ys = floor( y0 - yrate );     ys = zMax( ys, 0 );
    ye = floor( y0 + yrate ) + 1; ye = zMin( ye, src->height );
    for( j=0; j<dest->width; j++ ){
      if( ( x0 = j * xrate ) > src->width ) break;
      xs = floor( x0 - xrate );     xs = zMax( xs, 0 );
      xe = floor( x0 + xrate ) + 1; xe = zMin( xe, src->width );
      for( r=g=b=0, ut=0, k=ys; k<ye; k+=1.0 ){
        yl = ( y0 - k ) / yrate;
        for( l=xs; l<xe; l+=1.0 ){
          xl = ( x0 - l ) / xrate;
          zxImageCellRGB( src, &pm, l, k, &rs, &gs, &bs );
          u = __zx11_resize_filter( xl, yl );
          r += u * rs; g += u * gs; b += u * bs;
          ut += u;
        }
      }
      r /= ut; g /= ut; b /= ut;
      zxImageCellFromRGB( dest, &pm, j, i, r, g, b );
    }
  }
  return dest;
}

/* color manipulation */

zxImage *zxImageAbstRGB(zxImage *src, zxImage *rimg, zxImage *gimg, zxImage *bimg)
{
  uint i, j;
  ubyte r, g, b;
  zxPixelManip pm;

  zxPixelManipSet( &pm, zxdepth );
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellRGB( src, &pm, j, i, &r, &g, &b );
      if( rimg && zxImagePosIsValid( rimg, j, i ) )
        zxImageCellFromRGB( rimg, &pm, j, i, r, 0, 0 );
      if( gimg && zxImagePosIsValid( gimg, j, i ) )
        zxImageCellFromRGB( gimg, &pm, j, i, 0, g, 0 );
      if( bimg && zxImagePosIsValid( bimg, j, i ) )
        zxImageCellFromRGB( bimg, &pm, j, i, 0, 0, b );
    }
  return src;
}

zxImage *zxImageGrayscalize(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  ubyte r, g, b;
  zxPixelManip pm;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      zxImageCellRGB( src, &pm, j, i, &r, &g, &b );
      r = ( r + g + b ) / 3;
      zxImageCellFromGS( dest, &pm, j, i, r );
    }
  return dest;
}

zxImage *zxImageNegate(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  zxPixelManip pm;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( dest, j, i, zxImageCellNegate( src, &pm, j, i ) );
  return dest;
}

zxImage *zxImageToneDown(zxImage *src, zxImage *dest, double rate)
{
  uint i, j;
  ubyte r, g, b;
  zxPixelManip pm;

  rate = zLimit( rate, 0, 1 );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellRGB( src, &pm, j, i, &r, &g, &b );
      r *= rate; g *= rate; b *= rate;
      zxImageCellFromRGB( dest, &pm, j, i, r, g, b );
    }
  return dest;
}

static void _zxImageHistogram(zxImage *img, zxPixelManip *pm, uint h[], double sh[])
{
  uint i, j;
  ubyte r, g, b;
  double d;

  memset( h, 0, sizeof(uint)*0x100 );
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      zxImageCellRGB( img, pm, j, i, &r, &g, &b );
      h[(int)(r+g+b)/3]++;
    }
  d = 1.0 / ( img->width * img->height );
  for( sh[0]=d*h[0], i=1; i<=0xff; i++ )
    sh[i] = sh[i-1] + d * h[i];
}

zxImage *zxImageEqualize(zxImage *src, zxImage *dest)
{
  uint x, y, i;
  uint w, h;
  uint *hi;
  double *sh, vmin, scalefactor;
  ubyte r, g, b;
  zxPixelManip pm;

  hi = zAlloc( uint, 0x100 );
  sh = zAlloc( double, 0x100 );
  if( !hi || !sh ){
    dest = NULL;
    goto TERMINATE;
  }
  zxPixelManipSetDefault( &pm );
  _zxImageHistogram( src, &pm, hi, sh );
  for( vmin=0, i=0; i<=0xff; i++ )
    if( sh[i] > 0 ){
      vmin = sh[i];
      break;
    }
  scalefactor = (double)0xff / ( 1.0 - vmin );
  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  for( y=0; y<h; y++ ){
    for( x=0; x<w; x++ ){
      zxImageCellRGB( src, &pm, x, y, &r, &g, &b );
      r = zLimit( (int)( scalefactor * ( sh[r] - vmin ) ), 0, 0xff );
      g = zLimit( (int)( scalefactor * ( sh[g] - vmin ) ), 0, 0xff );
      b = zLimit( (int)( scalefactor * ( sh[b] - vmin ) ), 0, 0xff );
      zxImageCellFromRGB( dest, &pm, x, y, r, g, b );
    }
  }
 TERMINATE:
  free( hi );
  free( sh );
  return dest;
}

zxImage *zxImageDither(zxImage *src, zxImage *dest)
{
  static double bayerpattern[] = {
     0,  8,  2, 10,
    12,  4, 14,  6,
     3, 11,  1,  9,
    15,  7, 13,  5 };
  uint i, j;
  uint w, h;
  zxPixelManip pm;
  ubyte r, g, b, th;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      th = bayerpattern[ (i%4)*4 + (j%4) ]*16 + 8;
      zxImageCellRGB( src, &pm, j, i, &r, &g, &b );
      zxImageCellFromRGB( dest, &pm, j, i,
        r >= th ? 0xff : 0,
        g >= th ? 0xff : 0,
        b >= th ? 0xff : 0 );
    }
  return dest;
}

/* general filter */

static void _zxImageFilterPickPixel(zxImage *src, int x, int y, int w, int h, int size, zxPixel p[], uint sh)
{
  int i, j, k, sx, sy;

  for( k=0, i=0; i<size; i++ ){
    sy = zLimit( y + i - sh, 0, w-1 );
    for( j=0; j<size; j++ ){
      sx = zLimit( x + j - sh, 0, h-1 );
      p[k++] = zxImageCellPixel( src, sx, sy );
    }
  }
}

zxImage *zxImageFilter(zxImage *src, zxImage *dest, double f[], int size)
{
  uint x, y;
  uint w, h, s2, sh;
  zxPixel *p;
  zxPixelManip pm;

  if( !( p = zAlloc( zxPixel, ( s2 = size * size ) ) ) ) goto TERMINATE;
  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  sh = ( size - 1 ) / 2;
  for( y=0; y<h; y++ )
    for( x=0; x<w; x++ ){
      _zxImageFilterPickPixel( src, x, y, w, h, size, p, sh );
      zxImageCellFromPixel( dest, x, y,
        zxPixelBlend( &pm, p, f, s2 ) );
    }
 TERMINATE:
  free( p );
  return dest;
}

zxImage *zxImageFilter2(zxImage *src, zxImage *dest, double f1[], double f2[], int size)
{
  uint x, y;
  uint w, h, s2, sh;
  zxPixel *p, p1, p2;
  zxPixelManip pm;

  if( !( p = zAlloc( zxPixel, ( s2 = size * size ) ) ) ) goto TERMINATE;
  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  sh = ( size - 1 ) / 2;
  for( y=0; y<h; y++ )
    for( x=0; x<w; x++ ){
      _zxImageFilterPickPixel( src, x, y, w, h, size, p, sh );
      p1 = zxPixelBlend( &pm, p, f1, s2 );
      p2 = zxPixelBlend( &pm, p, f2, s2 );
      zxImageCellFromPixel( dest, x, y, zxPixelNorm2( &pm, p1, p2 ) );
    }
 TERMINATE:
  free( p );
  return dest;
}

/* blur */

static int _zx11_median_cmp(void *v1, void *v2, void *dummy)
{
  if( *(ubyte*)v1 > *(ubyte*)v2 ) return 1;
  if( *(ubyte*)v1 < *(ubyte*)v2 ) return -1;
  return 0;
}

static void _zxImageMedianFind(zxImage *img, zxPixelManip *pm, int j, int i, ubyte *rs, ubyte *gs, ubyte *bs, uint size, ubyte *r, ubyte *g, ubyte *b)
{
  uint _i, _j, si, sj, k;
  int s2, sh;

  s2 = size * size;
  sh = ( size - 1 ) / 2;
  for( _i=0; _i<size; _i++ )
    for( _j=0; _j<size; _j++ ){
      k = _i*size + _j;
      if( zxImagePosIsValid( img, ( sj = j+_j-sh ), ( si = i+_i-sh ) ) ){
        zxImageCellRGB( img, pm, sj, si, r, g, b );
      } else
        *r = *g = *b = 0;
      zInsertSort( rs, r, k, s2, sizeof(ubyte), _zx11_median_cmp, NULL );
      zInsertSort( gs, g, k, s2, sizeof(ubyte), _zx11_median_cmp, NULL );
      zInsertSort( bs, b, k, s2, sizeof(ubyte), _zx11_median_cmp, NULL );
    }
  sh = ( s2 - 1 ) / 2;
  *r = rs[sh];
  *g = gs[sh];
  *b = bs[sh];
}

zxImage *zxImageMedian(zxImage *src, zxImage *dest, int size)
{
  uint i, j;
  uint w, h;
  zxPixelManip pm;
  ubyte *rs, *gs, *bs, r, g, b;

  rs = zAlloc( ubyte, size * size );
  gs = zAlloc( ubyte, size * size );
  bs = zAlloc( ubyte, size * size );
  if( !rs || !gs || !bs ) goto TERMINATE;
  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSet( &pm, zxdepth );
  for( i=0; i<h; i++ ){
    for( j=0; j<w; j++ ){
      _zxImageMedianFind( src, &pm, j, i, rs, gs, bs, size, &r, &g, &b );
      zxImageCellFromRGB( dest, &pm, j, i, r, g, b );
    }
  }
 TERMINATE:
  free( rs );
  free( gs );
  free( bs );
  return dest;
}

zxImage *zxImageAntialias(zxImage *src, zxImage *dest)
{
  double weight[] = {
    0.041667, 0.083333, 0.041667,
    0.083333, 0.5,      0.083333,
    0.041667, 0.083333, 0.041667,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

zxImage *zxImageGaussian(zxImage *src, zxImage *dest)
{
  double weight[] = {
    0.0625, 0.1250, 0.0625,
    0.1250, 0.2500, 0.1250,
    0.0625, 0.1250, 0.0625,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* edge detection */

zxImage *zxImageDiff(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  ubyte r, g, b, pr, pg, pb;
  zxPixelManip pm;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<h; i++ ){
    zxImageCellRGB( src, &pm, 0, i, &pr, &pg, &pb );
    zxImageCellFromRGB( dest, &pm, 0, i, pr, pg, pb );
    for( j=1; j<w; j++ ){
      zxImageCellRGB( src, &pm, j, i, &r, &g, &b );
      zxImageCellFromRGB( dest, &pm, j, i, r-pr, g-pg, b-pb );
      pr = r; pg = g; pb = b;
    }
  }
  return dest;
}

zxImage *zxImageIntegral(zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  ubyte r, g, b, ri, gi, bi;
  zxPixelManip pm;

  zxImageCanvasRange( dest, src, 0, 0, &w, &h );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<h; i++ ){
    zxImageCellRGB( src, &pm, 0, i, &ri, &gi, &bi );
    memcpy( zxImageAddr(dest,0,i), zxImageAddr(src,0,i), dest->bpp );
    for( j=1; j<w; j++ ){
      zxImageCellRGB( src, &pm, j, i, &r, &g, &b );
      ri += r; gi += g; bi += b;
      zxImageCellFromRGB( dest, &pm, j, i, ri, gi, bi );
    }
  }
  return dest;
}

zxImage *zxImagePrewittH(zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0, 0.0, 1.0,
   -1.0, 0.0, 1.0,
   -1.0, 0.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

zxImage *zxImagePrewittV(zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0,-1.0,-1.0,
    0.0, 0.0, 0.0,
    1.0, 1.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

zxImage *zxImagePrewitt(zxImage *src, zxImage *dest)
{
  double weight1[] = {
   -1.0, 0.0, 1.0,
   -1.0, 0.0, 1.0,
   -1.0, 0.0, 1.0,
  };
  double weight2[] = {
   -1.0,-1.0,-1.0,
    0.0, 0.0, 0.0,
    1.0, 1.0, 1.0,
  };
  return zxImageFilter2( src, dest, weight1, weight2, 3 );
}

zxImage *zxImageSobelH(zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0, 0.0, 1.0,
   -2.0, 0.0, 2.0,
   -1.0, 0.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

zxImage *zxImageSobelV(zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0,-2.0,-1.0,
    0.0, 0.0, 0.0,
    1.0, 2.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

zxImage *zxImageSobel(zxImage *src, zxImage *dest)
{
  double weight1[] = {
   -1.0, 0.0, 1.0,
   -2.0, 0.0, 2.0,
   -1.0, 0.0, 1.0,
  };
  double weight2[] = {
   -1.0,-2.0,-1.0,
    0.0, 0.0, 0.0,
    1.0, 2.0, 1.0,
  };
  return zxImageFilter2( src, dest, weight1, weight2, 3 );
}

zxImage *zxImageLaplacian(zxImage *src, zxImage *dest)
{
  double weight[] = {
    1.0, 1.0, 1.0,
    1.0,-8.0, 1.0,
    1.0, 1.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* special effect */

zxPixel zxImageCellAverage(zxImage *img, zxPixelManip *pm, uint x, uint y, uint w, uint h)
{
  uint i, j;
  zxPixel rl, gl, bl, r, g, b;
  ubyte rd, gd, bd;

  r = g = b = 0;
  for( i=0; i<h; i++ ){
    rl = gl = bl = 0;
    for( j=0; j<w; j++ ){
      zxImageCellRGB( img, pm, x+j, y+i, &rd, &gd, &bd );
      rl += rd; gl += gd; bl += bd;
    }
    r += rl / w;
    g += gl / w;
    b += bl / w;
  }
  r /= h;
  g /= h;
  b /= h;
  return pm->PixelFromRGB( r, g, b );
}

zxImage *zxImageAverage(zxImage *img, zxPixelManip *pm, uint x, uint y, uint w, uint h)
{
  uint i, j;
  zxPixel p;

  w = zMin( img->width - x, w );
  h = zMin( img->height - y, h );
  p = zxImageCellAverage( img, pm, x, y, w, h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( img, x+j, y+i, p );
  return img;
}

zxImage *zxImageMosaic(zxImage *img, uint x, uint y, uint w, uint h, uint nx, uint ny)
{
  uint i, j, dx, dy, mx, my;
  zxPixelManip pm;

  dx = w / nx;
  dy = h / ny;
  mx = zMin( x+w, img->width );
  my = zMin( y+h, img->height );
  zxPixelManipSetDefault( &pm );
  for( i=y; i<my; i+=dy )
    for( j=x; j<mx; j+=dx )
      zxImageAverage( img, &pm, j, i, dx, dy );
  return img;
}

/* drawing */

Pixmap zxImageToPixmap(zxWindow *win, zxImage *img, Pixmap pmap, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y)
{
  XImage *ip;

  if( ( ip = XCreateImage( zxdisplay, CopyFromParent, zxdepth, ZPixmap,
          16, (char *)img->buf, img->width, img->height,
          img->bpp<<3, img->bpp*img->width ) ) ){
    XPutImage( zxdisplay, pmap, zxWindowGC(win), ip, src_x, src_y, dest_x, dest_y, w, h );
    free( ip );
  }
  return pmap;
}

Pixmap zxImageCreatePixmap(zxWindow *win, zxImage *img)
{
  Pixmap pmap;

  if( ( pmap = zxPixmapCreate( win, img->width, img->height ) ) == None )
    return None;
  return zxImageToPixmapAll( win, img, pmap );
}

Pixmap zxImageCreatePixmapMask(zxWindow *win, zxImage *img)
{
  uint x, y, z;
  uint bytes_per_row;
  ubyte *buf, flag;
  Pixmap mask;

  if( !img->mask_buf ) return None;
  bytes_per_row = img->width / 8 + ( ( img->width % 8 ) ? 1 : 0 );
  if( !( buf = zAlloc( ubyte, bytes_per_row*img->height ) ) )
    return None;
  for( y=0; y<img->height; y++ )
    for( z=bytes_per_row*y, flag=0x1, x=0; x<img->width; x++ ){
      if( zxImageMask( img, x, y ) )
        buf[z] |= flag;
      if( ( flag <<= 1 ) == 0 ){
        flag = 0x1;
        z++;
      }
    }
  mask = XCreateBitmapFromData( zxdisplay,
    zxWindowBody(win), (char *)buf, img->width, img->height );
  free( buf );
  return mask;
}

void zxImageDraw(zxWindow *win, zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y)
{
  Pixmap mask;

  if( ( mask = zxImageCreatePixmapMask( win, img ) ) != None ){
    zxClipSetOrigin( win, dest_x - src_x, dest_y - src_y );
    zxClipSetMask( win, mask );
  }
  zxImageToPixmap( win, img, zxWindowCanvas(win), src_x, src_y, w, h, dest_x, dest_y );
  if( mask != None ){
    zxClipUnsetMask( win );
    zxPixmapDestroy( mask );
  }
}

bool zxImageDrawMask(zxWindow *win, zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y)
{
  zxImage mask;
  zxPixelManip pm;
  uint i, j;
  ubyte val, *mp;

  if( !( mp = img->mask_buf ) ) return false;
  if( !zxImageAllocDefault( &mask, img->width, img->height ) ) return false;
  zxPixelManipSetDefault( &pm );
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      val = *mp++;
      zxImageCellFromRGB( &mask, &pm, j, i, val, val, val );
    }
  zxImageToPixmap( win, &mask, zxWindowCanvas(win), src_x, src_y, w, h, dest_x, dest_y );
  zxImageDestroy( &mask );
  return true;
}

zxImage *zxImageFromPixmap(zxImage *img, Pixmap pmap, uint w, uint h)
{
  XImage *_image;

  zxImageInit( img );
  if( !( _image = XGetImage( zxdisplay, pmap, 0, 0, w, h, AllPlanes, ZPixmap ) ) )
    return NULL;;
  img->width = w;
  img->height = h;
  img->bpp = zxImageBPPDefault();
  img->buf = (ubyte *)_image->data;
  free( _image );
  return img;
}

/* file format checker and generalized I/O */

bool zxImageFileIdent(char filename[], const unsigned char ident[], uint size)
{
  FILE *fp;
  char buf[BUFSIZ];

  if( !( fp = fopen( filename, "rb" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  if( fread( buf, sizeof(byte), size, fp ) != size );
  fclose( fp );
  return !memcmp( buf, ident, sizeof(byte)*size ) ? true : false;
}

int zxImageReadFile(zxImage *img, char filename[])
{
  zxImageInit( img );

  /* Portable Bitmap/Graymap/Pixmap */
  if( zxImageFileIsPNM( filename ) )
    return zxImageReadPNMFile( img, filename );

#ifdef __ZX11_USE_PNG
  if( zxImageFileIsPNG( filename ) )
    return zxImageReadPNGFile( img, filename );
#endif /* __ZX11_USE_PNG */

#ifdef __ZX11_USE_TIFF
  if( zxImageFileIsTIFF( filename ) )
    return zxImageReadTIFFFile( img, filename );
#endif /* __ZX11_USE_TIFF */

#ifdef __ZX11_USE_JPEG
  if( zxImageFileIsJPEG( filename ) )
    return zxImageReadJPEGFile( img, filename );
#endif /* __ZX11_USE_JPEG */

#ifdef __ZX11_USE_BMP
  if( zxImageFileIsBMP( filename ) )
    return zxImageReadBMPFile( img, filename );
#endif /* __ZX11_USE_BMP */

#ifdef __ZX11_USE_MAG
  if( zxImageFileIsMAG( filename ) )
    return zxImageReadMAGFile( img, filename );
#endif /* __ZX11_USE_MAG */

#ifdef __ZX11_USE_PDT
  if( zxImageFileIsPDT( filename ) )
    return zxImageReadPDTFile( img, filename );
#endif /* __ZX11_USE_PDT */

  ZRUNERROR( "unknown format" );
  return 0;
}

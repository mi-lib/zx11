/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage - image manipulation on X11
 */

#include <zx11/zximage.h>
#include <math.h>

#include <zx11/zximage_supported.h>

/* pixel manipulation */

/* get a pixel of an image. */
zxPixel zxImageCellPixel(const zxImage *img, uint x, uint y)
{
  zxPixel pixel = 0;

  memcpy( &pixel, zxImageAddr(img,x,y), img->bpp );
  return pixel;
}

/* get a pixel of an image if the designated position is valid. */
zxPixel zxImageCellPixelCheck(const zxImage *img, uint x, uint y)
{
  return zxImagePosIsValid( img, x, y ) ?
    zxImageCellPixel( img, x, y ) : 0;
}

/* put a pixel on an image. */
void zxImageCellFromPixel(zxImage *img, uint x, uint y, zxPixel pixel)
{
  memcpy( zxImageAddr(img,x,y), &pixel, img->bpp );
}

/* put a pixel on an image if the designated position is valid. */
void zxImageCellFromPixelCheck(zxImage *img, uint x, uint y, zxPixel pixel)
{
  if( zxImagePosIsValid( img, x, y ) )
    zxImageCellFromPixel( img, x, y, pixel );
}

/* get a grayscalized pixel of an image. */
ubyte zxImageCellGS(const zxImage *img, uint x, uint y)
{
  ubyte r, g, b;

  zxImageCellRGB( img, x, y, &r, &g, &b );
  return ( r + g + b ) / 3;
}

/* bit per pixel from color depth. */
static ubyte _zxImageBPP(ubyte depth)
{
  switch( depth ){
  case 8:  return sizeof(ubyte);
  case 16: return sizeof(uword);
  case 24:
  case 32: return sizeof(uint32_t);
  default: ;
  }
  ZRUNERROR( "unsupported color depth %d", depth );
  return 0;
}

/* set color depth and pixel manipulator of an image. */
static bool _zxImageSetDepth(zxImage *img, ubyte depth)
{
  if( ( img->bpp = _zxImageBPP( depth ) ) == 0 ) return false;
  img->pm = zxPixelManipFind( depth );
  return true;
}

/* buffer manipulation */

/* initialize an image. */
zxImage *zxImageInit(zxImage *img)
{
  img->width = img->height = 0;
  img->bpp = 0;
  img->pm = NULL;
  img->buf = NULL;
  img->mask_buf = NULL;
  return img;
}

/* allocate an image. */
zxImage *zxImageAlloc(zxImage *img, uint width, uint height, ubyte depth)
{
  img->width = width;
  img->height = height;
  img->mask_buf = NULL;
  if( !_zxImageSetDepth( img, depth ) ) return NULL;
  if( !( img->buf = zAlloc( ubyte, img->width*img->height*img->bpp ) ) ){
    ZRUNERROR( "cannot allocate enough memory for image buffer" );
    return NULL;
  }
  return img;
}

/* allocate mask buffer of an image. */
zxImage *zxImageAllocMask(zxImage *img)
{
  if( !( img->mask_buf = zAlloc( ubyte, img->width*img->height ) ) ){
    ZRUNERROR( "cannot allocate enough memory for mask buffer" );
    return NULL;
  }
  return img;
}

/* set mask buffer of an image according to the pecified pixel. */
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

/* destroy an image. */
zxImage *zxImageDestroy(zxImage *img)
{
  free( img->buf );
  zFree( img->mask_buf );
  return zxImageInit( img );
}

/* clear an image. */
zxImage *zxImageClear(zxImage *img)
{
  memset( img->buf, 0, img->width*img->height*img->bpp );
  if( img->mask_buf )
    memset( img->mask_buf, 0, img->width*img->height );
  return img;
}

/* fill an image with the specified pixel. */
zxImage *zxImageFill(zxImage *img, zxPixel pixel)
{
  uint i, j;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ )
      zxImageCellFromPixel( img, j, i, pixel );
  return img;
}

/* check if sizes of two images are equal. */
bool zxImageSizeEqual(const zxImage *img1, const zxImage *img2)
{
  if( img1->width  != img2->width ||
      img1->height != img2->height ||
      img1->bpp    != img2->bpp ){
    ZRUNERROR( "size mismatch of two image buffers" );
    return false;
  }
  return true;
}

/* copy an image. */
zxImage *zxImageCopy(const zxImage *src, zxImage *dest)
{
  memcpy( dest->buf, src->buf, src->width*src->height*src->bpp );
  dest->pm = src->pm;
  return dest;
}

/* clone an image. */
zxImage *zxImageClone(const zxImage *src, zxImage *dest)
{
  if( !zxImageAlloc( dest, src->width, src->height, src->bpp << 3 ) )
    return NULL;
  zxImageCopy( src, dest );
  if( !src->mask_buf ) return dest;
  /* mask cloning */
  if( !zxImageAllocMask( dest ) ) return NULL;
  memcpy( dest->mask_buf, src->mask_buf, src->width*src->height );
  return dest;
}

/* compare two images. */
bool zxImageCmp(const zxImage *img1, const zxImage *img2)
{
  if( !zxImageSizeEqual( img1, img2 ) ) return false;
  return memcmp( img1->buf, img2->buf, img1->width * img1->height * img1->bpp ) == 0;
}

/* image pasting */

/* find valid range of an image to manipulate. */
bool zxCanvasRange(uint width_dest, uint height_dest, uint width_src, uint height_src, uint x, uint y, uint *w, uint *h)
{
  uint dx, dy;

  dx = x >= width_dest  ? 0 : width_dest  - x;
  dy = y >= height_dest ? 0 : height_dest - y;
  if( ( *w = zMin( dx, width_src ) ) == 0 || ( *h = zMin( dy, height_src ) ) == 0 ){
    ZRUNERROR( "zero-sized canvas specified" );
    return false;
  }
  return true;
}

/* find valid range of an image to manipulate. */
bool zxImageCanvasRange(const zxImage *canvas, const zxImage *img, uint x, uint y, uint *w, uint *h)
{
  return zxCanvasRange( canvas->width, canvas->height, img->width, img->height, x, y, w, h );
}

/* put a partial image on a canvas image. */
zxImage *zxImagePut(zxImage *canvas, const zxImage *img, uint x, uint y)
{
  int w, h;

  if( !zxImageCanvasRange( canvas, img, x, y, (uint *)&w, (uint *)&h ) ) return NULL;
  for( h--; h>=0; h-- )
    memcpy( zxImageAddr( canvas, x, y+h ), zxImageAddr( img, 0, h ), w*canvas->bpp );
  return canvas;
}

/* put a partial image on a canvas image with a mask. */
zxImage *zxImagePutMasked(zxImage *canvas, const zxImage *img, uint x, uint y, zxPixel mask)
{
  uint i, j;
  uint w, h;
  zxPixel pixel;

  if( !zxImageCanvasRange( canvas, img, x, y, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      if( ( pixel = zxImageCellPixel( img, j, i ) ) != mask )
        zxImageCellFromPixel( canvas, x+j, y+i, pixel );
  return canvas;
}

/* alpha-blend two images. */
zxImage *zxImagePutAlphaBlend(zxImage *canvas, const zxImage *img1, const zxImage *img2, uint x, uint y, double alpha)
{
  uint i, j;
  uint w, h;

  if( !zxImageSizeEqual( img1, img2 ) ) return NULL;
  if( !zxImageCanvasRange( canvas, img1, x, y, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( canvas, x+j, y+i,
        zxPixelAlphaBlend( canvas->pm, zxImageCellPixel( img1, j, i ), zxImageCellPixel( img2, j, i ), alpha ) );
  return canvas;
}

/* alpha-blend two images with a mask. */
zxImage *zxImagePutAlphaBlendMasked(zxImage *canvas, const zxImage *img1, const zxImage *img2, uint x, uint y, double alpha, zxPixel mask)
{
  uint i, j;
  uint w, h;
  zxPixel p1, p2;

  if( !zxImageSizeEqual( img1, img2 ) ) return NULL;
  if( !zxImageCanvasRange( canvas, img1, x, y, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      p1 = zxImageCellPixel( img1, j, i );
      p2 = zxImageCellPixel( img2, j, i );
      zxImageCellFromPixel( canvas, x+j, y+i,
        p2 == mask ? p1 : zxPixelAlphaBlend( canvas->pm, p1, p2, alpha ) );
    }
  return canvas;
}

/* superimpose an image to a canvas image. */
zxImage *zxImagePutSuperimpose(zxImage *canvas, const zxImage *img, uint x, uint y)
{
  uint i, j;
  uint w, h;
  zxPixel p1, p2;
  double alpha;

  if( !img->mask_buf )
    return zxImagePut( canvas, img, 0, 0 );
  if( !zxImageCanvasRange( canvas, img, x, y, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      p1 = zxImageCellPixel( canvas, x+j, y+i );
      p2 = zxImageCellPixel( img, j, i );
      alpha = (double)zxImageMask( img, j, i ) / 0xff;
      zxImageCellFromPixel( canvas, x+j, y+i, zxPixelAlphaBlend( canvas->pm, p1, p2, alpha ) );
    }
  return canvas;
}

/* get a partial image from another. */
zxImage *zxImageGet(const zxImage *src, zxImage *dest, uint x, uint y)
{
  int w, h;

  if( !zxImageCanvasRange( src, dest, x, y, (uint *)&w, (uint *)&h ) ) return NULL;
  for( h--; h>=0; h-- )
    memcpy( zxImageAddr( dest, 0, h ), zxImageAddr( src, x, y+h ), w*dest->bpp );
  return dest;
}

/* geometric manipulations */

/* flip an image vertically. */
zxImage *zxImageVertFlip(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i), zxImageAddr(src,w-j-1,i), src->bpp );
  return dest;
}

/* flip an image horizontally. */
zxImage *zxImageHorizFlip(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i), zxImageAddr(src,j,src->height-i-1), src->bpp );
  return dest;
}

/* rotate an image 90 deg rightward. */
zxImage *zxImageRotRight(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  w = zMin( src->height, dest->width );
  h = zMin( src->width, dest->height );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i), zxImageAddr(src,i,src->height-j-1), src->bpp );
  return dest;
}

/* rotate an image 90 deg leftward. */
zxImage *zxImageRotLeft(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  w = zMin( src->height, dest->width );
  h = zMin( src->width, dest->height );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      memcpy( zxImageAddr(dest,j,i), zxImageAddr(src,src->width-i-1,j), src->bpp );
  return dest;
}

/* rotate an image at an arbitrary angle. */
zxImage *zxImageRot(const zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double angle)
{
  uint i, j, k;
  double x0, x1, x2, y0, y1, y2;
  uint px, py;
  double ct, st;

  ct = cos( angle );
  st = sin( angle );
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
        memcpy( zxImageAddr(dest,j,i), zxImageAddr(src,px,py), src->bpp );
      else
        memset( zxImageAddr(dest,j,i), 0, src->bpp );
    }
  }
  return dest;
}

/* rotate an image at an arbitrary angle with blending. */
zxImage *zxImageRotFilt(const zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double angle)
{
  uint i, j, k;
  double x0, x1, x2, y0, y1, y2, px[3], py[3];
  double d[4], w[4], wt;
  double ct, st;
  zxPixel p[4];

  ct = cos( angle );
  st = sin( angle );
  x0 = ox - (double)x*ct - (double)y*st;
  y0 = oy + (double)x*st - (double)y*ct;

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
        zxImageCellFromPixel( dest, j, i, zxPixelBlend( dest->pm, p, w, 4 ) );
      } else
        memset( zxImageAddr(dest,j,i), 0, src->bpp );
    }
  }
  return dest;
}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

static double __zx11_resize_filter(double x, double y){
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

/* resize an image as to fit the destination image. */
zxImage *zxImageResize(const zxImage *src, zxImage *dest)
{
  uint i, j;
  ubyte rs, gs, bs;
  double k, l;
  double x0, xs, xe, y0, ys, ye, xl, yl, xrate, yrate;
  double u, ut, r, g, b;

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
          zxImageCellRGB( src, l, k, &rs, &gs, &bs );
          u = __zx11_resize_filter( xl, yl );
          r += u * rs; g += u * gs; b += u * bs;
          ut += u;
        }
      }
      r /= ut; g /= ut; b /= ut;
      zxImageCellFromRGB( dest, j, i, r, g, b );
    }
  }
  return dest;
}

/* color manipulations */

/* abstract RGB layers of an image. */
const zxImage *zxImageAbstRGB(const zxImage *src, zxImage *rimg, zxImage *gimg, zxImage *bimg)
{
  uint i, j;
  ubyte r, g, b;

  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellRGB( src, j, i, &r, &g, &b );
      if( rimg && zxImagePosIsValid( rimg, j, i ) )
        zxImageCellFromRGB( rimg, j, i, r, 0, 0 );
      if( gimg && zxImagePosIsValid( gimg, j, i ) )
        zxImageCellFromRGB( gimg, j, i, 0, g, 0 );
      if( bimg && zxImagePosIsValid( bimg, j, i ) )
        zxImageCellFromRGB( bimg, j, i, 0, 0, b );
    }
  return src;
}

/* grayscalize an image. */
zxImage *zxImageGrayscalize(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  ubyte r, g, b;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      zxImageCellRGB( src, j, i, &r, &g, &b );
      r = ( r + g + b ) / 3;
      zxImageCellFromGS( dest, j, i, r );
    }
  return dest;
}

/* binarize an image. */
zxImage *zxImageBinarize(const zxImage *src, zxImage *dest, ubyte threshold)
{
  uint i, j, w, h;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromGS( dest, j, i, zxImageCellGS( src, j, i ) > threshold ? 0xff : 0 );
  return dest;
}

/* negate an image. */
zxImage *zxImageNegate(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( dest, j, i, zxImageCellNegate( src, j, i ) );
  return dest;
}

/* brighten an image. */
zxImage *zxImageBrighten(const zxImage *src, zxImage *dest, double rate)
{
  uint i, j;
  float r, g, b, rb, gb, bb;

  rate = zLimit( rate-1, -1, 1 );
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellFRGB( src, j, i, &r, &g, &b );
      rb = zLimit( r + rate, 0, 1 );
      gb = zLimit( g + rate, 0, 1 );
      bb = zLimit( b + rate, 0, 1 );
      zxImageCellFromFRGB( dest, j, i, rb, gb, bb );
    }
  return dest;
}

/* contrast an image. */
zxImage *zxImageContrast(const zxImage *src, zxImage *dest, double rate)
{
  uint i, j;
  float r, g, b, rc, gc, bc;

  if( ( rate = zLimit( rate, 0, 2 ) ) > 1 )
    rate = 1.0 / ( 2 - rate );
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellFRGB( src, j, i, &r, &g, &b );
      rc = zLimit( 0.5 + ( r - 0.5 ) * rate, 0, 1 );
      gc = zLimit( 0.5 + ( g - 0.5 ) * rate, 0, 1 );
      bc = zLimit( 0.5 + ( b - 0.5 ) * rate, 0, 1 );
      zxImageCellFromFRGB( dest, j, i, rc, gc, bc );
    }
  return dest;
}

/* correct gamma value of an image. */
zxImage *zxImageCorrectGamma(const zxImage *src, zxImage *dest, double gamma)
{
  uint i, j;
  float r, g, b, rg, gg, bg;
  const double gamma_min = 1.0e-6;
  const double gamma_max = 10.0;

  gamma = 1.0 / zLimit( gamma, gamma_min, gamma_max );
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellFRGB( src, j, i, &r, &g, &b );
      rg = pow( r, gamma );
      gg = pow( g, gamma );
      bg = pow( b, gamma );
      zxImageCellFromFRGB( dest, j, i, rg, gg, bg );
    }
  return dest;
}

/* normalize an image. */
zxImage *zxImageNormalize(const zxImage *src, zxImage *dest)
{
  uint i, j;
  ubyte r, g, b, r_max, g_max, b_max;

  /* find maximum value */
  r_max = g_max = b_max = 0;
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellRGB( src, j, i, &r, &g, &b );
      if( r > r_max ) r_max = r;
      if( g > g_max ) g_max = g;
      if( b > b_max ) b_max = b;
    }
  /* normalize */
  if( r_max == 0 ) r_max = 1;
  if( g_max == 0 ) g_max = 1;
  if( b_max == 0 ) b_max = 1;
  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ ){
      zxImageCellRGB( src, j, i, &r, &g, &b );
      zxImageCellFromFRGB( dest, j, i, (double)r/r_max, (double)g/g_max, (double)b/b_max );
    }
  return dest;
}

static void _zxImageHistogram(const zxImage *img, uint h[], double sh[])
{
  uint i, j;
  ubyte r, g, b;
  double d;

  memset( h, 0, sizeof(uint)*0x100 );
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      zxImageCellRGB( img, j, i, &r, &g, &b );
      h[(int)(r+g+b)/3]++;
    }
  d = 1.0 / ( img->width * img->height );
  for( sh[0]=d*h[0], i=1; i<=0xff; i++ )
    sh[i] = sh[i-1] + d * h[i];
}

/* equalize an image. */
zxImage *zxImageEqualize(const zxImage *src, zxImage *dest)
{
  uint x, y, i;
  uint w, h;
  uint *hi;
  double *sh, vmin, scalefactor;
  ubyte r, g, b;

  hi = zAlloc( uint, 0x100 );
  sh = zAlloc( double, 0x100 );
  if( !hi || !sh ){
    dest = NULL;
    goto TERMINATE;
  }
  _zxImageHistogram( src, hi, sh );
  for( vmin=0, i=0; i<=0xff; i++ )
    if( sh[i] > 0 ){
      vmin = sh[i];
      break;
    }
  scalefactor = (double)0xff / ( 1.0 - vmin );
  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( y=0; y<h; y++ ){
    for( x=0; x<w; x++ ){
      zxImageCellRGB( src, x, y, &r, &g, &b );
      r = zLimit( (int)( scalefactor * ( sh[r] - vmin ) ), 0, 0xff );
      g = zLimit( (int)( scalefactor * ( sh[g] - vmin ) ), 0, 0xff );
      b = zLimit( (int)( scalefactor * ( sh[b] - vmin ) ), 0, 0xff );
      zxImageCellFromRGB( dest, x, y, r, g, b );
    }
  }
 TERMINATE:
  free( hi );
  free( sh );
  return dest;
}

/* dither */

/* dither an image. */
static zxImage *_zxImageDither(const zxImage *src, zxImage *dest, const double pattern[])
{
  uint i, j;
  uint w, h;
  ubyte r, g, b, th;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      th = pattern[ (i%4)*4 + (j%4) ]*16 + 8;
      zxImageCellRGB( src, j, i, &r, &g, &b );
      zxImageCellFromRGB( dest, j, i,
        r >= th ? 0xff : 0,
        g >= th ? 0xff : 0,
        b >= th ? 0xff : 0 );
    }
  return dest;
}

/* dither an image based on the Bayer filter. */
zxImage *zxImageDitherBayer(const zxImage *src, zxImage *dest)
{
  const double pattern[] = {
     0,  8,  2, 10,
    12,  4, 14,  6,
     3, 11,  1,  9,
    15,  7, 13,  5 };
  return _zxImageDither( src, dest, pattern );
}

/* dither an image based on the net filter. */
zxImage *zxImageDitherNet(const zxImage *src, zxImage *dest)
{
  const double pattern[] = {
    11,  4,  6,  9,
    12,  0,  2, 14,
     7,  8, 10,  5,
     3, 15, 13,  1 };
  return _zxImageDither( src, dest, pattern );
}

/* dither an image based on the spiral filter. */
zxImage *zxImageDitherSpiral(const zxImage *src, zxImage *dest)
{
  const double pattern[] = {
     6,  7,  8,  9,
     5,  0,  1, 10,
     4,  3,  2, 11,
    15, 14, 13, 12 };
  return _zxImageDither( src, dest, pattern );
}

/* dither an image based on the error diffusion method. */
#define _zxImageDitherErrorDiffusionVal(val,error) do{\
  if( *(val) >= 0.5 ){\
    *(error) = *(val) - 1.0;\
    *(val) = 1.0;\
  } else{\
    *(error) = *(val);\
    *(val) = 0.0;\
  }\
} while(0)

/* dither an image based on the error diffusion method. */
#define _zxImageDitherErrorDiffusionPixel(img,j,i,er,eg,eb,weight) do{\
  if( zxImagePosIsValid( img, j, i ) ){\
    float __r, __g, __b;\
    zxImageCellFRGB( img, j, i, &__r, &__g, &__b );\
    __r = zLimit( __r + weight * er, 0, 1 );\
    __g = zLimit( __g + weight * eg, 0, 1 );\
    __b = zLimit( __b + weight * eb, 0, 1 );\
    zxImageCellFromFRGB( img, j, i, __r, __g, __b );\
  }\
} while(0)

/* dither based on error diffusion with Floyd-Steinberg distribution. */
zxImage *zxImageDitherErrorDiffusionDRC(zxImage *img)
{
  uint i, j;
  float r, g, b, er, eg, eb;

  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      zxImageCellFRGB( img, j, i, &r, &g, &b );
      _zxImageDitherErrorDiffusionVal( &r, &er );
      _zxImageDitherErrorDiffusionVal( &g, &eg );
      _zxImageDitherErrorDiffusionVal( &b, &eb );
      _zxImageDitherErrorDiffusionPixel( img, j+1, i  , er, eg, eb, 7.0/16 );
      _zxImageDitherErrorDiffusionPixel( img, j-1, i+1, er, eg, eb, 3.0/16 );
      _zxImageDitherErrorDiffusionPixel( img, j  , i+1, er, eg, eb, 5.0/16 );
      _zxImageDitherErrorDiffusionPixel( img, j+1, i+1, er, eg, eb, 1.0/16 );
      zxImageCellFromFRGB( img, j, i, r, g, b );
    }
  return img;
}

/* dither based on error diffusion. */
zxImage *zxImageDitherErrorDiffusion(const zxImage *src, zxImage *dest)
{
  if( !zxImageCopy( src, dest ) ) return NULL;
  return zxImageDitherErrorDiffusionDRC( dest );
}

/* general filter */

/* pick up a pixel of an image to filter it. */
static void _zxImageFilterPickPixel(const zxImage *src, uint x, uint y, uint w, uint h, int size, zxPixel p[], uint sh)
{
  int i, j, k, sx, sy;

  for( k=0, i=0; i<size; i++ ){
    sy = zLimit( y + i - sh, 0, h-1 );
    for( j=0; j<size; j++ ){
      sx = zLimit( x + j - sh, 0, w-1 );
      p[k++] = zxImageCellPixel( src, sx, sy );
    }
  }
}

/* filter an image. */
zxImage *zxImageFilter(const zxImage *src, zxImage *dest, double f[], int size)
{
  uint x, y;
  uint w, h, s2, sh;
  zxPixel *p;

  if( !( p = zAlloc( zxPixel, ( s2 = size * size ) ) ) ) goto TERMINATE;
  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  sh = ( size - 1 ) / 2;
  for( y=0; y<h; y++ )
    for( x=0; x<w; x++ ){
      _zxImageFilterPickPixel( src, x, y, w, h, size, p, sh );
      zxImageCellFromPixel( dest, x, y, zxPixelBlend( dest->pm, p, f, s2 ) );
    }
 TERMINATE:
  free( p );
  return dest;
}

/* filter an image by two patterns. */
zxImage *zxImageFilter2(const zxImage *src, zxImage *dest, double f1[], double f2[], int size)
{
  uint x, y;
  uint w, h, s2, sh;
  zxPixel *p, p1, p2;

  if( !( p = zAlloc( zxPixel, ( s2 = size * size ) ) ) ) goto TERMINATE;
  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  sh = ( size - 1 ) / 2;
  for( y=0; y<h; y++ )
    for( x=0; x<w; x++ ){
      _zxImageFilterPickPixel( src, x, y, w, h, size, p, sh );
      p1 = zxPixelBlend( src->pm, p, f1, s2 );
      p2 = zxPixelBlend( src->pm, p, f2, s2 );
      zxImageCellFromPixel( dest, x, y, zxPixelNorm2( dest->pm, p1, p2 ) );
    }
 TERMINATE:
  free( p );
  return dest;
}

/* smoothing */

/* comparison function for smooth filtering. */
static int _zx11_smooth_filter_cmp(void *v1, void *v2, void *dummy)
{
  if( *(ubyte*)v1 > *(ubyte*)v2 ) return 1;
  if( *(ubyte*)v1 < *(ubyte*)v2 ) return -1;
  return 0;
}

/* smooth filtering of an image with a key. */
static void _zxImageSmoothFilterKey(const zxImage *img, uint j, uint i, ubyte *rs, ubyte *gs, ubyte *bs, uint size, ubyte *r, ubyte *g, ubyte *b, int key)
{
  uint _i, _j, si, sj, k;
  int s2, sh;

  s2 = size * size;
  sh = ( size - 1 ) / 2;
  for( _i=0; _i<size; _i++ )
    for( _j=0; _j<size; _j++ ){
      k = _i*size + _j;
      if( zxImagePosIsValid( img, ( sj = j+_j-sh ), ( si = i+_i-sh ) ) ){
        zxImageCellRGB( img, sj, si, r, g, b );
      } else
        *r = *g = *b = 0;
      zInsertSort( rs, r, k, s2, sizeof(ubyte), _zx11_smooth_filter_cmp, NULL );
      zInsertSort( gs, g, k, s2, sizeof(ubyte), _zx11_smooth_filter_cmp, NULL );
      zInsertSort( bs, b, k, s2, sizeof(ubyte), _zx11_smooth_filter_cmp, NULL );
    }
  *r = rs[key];
  *g = gs[key];
  *b = bs[key];
}

/* smooth filtering of an image. */
static zxImage *_zxImageSmoothFilter(const zxImage *src, zxImage *dest, int size, int key)
{
  uint i, j;
  uint w, h;
  ubyte *rs, *gs, *bs, r, g, b;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  rs = zAlloc( ubyte, size * size );
  gs = zAlloc( ubyte, size * size );
  bs = zAlloc( ubyte, size * size );
  if( !rs || !gs || !bs ) goto TERMINATE;
  for( i=0; i<h; i++ ){
    for( j=0; j<w; j++ ){
      _zxImageSmoothFilterKey( src, j, i, rs, gs, bs, size, &r, &g, &b, key );
      zxImageCellFromRGB( dest, j, i, r, g, b );
    }
  }
 TERMINATE:
  free( rs );
  free( gs );
  free( bs );
  return dest;
}

/* median filter of an image. */
zxImage *zxImageSmoothMedian(const zxImage *src, zxImage *dest, int size)
{
  return _zxImageSmoothFilter( src, dest, size, ( size*size - 1 ) / 2 );
}

/* minimum filter of an image. */
zxImage *zxImageSmoothMin(const zxImage *src, zxImage *dest, int size)
{
  return _zxImageSmoothFilter( src, dest, size, 0 );
}

/* maximum filter of an image. */
zxImage *zxImageSmoothMax(const zxImage *src, zxImage *dest, int size)
{
  return _zxImageSmoothFilter( src, dest, size, size*size - 1 );
}

/* Gaussian filter of an image. */
zxImage *zxImageSmoothGaussian(const zxImage *src, zxImage *dest)
{
  double weight[] = {
    0.0625, 0.1250, 0.0625,
    0.1250, 0.2500, 0.1250,
    0.0625, 0.1250, 0.0625,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* antialiasing of an image. */
zxImage *zxImageAntialias(const zxImage *src, zxImage *dest)
{
  double weight[] = {
    0.041667, 0.083333, 0.041667,
    0.083333, 0.5,      0.083333,
    0.041667, 0.083333, 0.041667,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* bilateral smoothing of an image. */
zxImage *zxImageSmoothBilateral(const zxImage *src, zxImage *dest)
{
  double weight[] = {
    0.0625, 0.1250, 0.0625,
    0.1250, 0.2500, 0.1250,
    0.0625, 0.1250, 0.0625,
  };
  double filter[9], diff, den;
  zxPixel p[9];
  uint x, y, w, h;
  float r1, g1, b1, r0, g0, b0;
  int i, j, k;
  const double beta = 2 * 0.1 * 0.1;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( y=0; y<h; y++ )
    for( x=0; x<w; x++ ){
      _zxImageFilterPickPixel( src, x, y, w, h, 3, p, 1 );
      src->pm->PixelFRGB( p[4], &r0, &g0, &b0 );
      for( den=0, k=0, i=0; i<3; i++ )
        for( j=0; j<3; j++, k++ ){
          src->pm->PixelFRGB( p[k], &r1, &g1, &b1 );
          diff = (r1-r0)*(r1-r0) + (g1-g0)*(g1-g0) + (b1-b0)*(b1-b0);
          den += ( filter[k] = weight[k] * exp( -diff/beta ) );
        }
      for( k=0; k<9; k++ ) filter[k] /= den;
      zxImageCellFromPixel( dest, x, y, zxPixelBlend( dest->pm, p, filter, 9 ) );
    }
  return dest;
}

/* edge detection */

/* differentiation of an image. */
zxImage *zxImageDiff(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  ubyte r, g, b, pr, pg, pb;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ ){
    zxImageCellRGB( src, 0, i, &pr, &pg, &pb );
    zxImageCellFromRGB( dest, 0, i, pr, pg, pb );
    for( j=1; j<w; j++ ){
      zxImageCellRGB( src, j, i, &r, &g, &b );
      zxImageCellFromRGB( dest, j, i, r-pr, g-pg, b-pb );
      pr = r; pg = g; pb = b;
    }
  }
  return dest;
}

/* integral of an image. */
zxImage *zxImageIntegral(const zxImage *src, zxImage *dest)
{
  uint i, j;
  uint w, h;
  ubyte r, g, b, ri, gi, bi;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ ){
    zxImageCellRGB( src, 0, i, &ri, &gi, &bi );
    memcpy( zxImageAddr(dest,0,i), zxImageAddr(src,0,i), dest->bpp );
    for( j=1; j<w; j++ ){
      zxImageCellRGB( src, j, i, &r, &g, &b );
      ri += r; gi += g; bi += b;
      zxImageCellFromRGB( dest, j, i, ri, gi, bi );
    }
  }
  return dest;
}

/* Prewitt-H filter for edge detection of an image. */
zxImage *zxImageEdgePrewittH(const zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0, 0.0, 1.0,
   -1.0, 0.0, 1.0,
   -1.0, 0.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* Prewitt-V filter for edge detection of an image. */
zxImage *zxImageEdgePrewittV(const zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0,-1.0,-1.0,
    0.0, 0.0, 0.0,
    1.0, 1.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* Prewitt filter for edge detection of an image. */
zxImage *zxImageEdgePrewitt(const zxImage *src, zxImage *dest)
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

/* Sobel-H filter for edge detection of an image. */
zxImage *zxImageEdgeSobelH(const zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0, 0.0, 1.0,
   -2.0, 0.0, 2.0,
   -1.0, 0.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* Sobel-V filter for edge detection of an image. */
zxImage *zxImageEdgeSobelV(const zxImage *src, zxImage *dest)
{
  double weight[] = {
   -1.0,-2.0,-1.0,
    0.0, 0.0, 0.0,
    1.0, 2.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* Sobel filter for edge detection of an image. */
zxImage *zxImageEdgeSobel(const zxImage *src, zxImage *dest)
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

/* Laplacian filter for edge detection of an image. */
zxImage *zxImageEdgeLaplacian(const zxImage *src, zxImage *dest)
{
  double weight[] = {
    1.0, 1.0, 1.0,
    1.0,-8.0, 1.0,
    1.0, 1.0, 1.0,
  };
  return zxImageFilter( src, dest, weight, 3 );
}

/* get four adjacent pixels of a pixel of an image. */
static void _zxImageEdgeFilterGet4Pixels(const zxImage *img, uint j, uint i, float r[4], float g[4], float b[4])
{
  uint j1, i1;

  j1 = j < img->width  - 1 ? j + 1 : j;
  i1 = i < img->height - 1 ? i + 1 : i;
  zxImageCellFRGB( img, j,   i, &r[0], &g[0], &b[0] );
  zxImageCellFRGB( img, j1,  i, &r[1], &g[1], &b[1] );
  zxImageCellFRGB( img, j,  i1, &r[2], &g[2], &b[2] );
  zxImageCellFRGB( img, j1, i1, &r[3], &g[3], &b[3] );
}

/* pixel value function of Roberts filter. */
static float _zxImageEdgeRobertsPixelVal(const float val[4])
{
  return sqrt( pow( sqrt(val[0]) - sqrt(val[3]), 2 ) + pow( sqrt(val[1]) - sqrt(val[2]), 2 ) );
}

/* pixel value function of Forsen filter. */
static float _zxImageEdgeForsenPixelVal(const float val[4])
{
  return fabs( val[0] - val[3] ) + fabs( val[1] - val[2] );
}

/* edge filter of an image. */
static zxImage *_zxImageEdgeFilter(const zxImage *src, zxImage *dest, float (* pixel_val)(const float val[4]))
{
  uint i, j;
  float r[4], g[4], b[4];
  float val;

  for( i=0; i<src->height; i++ ){
    for( j=0; j<src->width; j++ ){
      _zxImageEdgeFilterGet4Pixels( src, j, i, r, g, b );
      val = pixel_val( r );
      zxImageCellFromFRGB( dest, j, i, val, val, val );
    }
  }
  return dest;
}

/* Roberts filter for edge detection of an image. */
zxImage *zxImageEdgeRoberts(const zxImage *src, zxImage *dest)
{
  return _zxImageEdgeFilter( src, dest, _zxImageEdgeRobertsPixelVal );
}

/* Forsen filter for edge detection of an image. */
zxImage *zxImageEdgeForsen(const zxImage *src, zxImage *dest)
{
  return _zxImageEdgeFilter( src, dest, _zxImageEdgeForsenPixelVal );
}

/* Hough transformation */

ZEDA_DEF_ARRAY2_CLASS( zxHoughBin, zxHoughBinListCell* );

/* Hough transformation for line detection. */
zxHoughBinList *zxImageHoughLines(zxHoughBinList *bin_list, const zxImage *src, uint theta_div, uint dist_div)
{
  double theta, dist, dist_max;
  uint i, j, k, l;
  zxHoughBin bin;
  zxHoughBinListCell *bin_list_cell, *bin_list_cell_prev;
  ubyte r, g, b;
  zxImage tmpimg1, tmpimg2;

  zListInit( bin_list );
  zArray2Alloc( &bin, zxHoughBinListCell*, theta_div, dist_div );
  for( i=0; i<dist_div; i++ )
    for( j=0; j<theta_div; j++ )
      *zArray2ElemNC(&bin,j,i) = NULL;
  /* edge detection */
  zxImageClone( src, &tmpimg1 );
  zxImageGrayscalizeDRC( &tmpimg1 );
  zxImageAllocDefault( &tmpimg2, tmpimg1.width, tmpimg1.height );
  zxImageEdgeLaplacian( &tmpimg1, &tmpimg2 );
  zxImageNormalizeDRC( &tmpimg2 );
  /* vote to bins */
  dist_max = sqrt( tmpimg2.width*tmpimg2.width + tmpimg2.height*tmpimg2.height );
  for( i=0; i<tmpimg2.height; i++ ){
    for( j=0; j<tmpimg2.width; j++ ){
      zxImageCellRGB( &tmpimg2, j, i, &r, &g, &b );
      if( r < 0x80 ) continue;
      for( k=0; k<theta_div; k++ ){
        theta = 2 * M_PI * k / theta_div;
        dist = j * cos(theta) + i * sin(theta);
        l = (uint)( dist/dist_max * dist_div );
        if( !( bin_list_cell = *zArray2ElemNC(&bin,k,l) ) ){
          if( !( bin_list_cell = zAlloc( zxHoughBinListCell, 1 ) ) ){
            ZALLOCERROR();
            bin_list = NULL;
            goto TERMINATE;
          }
          bin_list_cell->data.theta = theta;
          bin_list_cell->data.dist = dist_max * l / dist_div;
          bin_list_cell->data.count = 1;
          *zArray2ElemNC(&bin,k,l) = bin_list_cell;
          zListInsertHead( bin_list, bin_list_cell );
        } else{
          bin_list_cell->data.count++;
          while( zListCellPrev(bin_list_cell) != zListRoot(bin_list) &&
                 zListCellPrev(bin_list_cell)->data.count < bin_list_cell->data.count ){
            bin_list_cell_prev = zListCellPrev(bin_list_cell);
            zListCellSwap( zxHoughBinListCell, bin_list_cell, bin_list_cell_prev );
          }
        }
      }
    }
  }
  if( zListIsEmpty( bin_list ) ){
    ZRUNWARN( "visible pixels not found" );
    bin_list = NULL;
    goto TERMINATE;
  }

 TERMINATE:
  zxImageDestroy( &tmpimg1 );
  zxImageDestroy( &tmpimg2 );
  zArray2Free( &bin );
  return bin_list;
}

/* normal map */

/* tangent of a height map along x-axis */
static double _zxImageNormalDX(const zxImage *img, uint j, uint i)
{
  ubyte v1, v2, v3;

  if( j <= 0 ){
    v1 = zxImageCellGS( img, 0, i );
    v2 = zxImageCellGS( img, 1, i );
    v3 = zxImageCellGS( img, 2, i );
    return -0.5 * (double)v1 + 2 * (double)v2 / 3 - (double)v3 / 6;
  }
  if( j + 1 >= img->width ){
    v1 = zxImageCellGS( img, img->width-3, i );
    v2 = zxImageCellGS( img, img->width-2, i );
    v3 = zxImageCellGS( img, img->width-1, i );
    return (double)v1 / 6 - 2 * (double)v2 / 3 + 0.5 * (double)v3;
  }
  v1 = zxImageCellGS( img, j-1, i );
  v3 = zxImageCellGS( img, j+1, i );
  return (double)( v3 - v1 ) / 6;
}

/* tangent of a height map along y-axis */
static double _zxImageNormalDY(const zxImage *img, uint j, uint i)
{
  ubyte v1, v2, v3;

  if( i <= 0 ){
    v1 = zxImageCellGS( img, j, 0 );
    v2 = zxImageCellGS( img, j, 1 );
    v3 = zxImageCellGS( img, j, 2 );
    return -0.5 * (double)v1 + 2 * (double)v2 / 3 - (double)v3 / 6;
  }
  if( i + 1 >= img->height ){
    v1 = zxImageCellGS( img, j, img->height-3 );
    v2 = zxImageCellGS( img, j, img->height-2 );
    v3 = zxImageCellGS( img, j, img->height-1 );
    return (double)v1 / 6 - 2 * (double)v2 / 3 + 0.5 * (double)v3;
  }
  v1 = zxImageCellGS( img, j, i-1 );
  v3 = zxImageCellGS( img, j, i+1 );
  return (double)( v3 - v1 ) / 6;
}

/* normal vector of an image. */
void zxImageNormalVec(const zxImage *img, double depth, uint j, uint i, double *x, double *y, double *z)
{
  double dx, dy, l;

  dx = _zxImageNormalDX( img, j, i );
  dy = _zxImageNormalDY( img, j, i );
  l = sqrt( dx*dx + dy*dy + 1.0/(depth*depth) );
  *x = _zxNormalize( -dx / l );
  *y = _zxNormalize( -dy / l );
  *z = _zxNormalize( 1.0 / l );
}

/* normal vector map of an image. */
zxImage *zxImageNormalMap(const zxImage *src, double depth, zxImage *dest)
{
  uint i, j;
  uint w, h;
  double x, y, z;

  if( !zxImageCanvasRange( dest, src, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      zxImageNormalVec( src, depth, j, i, &x, &y, &z );
      zxImageCellFromFRGB( dest, j, i, x, y, z );
    }
  return dest;
}

/* special effect */

/* put a rectangle on an image. */
zxImage *zxImagePutRect(zxImage *img, uint x, uint y, uint width, uint height, ubyte red, ubyte green, ubyte blue)
{
  uint i, j, w, h;
  zxPixel pixel;

  if( !zxCanvasRange( img->width, img->height, width, height, x, y, &w, &h ) ) return NULL;
  pixel = img->pm->PixelFromRGB( red, green, blue );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( img, x+j, y+i, pixel );
  return img;
}

/* average of pixels in a designated area of an image. */
static zxPixel _zxImageCellAverage(const zxImage *img, uint x, uint y, uint w, uint h)
{
  uint i, j;
  zxPixel rl, gl, bl, r, g, b;
  ubyte rd, gd, bd;

  r = g = b = 0;
  for( i=0; i<h; i++ ){
    rl = gl = bl = 0;
    for( j=0; j<w; j++ ){
      zxImageCellRGB( img, x+j, y+i, &rd, &gd, &bd );
      rl += rd; gl += gd; bl += bd;
    }
    r += rl / w;
    g += gl / w;
    b += bl / w;
  }
  r /= h;
  g /= h;
  b /= h;
  return img->pm->PixelFromRGB( r, g, b );
}

/* average an image. */
static zxImage *_zxImageAverage(zxImage *img, uint x, uint y, uint w, uint h)
{
  uint i, j;
  zxPixel p;

  w = zMin( img->width - x, w );
  h = zMin( img->height - y, h );
  p = _zxImageCellAverage( img, x, y, w, h );
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromPixel( img, x+j, y+i, p );
  return img;
}

/* mosaic of an image. */
zxImage *zxImageMosaic(zxImage *img, uint x, uint y, uint w, uint h, uint nx, uint ny)
{
  uint i, j, dx, dy, mx, my;

  dx = w / nx;
  dy = h / ny;
  mx = zMin( x+w, img->width );
  my = zMin( y+h, img->height );
  for( i=y; i<my; i+=dy )
    for( j=x; j<mx; j+=dx )
      _zxImageAverage( img, j, i, dx, dy );
  return img;
}

/* drawing */

/* convert an image to a pixmap. */
Pixmap zxImageToPixmap(zxWindow *win, const zxImage *img, Pixmap pmap, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y)
{
  XImage *ip;

  if( ( ip = XCreateImage( zxdisplay, CopyFromParent, zxdepth, ZPixmap,
          16, (char *)img->buf, img->width, img->height,
          img->bpp<<3, img->bpp*img->width ) ) ){
    XPutImage( zxdisplay, pmap, zxWindowGC(win), ip, src_x, src_y, dest_x, dest_y, w, h );
    /* Note the asymmetry of XCreateImage and XDestroyImage. */
    /* Never call XDestroyImage since it frees memory allocated for pixel data, which is managed in zxImage. */
    free( ip );
  }
  return pmap;
}

/* create a pixmap from an image. */
Pixmap zxImageCreatePixmap(zxWindow *win, const zxImage *img)
{
  Pixmap pmap;

  if( ( pmap = zxPixmapCreate( win, img->width, img->height ) ) == None )
    return None;
  return zxImageToPixmapAll( win, img, pmap );
}

/* create a pixmap mask from an image. */
Pixmap zxImageCreatePixmapMask(zxWindow *win, const zxImage *img)
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

/* draw an image to a window. */
void zxImageDraw(zxWindow *win, const zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y)
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

/* draw an image to a window with a mask. */
bool zxImageDrawMask(zxWindow *win, const zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y)
{
  zxImage mask;
  uint i, j;
  ubyte val, *mp;

  if( !( mp = img->mask_buf ) ) return false;
  if( !zxImageAllocDefault( &mask, img->width, img->height ) ) return false;
  for( i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++ ){
      val = *mp++;
      zxImageCellFromRGB( &mask, j, i, val, val, val );
    }
  zxImageToPixmap( win, &mask, zxWindowCanvas(win), src_x, src_y, w, h, dest_x, dest_y );
  zxImageDestroy( &mask );
  return true;
}

/* convert a pixmap to an image. */
zxImage *zxImageFromPixmap(zxImage *img, Pixmap pmap, uint w, uint h)
{
  XImage *_image;

  zxImageInit( img );
  if( !( _image = XGetImage( zxdisplay, pmap, 0, 0, w, h, AllPlanes, ZPixmap ) ) )
    return NULL;;
  img->width = w;
  img->height = h;
  if( !_zxImageSetDepth( img, zxdepth ) ) return NULL;
  img->buf = (ubyte *)_image->data;
  free( _image );
  return img;
}

/* file format checker and generalized I/O */

/* identify format of an image file. */
bool zxImageFileIdent(const char *filename, const unsigned char ident[], uint size)
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

/* read an image file by automatically detecting format. */
int zxImageReadFile(zxImage *img, const char *filename)
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

/* write an image file by automatically detecting format. */
int zxImageWriteFile(const zxImage *img, const char *filename)
{
  char *suffix, suffix_lower[BUFSIZ];

  if( !( suffix = zGetSuffix( filename ) ) ) goto ZXIMAGE_WRITE_ERROR;
  zStrToLower( suffix, BUFSIZ, suffix_lower );
  if( strcmp( suffix_lower, "pbm" ) == 0 ) /* Portable Bitmap */
    return zxImageWritePBMFile( img, filename );
  if( strcmp( suffix_lower, "pgm" ) == 0 ) /* Portable Graymap */
    return zxImageWritePGMFile( img, filename );
  if( strcmp( suffix_lower, "ppm" ) == 0 ) /* Portable Pixmap */
    return zxImageWritePPMFile( img, filename );

#ifdef __ZX11_USE_PNG
  if( strcmp( suffix_lower, "png" ) == 0 )
    return zxImageWritePNGFile( img, filename );
#endif /* __ZX11_USE_PNG */

#ifdef __ZX11_USE_TIFF
  if( strcmp( suffix_lower, "tiff" ) == 0 )
    return zxImageWriteTIFFFileDefault( img, filename );
#endif /* __ZX11_USE_TIFF */

#ifdef __ZX11_USE_JPEG
  if( strcmp( suffix_lower, "jpeg" ) == 0 )
    return zxImageWriteJPEGFileDefault( img, filename );
#endif /* __ZX11_USE_JPEG */

#ifdef __ZX11_USE_BMP
  if( strcmp( suffix_lower, "bmp" ) == 0 )
    return zxImageWriteBMPFile( img, filename );
#endif /* __ZX11_USE_BMP */

 ZXIMAGE_WRITE_ERROR:
  ZRUNERROR( "cannot identify image format to save" );
  return 0;
}

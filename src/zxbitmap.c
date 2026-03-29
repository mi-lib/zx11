/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxbitmap - bitmap manipulation.
 */

#include <zx11/zximage.h>

/* initialize a bitmap. */
zBitmap *zBitmapInit(zBitmap *bitmap)
{
  bitmap->width = bitmap->height = 0;
  bitmap->byte_per_line = 0;
  bitmap->buf = NULL;
  return bitmap;
}

/* allocate a bitmap. */
zBitmap *zBitmapAlloc(zBitmap *bitmap, uint width, uint height)
{
  bitmap->byte_per_line = ( ( bitmap->width = width ) - 1 ) / 8 + 1;
  bitmap->height = height;
  if( !( bitmap->buf = zAlloc( ubyte, bitmap->byte_per_line*bitmap->height ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return bitmap;
}

/* destroy a bitmap. */
zBitmap *zBitmapDestroy(zBitmap *bitmap)
{
  free( bitmap->buf );
  return zBitmapInit( bitmap );
}

/* copy a bitmap. */
zBitmap *zBitmapCopy(const zBitmap *src, zBitmap *dest)
{
  if( dest->width != src->width || dest->height != src->height ){
    ZRUNERROR( "size mismatch between source (%dx%d) and destination (%dx%d) bitmaps", src->width, src->height, dest->width, dest->height );
    return NULL;
  }
  dest->byte_per_line = src->byte_per_line;
  memcpy( dest->buf, src->buf, sizeof(ubyte)*src->byte_per_line*src->height );
  return dest;
}

/* clone a bitmap. */
zBitmap *zBitmapClone(const zBitmap *src, zBitmap *dest)
{
  if( !zBitmapAlloc( dest, src->width, src->height ) ) return NULL;
  return zBitmapCopy( src, dest );
}

/* get a bit of a bitmap. */
ubyte zBitmapGetBit(const zBitmap *bitmap, uint x, uint y)
{
  ubyte *p;
  uint fx;

  p = bitmap->buf + bitmap->byte_per_line * y + x / 8;
  fx = x % 8;
  return bit_get( *p, fx );
}

/* put a bit on a bitmap. */
ubyte zBitmapPutBit(zBitmap *bitmap, uint x, uint y, ubyte val)
{
  ubyte *p;
  uint fx;

  p = bitmap->buf + bitmap->byte_per_line * y + x / 8;
  fx = x % 8;
  return val ? bit_set( *p, fx ) : bit_reset( *p, fx );
}

/* fill a bitmap with a specified value. */
zBitmap *zBitmapFill(zBitmap *bitmap, ubyte val)
{
  uint i, j;

  for( i=0; i<bitmap->height; i++ )
    for( j=0; j<bitmap->width; j++ )
      zBitmapPutBit( bitmap, j, i, val );
  return bitmap;
}

/* convert a bitmap to an image. */
zxImage *zBitmapToImage(const zBitmap *src, zxImage *dest)
{
  uint i, j, w, h;

  if( !zxCanvasRange( dest->width, dest->height, src->width, src->height, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zxImageCellFromGS( dest, j, i, zBitmapGetBit( src, j, i ) ? 0xff : 0 );
  return dest;
}

/* convert an image to a bitmap. */
zBitmap *zxImageToBitmap(const zxImage *src, zBitmap *dest, ubyte threshold)
{
  uint i, j, w, h;

  if( !zxCanvasRange( dest->width, dest->height, src->width, src->height, 0, 0, &w, &h ) ) return NULL;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ )
      zBitmapPutBit( dest, j, i, zxImageCellGS( src, j, i ) > threshold ? 1 : 0 );
  return dest;
}

/* infration and defration of a bitmap. */

/* count neighbor grids that have designated referential value. */
static int _zBitmapCountNeighbors(const zBitmap *bitmap, uint x, uint y, uint w, uint h, ubyte ref)
{
  int i, j, sx, sy, count = 0;

  for( i=0; i<3; i++ ){
    sy = zLimit( y + i - 1, 0, h-1 );
    for( j=0; j<3; j++ ){
      sx = zLimit( x + j - 1, 0, w-1 );
      count += zBitmapGetBit( bitmap, sx, sy ) == ref;
    }
  }
  return count;
}

/* internal function to infrate a bitmap one-step. */
static int _zBitmapInfrateOne(const zBitmap *src, zBitmap *dest, ubyte ref)
{
  uint i, j, w, h;
  int count = 0;

  if( !zxCanvasRange( dest->width, dest->height, src->width, src->height, 0, 0, &w, &h ) ) return -1;
  for( i=0; i<h; i++ )
    for( j=0; j<w; j++ ){
      if( zBitmapPutBit( dest, j, i, _zBitmapCountNeighbors( src, j, i, src->width, src->height, ref ) > 0 ? ref : 1 - ref ) != zBitmapGetBit( src, j, i ) )
        count++;
    }
  return count;
}

/* infrate a bitmap one-step. */
int zBitmapInfrateOne(const zBitmap *src, zBitmap *dest)
{
  return _zBitmapInfrateOne( src, dest, 1 );
}

/* defrate a bitmap one-step. */
int zBitmapDefrateOne(const zBitmap *src, zBitmap *dest)
{
  return _zBitmapInfrateOne( src, dest, 0 );
}

/* internal function to infrate a bitmap. */
static int _zBitmapInfrate(const zBitmap *src, zBitmap *dest, ubyte ref, int num)
{
  zBitmap tmp;
  int ret;

  if( !zBitmapClone( src, &tmp ) ) return -1;
  while( 1 ){
    ret = _zBitmapInfrateOne( &tmp, dest, ref );
    if( --num == 0 ) break;
    zBitmapCopy( dest, &tmp );
  }
  zBitmapDestroy( &tmp );
  return ret;
}

/* infrate a bitmap. */
int zBitmapInfrate(const zBitmap *src, zBitmap *dest, int num)
{
  return _zBitmapInfrate( src, dest, 1, num );
}

/* defrate a bitmap. */
int zBitmapDefrate(const zBitmap *src, zBitmap *dest, int num)
{
  return _zBitmapInfrate( src, dest, 0, num );
}

/* skeletonization */

static int _zBitmapSkeletonizeGetNeighbors(const zBitmap *bitmap, uint x, uint y, int index, ubyte *neighbors)
{
  if( zBitmapXPosIsValid( bitmap, x ) && zBitmapGetBit( bitmap, x, y ) ){
    *neighbors |= 0x1 << index;
    return 1;
  }
  return 0;
}

static int _zBitmapSkeletonizeN(const zBitmap *bitmap, uint x, uint y, ubyte *neighbors)
{
  int x1, x2, y1, y2;
  int count_n = 0;

  x1 = x - 1;
  x2 = x + 1;
  y1 = y - 1;
  y2 = y + 1;
  *neighbors = 0;
  if( zBitmapYPosIsValid( bitmap, y1 ) ){
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x1, y1, 7, neighbors );
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x , y1, 0, neighbors );
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x2, y1, 1, neighbors );
  }
  if( zBitmapYPosIsValid( bitmap, y ) ){
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x1, y , 6, neighbors );
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x2, y , 2, neighbors );
  }
  if( zBitmapYPosIsValid( bitmap, y2 ) ){
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x1, y2 , 5, neighbors );
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x , y2 , 4, neighbors );
    count_n += _zBitmapSkeletonizeGetNeighbors( bitmap, x2, y2 , 3, neighbors );
  }
  return count_n;
}

static int _zBitmapSkeletonizeS(ubyte neighbors)
{
  ubyte mask, pat;
  int count_s;

  for( count_s=0, mask=0x81, pat=0x1; pat!=0; mask=bit_rotate(mask,8,1), pat<<=1 )
    count_s += ( bit_cmp( neighbors, pat ) & mask ) == mask ? 1 : 0;
  return count_s;
}

static bool _zBitmapSkeletonizeCheck(const zBitmap *src, zBitmap *dest, uint j, uint i, ubyte pat1, ubyte pat2)
{
  int count_n;
  ubyte neighbors;

  if( zBitmapGetBit( src, j, i ) == 0 ) return false;
  count_n = _zBitmapSkeletonizeN( src, j, i, &neighbors );
  if( count_n < 2 || count_n > 6 ) return false;
  if( _zBitmapSkeletonizeS( neighbors ) != 1 ) return false;
  if( ( neighbors & pat1 ) == pat1 || ( neighbors & pat2 ) == pat2 ) return false;
  return true;
}

static int _zBitmapSkeletonizeOne(const zBitmap *src, zBitmap *dest, ubyte pat1, ubyte pat2)
{
  uint i, j;
  int count = 0;

  for( i=0; i<src->height; i++ )
    for( j=0; j<src->width; j++ )
      if( _zBitmapSkeletonizeCheck( src, dest, j, i, pat1, pat2 ) ){
        zBitmapPutBit( dest, j, i, 0 );
        count += zBitmapGetBit( src, j, i );
      } else
        zBitmapPutBit( dest, j, i, zBitmapGetBit( src, j, i ) );
  return count;
}

/* Skeletonize a bitmap.
 * The original algorithm was proposed by Zhang and Suen in the following paper:
 * T. Y. Zhang and C. Y. Suen, "A Fast Parallel Algorithm for Thinning Digital Patterns,"
 * Image Processing and Computer Vision, Communications of the ACM, 27(3):236--239 (1984).
 */
zBitmap *zBitmapSkeletonize(const zBitmap *src, zBitmap *dest)
{
  zBitmap tmp;

  if( !zBitmapAlloc( &tmp, dest->width, dest->height ) ) return NULL;
  zBitmapCopy( src, dest );
  while( 1 ){
    _zBitmapSkeletonizeOne( dest, &tmp, 0x15, 0x54 );
    if( _zBitmapSkeletonizeOne( &tmp, dest, 0x45, 0x51 ) == 0 ) break;
  }
  zBitmapDestroy( &tmp );
  return dest;
}

/* I/O */

/* read a bitmap from a pattern represented by charactors. */
zBitmap *zBitmapRead(zBitmap *bitmap, const char *pattern[], char c)
{
  uint i, j, width, height;

  /* get size of the bitmap. */
  for( width=0, height=0, i=0; pattern[i]; i++, height++ ){
    for( j=0; pattern[i][j]; j++ );
    if( j > width ) width = j;
  }
  if( !zBitmapAlloc( bitmap, width, height ) ) return NULL;
  for( i=0; i<height; i++ )
    for( j=0; j<width; j++ )
      zBitmapPutBit( bitmap, j, i, pattern[i][j] == c ? 1 : 0 );
  return bitmap;
}

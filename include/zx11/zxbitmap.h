/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxbitmap - bitmap manipulation.
 */

#ifndef __ZX_BITMAP_H__
#define __ZX_BITMAP_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \struct zBitmap
 * \brief binary image buffer class.
 */
typedef struct{
  uint width;  /*! \brief width of image  */
  uint height; /*! \brief height of image */
  /*! \cond */
  uint byte_per_line;
  /*! \endcond */
  ubyte *buf;  /*! \brief buffer */
} zBitmap;

/*! \brief initialize a bitmap. */
zBitmap *zBitmapInit(zBitmap *bitmap);

/*! \brief allocate a bitmap. */
zBitmap *zBitmapAlloc(zBitmap *bitmap, uint width, uint height);

/*! \brief destroy a bitmap. */
zBitmap *zBitmapDestroy(zBitmap *bitmap);

/*! \brief copy a bitmap. */
zBitmap *zBitmapCopy(const zBitmap *src, zBitmap *dest);

/*! \brief clone a bitmap. */
zBitmap *zBitmapClone(const zBitmap *src, zBitmap *dest);

/*! \brief get a bit of a bitmap. */
ubyte zBitmapGetBit(const zBitmap *bitmap, uint x, uint y);

/*! \brief put a bit on a bitmap. */
ubyte zBitmapPutBit(zBitmap *bitmap, uint x, uint y, ubyte val);

/*! \brief fill a bitmap with a specified value. */
zBitmap *zBitmapFill(zBitmap *bitmap, ubyte val);

/*! \brief convert a bitmap to an image. */
zxImage *zBitmapToImage(const zBitmap *src, zxImage *dest);

/*! \brief convert an image to a bitmap. */
zBitmap *zxImageToBitmap(const zxImage *src, zBitmap *dest, ubyte threshold);

/* infration and defration of a bitmap. */

/*! \brief infrate a bitmap one-step. */
int zBitmapInfrateOne(const zBitmap *src, zBitmap *dest);

/*! \brief defrate a bitmap one-step. */
int zBitmapDefrateOne(const zBitmap *src, zBitmap *dest);

/*! \brief infrate a bitmap. */
int zBitmapInfrate(const zBitmap *src, zBitmap *dest, int num);

/*! \brief defrate a bitmap. */
int zBitmapDefrate(const zBitmap *src, zBitmap *dest, int num);

/*! \brief Skeletonize a bitmap.
 * The original algorithm was proposed by Zhang and Suen in the following paper:
 * T. Y. Zhang and C. Y. Suen, "A Fast Parallel Algorithm for Thinning Digital Patterns,"
 * Image Processing and Computer Vision, Communications of the ACM, 27(3):236--239 (1984).
 */
zBitmap *zBitmapSkeletonize(const zBitmap *src, zBitmap *dest);

/* I/O */

/*! \brief read a bitmap from a pattern represented by charactors. */
zBitmap *zBitmapRead(zBitmap *bitmap, const char *pattern[], char c);

__END_DECLS

#endif /* __ZX_BITMAP_H__ */

/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage - image manipulation on X11.
 */

#ifndef __ZX_IMAGE_H__
#define __ZX_IMAGE_H__

#include <zx11/zxpixel.h>

__BEGIN_DECLS

#define ZX_BIT_PER_BYTE 8

/*! \struct zxImage
 * \brief image buffer class.
 */
typedef struct{
  uint width;       /*! \brief width of image  */
  uint height;      /*! \brief height of image */
  ubyte bpp;        /*! \brief byte per pixel */
  zxPixelManip *pm; /*! \brief pixel manipulator */
  ubyte *buf;       /*! \brief buffer */
  ubyte *mask_buf;  /*! \brief mask buffer */
} zxImage;

/* pixel manipulation */

#define zxImageXPosIsValid(img,x)  ( (x) >= 0 && (x) < (img)->width )
#define zxImageYPosIsValid(img,y)  ( (y) >= 0 && (y) < (img)->height )
#define zxImagePosIsValid(img,x,y) ( zxImageXPosIsValid(img,x) && zxImageYPosIsValid(img,y) )

#define zxImageAddr(img,x,y) ( (img)->buf + (img)->bpp * ( (img)->width * (y) + (x) ) )
#define zxImageCell(img,x,y) *zxImageAddr(img,x,y)

zxPixel zxImageCellPixel(const zxImage *img, uint x, uint y);
zxPixel zxImageCellPixelCheck(const zxImage *img, uint x, uint y);
void zxImageCellFromPixel(zxImage *img, uint x, uint y, zxPixel pixel);
void zxImageCellFromPixelCheck(zxImage *img, uint x, uint y, zxPixel pixel);

#define zxImageCellRGB(img,x,y,r,g,b) \
  (img)->pm->PixelRGB( zxImageCellPixel(img,x,y), r, g, b )
#define zxImageCellFRGB(img,x,y,r,g,b) \
  (img)->pm->PixelFRGB( zxImageCellPixel(img,x,y), r, g, b )
ubyte zxImageCellGS(const zxImage *img, uint x, uint y);
#define zxImageCellFromRGB(img,x,y,r,g,b) \
  zxImageCellFromPixel( img, x, y, (img)->pm->PixelFromRGB(r,g,b) )
#define zxImageCellFromGS(img,x,y,v) \
  zxImageCellFromPixel( img, x, y, zxPixelFromGS((img)->pm,v) )
#define zxImageCellFromFRGB(img,x,y,r,g,b) \
  zxImageCellFromPixel( img, x, y, (img)->pm->PixelFromFRGB(r,g,b) )
#define zxImageCellNegate(img,x,y) \
  zxPixelNegate( (img)->pm, zxImageCellPixel(img,x,y) )

#define zxImageMaskAddr(img,x,y)  ( (img)->mask_buf + ( (img)->width * (y) + (x) ) )
#define zxImageMask(img,x,y)      *zxImageMaskAddr(img,x,y)
#define zxImageSetMask(img,x,y,m) ( zxImageMask(img,x,y) = (m) )

/* buffer manipulation */

zxImage *zxImageInit(zxImage *img);
zxImage *zxImageAlloc(zxImage *img, uint width, uint height, ubyte depth);
#define zxImageAllocDefault(img,w,h)  zxImageAlloc( img, w, h, zxdepth )
zxImage *zxImageAllocMask(zxImage *img);
zxImage *zxImageCreateMask(zxImage *img, zxPixel mask);
#define zxImageCreateMaskDefault(img) zxImageCreateMask( img, WhitePixel( zxdisplay, 0 ) )
zxImage *zxImageDestroy(zxImage *img);
zxImage *zxImageClear(zxImage *img);
zxImage *zxImageFill(zxImage *img, zxPixel pixel);
bool zxImageSizeEqual(const zxImage *img1, const zxImage *img2);
zxImage *zxImageCopy(const zxImage *src, zxImage *dest);
zxImage *zxImageClone(const zxImage *src, zxImage *dest);

bool zxImageCmp(const zxImage *img1, const zxImage *img2);

/* image pasting */

/* find valid range of an image to manipulate. */
bool zxCanvasRange(uint width_dest, uint height_dest, uint width_src, uint height_src, uint x, uint y, uint *w, uint *h);
bool zxImageCanvasRange(const zxImage *canvas, const zxImage *img, uint x, uint y, uint *w, uint *h);

zxImage *zxImagePut(zxImage *canvas, const zxImage *img, uint x, uint y);
zxImage *zxImagePutMasked(zxImage *canvas, const zxImage *img, uint x, uint y, zxPixel mask);
zxImage *zxImagePutAlphaBlend(zxImage *canvas, const zxImage *img1, const zxImage *img2, uint x, uint y, double alpha);
zxImage *zxImagePutAlphaBlendMasked(zxImage *canvas, const zxImage *img1, const zxImage *img2, uint x, uint y, double alpha, zxPixel mask);
zxImage *zxImagePutSuperimpose(zxImage *canvas, const zxImage *img, uint x, uint y);
#define zxImagePutSuperimposeDefault(c,img) zxImagePutSuperimpose( c, img, 0, 0 )

zxImage *zxImageGet(const zxImage *src, zxImage *dest, uint x, uint y);

/* geometric manipulations */

zxImage *zxImageVertFlip(const zxImage *src, zxImage *dest);
zxImage *zxImageHorizFlip(const zxImage *src, zxImage *dest);
zxImage *zxImageRotRight(const zxImage *src, zxImage *dest);
zxImage *zxImageRotLeft(const zxImage *src, zxImage *dest);
zxImage *zxImageRot(const zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double theta);
zxImage *zxImageRotFilt(const zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double theta);

zxImage *zxImageResize(const zxImage *src, zxImage *dest);

/* color manipulations */

const zxImage *zxImageAbstRGB(const zxImage *src, zxImage *rimg, zxImage *gimg, zxImage *bimg);
zxImage *zxImageGrayscalize(const zxImage *src, zxImage *dest);
zxImage *zxImageBinarize(const zxImage *src, zxImage *dest, ubyte threshold);
zxImage *zxImageNegate(const zxImage *src, zxImage *dest);

#define zxImageGrayscalizeDRC(img)        zxImageGrayscalize( img, img )
#define zxImageBinarizeDRC(img)           zxImageBinarize( img, img );
#define zxImageNegateDRC(img)             zxImageNegate( img, img )

zxImage *zxImageBrighten(const zxImage *src, zxImage *dest, double rate);
zxImage *zxImageContrast(const zxImage *src, zxImage *dest, double rate);
zxImage *zxImageCorrectGamma(const zxImage *src, zxImage *dest, double gamma);

#define zxImageBrightenDRC(img,rate)      zxImageBrighten( img, img, rate )
#define zxImageContrastDRC(img,rate)      zxImageContrast( img, img, rate )
#define zxImageCorrectGammaDRC(img,gamma) zxImageCorrectGamma( img, img, gamma )

zxImage *zxImageNormalize(const zxImage *src, zxImage *dest);
zxImage *zxImageEqualize(const zxImage *src, zxImage *dest);

#define zxImageNormalizeDRC(img)          zxImageNormalize( img, img )
#define zxImageEqualizeDRC(img)           zxImageEqualize( img, img )

zxImage *zxImageDitherBayer(const zxImage *src, zxImage *dest);
zxImage *zxImageDitherNet(const zxImage *src, zxImage *dest);
zxImage *zxImageDitherSpiral(const zxImage *src, zxImage *dest);
#define zxImageDither(src,dest,pattern) zxImageDither##pattern( src, dest )

#define zxImageDitherBayerDRC(img)        zxImageDitherBayer( img, img )
#define zxImageDitherNetDRC(img)          zxImageDitherNet( img, img )
#define zxImageDitherSpiralDRC(img)       zxImageDitherSpiral( img, img )
#define zxImageDitherDRC(img,pattern)     zxImageDither##pattern##DRC( img )

/* dither based on error diffusion with Floyd-Steinberg distribution. */
zxImage *zxImageDitherErrorDiffusionDRC(zxImage *img);
zxImage *zxImageDitherErrorDiffusion(const zxImage *src, zxImage *dest);

/* general filter */

zxImage *zxImageFilter(const zxImage *src, zxImage *dest, double f[], int size);
zxImage *zxImageFilter2(const zxImage *src, zxImage *dest, double f1[], double f2[], int size);

/* smoothing */

zxImage *zxImageSmoothMedian(const zxImage *src, zxImage *dest, int size);
zxImage *zxImageSmoothMin(const zxImage *src, zxImage *dest, int size);
zxImage *zxImageSmoothMax(const zxImage *src, zxImage *dest, int size);
zxImage *zxImageSmoothGaussian(const zxImage *src, zxImage *dest);
zxImage *zxImageAntialias(const zxImage *src, zxImage *dest);
zxImage *zxImageSmoothBilateral(const zxImage *src, zxImage *dest);

/* edge detection */

zxImage *zxImageDiff(const zxImage *src, zxImage *dest);
zxImage *zxImageIntegral(const zxImage *src, zxImage *dest);

zxImage *zxImageEdgePrewittH(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgePrewittV(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgePrewitt(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgeSobelH(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgeSobelV(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgeSobel(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgeLaplacian(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgeRoberts(const zxImage *src, zxImage *dest);
zxImage *zxImageEdgeForsen(const zxImage *src, zxImage *dest);

/* Hough transformation */

/*! \struct zxHoughBinData
 * \brief bin class for Hough transformation.
 */
typedef struct{
  double theta;
  double dist;
  int count;
} zxHoughBinData;
ZEDA_DEF_LIST_CLASS( zxHoughBinList, zxHoughBinListCell, zxHoughBinData );

zxHoughBinList *zxImageHoughLines(zxHoughBinList *bin_list, const zxImage *src, uint theta_div, uint dist_div);

/* normal map */

#define _zxNormalize(x) ( 0.5 * ( (x) + 1.0 ) )
void zxImageNormalVec(const zxImage *img, double depth, uint j, uint i, double *x, double *y, double *z);
zxImage *zxImageNormalMap(const zxImage *src, double depth, zxImage *dest);

/* special effect */

zxImage *zxImagePutRect(zxImage *img, uint x, uint y, uint width, uint height, ubyte red, ubyte green, ubyte blue);
zxImage *zxImageMosaic(zxImage *img, uint x, uint y, uint w, uint h, uint nx, uint ny);

/* drawing */

Pixmap zxImageToPixmap(zxWindow *win, const zxImage *img, Pixmap pmap, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageToPixmapAll(win,img,pixmap) \
  zxImageToPixmap( (win), (img), (pixmap), 0, 0, (img)->width, (img)->height, 0, 0 )
Pixmap zxImageCreatePixmap(zxWindow *win, const zxImage *img);
Pixmap zxImageCreatePixmapMask(zxWindow *win, const zxImage *img);

void zxImageDraw(zxWindow *win, const zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageDrawAll(win,img,x,y) \
  zxImageDraw( (win), (img), 0, 0, (img)->width, (img)->height, (x), (y) )

bool zxImageDrawMask(zxWindow *win, const zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageDrawMaskAll(win,img,x,y) \
  zxImageDrawMask( (win), (img), 0, 0, (img)->width, (img)->height, (x), (y) )

zxImage *zxImageFromPixmap(zxImage *img, Pixmap pmap, uint w, uint h);

/* file format checker and generalized I/O */

bool zxImageFileIdent(const char *filename, const unsigned char ident[], uint size);

int zxImageReadFile(zxImage *img, const char *filename);
int zxImageWriteFile(const zxImage *img, const char *filename);

__END_DECLS

/* Freetype2 support available together with Xft */
#ifdef __ZX11_USE_XFT
#include <zx11/zximage_freetype.h>
#endif /* __ZX11_USE_XFT */

/* MagickWand support available */
#ifdef __ZX11_USE_MAGICKWAND
# include <zx11/zximage_magickwand.h>
#endif /* __ZX11_USE_MAGICKWAND */

#include <zx11/zximage_supported.h>

/* bitmap manipulation */
#include <zx11/zxbitmap.h>

#endif /* __ZX_IMAGE_H__ */

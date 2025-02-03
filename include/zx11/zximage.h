/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage - image manipulation on X11
 */

#ifndef __ZX_IMAGE_H__
#define __ZX_IMAGE_H__

#include <zx11/zxpixel.h>

__BEGIN_DECLS

#define ZX_BIT_PER_BYTE 8

/* ********************************************************** */
/* CLASS: zxImage
 * image buffer
 * ********************************************************** */

typedef struct{
  uint width, height;
  ubyte bpp; /* byte per pixel */
  zxPixelManip *pm; /* pixel manipulator */
  ubyte *buf;
  ubyte *mask_buf; /* mask buffer */
} zxImage;

/* pixel manipulation */

#define zxImagePosIsValid(img,x,y) \
  ( (x) < (img)->width && (y) < (img)->height )

#define zxImageAddr(img,x,y) ((img)->buf+(img)->bpp*((img)->width*(y)+(x)))
#define zxImageCell(img,x,y) *zxImageAddr(img,x,y)

zxPixel zxImageCellPixel(zxImage *img, uint x, uint y);
zxPixel zxImageCellPixelCheck(zxImage *img, uint x, uint y);
void zxImageCellFromPixel(zxImage *img, uint x, uint y, zxPixel pixel);
void zxImageCellFromPixelCheck(zxImage *img, uint x, uint y, zxPixel pixel);

#define zxImageCellRGB(img,x,y,r,g,b) \
  (img)->pm->PixelRGB( zxImageCellPixel(img,x,y), r, g, b )
#define zxImageCellFRGB(img,x,y,r,g,b) \
  (img)->pm->PixelFRGB( zxImageCellPixel(img,x,y), r, g, b )
ubyte zxImageCellGS(zxImage *img, uint x, uint y);
#define zxImageCellFromRGB(img,x,y,r,g,b) \
  zxImageCellFromPixel( img, x, y, (img)->pm->PixelFromRGB(r,g,b) )
#define zxImageCellFromGS(img,x,y,v) \
  zxImageCellFromPixel( img, x, y, zxPixelFromGS((img)->pm,v) )
#define zxImageCellFromFRGB(img,x,y,r,g,b) \
  zxImageCellFromPixel( img, x, y, (img)->pm->PixelFromFRGB(r,g,b) )
#define zxImageCellNegate(img,x,y) \
  zxPixelNegate( (img)->pm, zxImageCellPixel(img,x,y) )

#define zxImageMaskAddr(img,x,y)  ((img)->mask_buf+((img)->width*(y)+(x)))
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
bool zxImageSizeEqual(zxImage *img1, zxImage *img2);
zxImage *zxImageCopy(zxImage *src, zxImage *dest);
zxImage *zxImageClone(zxImage *src, zxImage *dest);

bool zxImageCmp(zxImage *img1, zxImage *img2);

/* image pasting */

void zxImageCanvasRange(zxImage *canvas, zxImage *img, uint x, uint y, uint *w, uint *h);

zxImage *zxImagePut(zxImage *canvas, zxImage *img, uint x, uint y);
zxImage *zxImagePutMasked(zxImage *canvas, zxImage *img, uint x, uint y, zxPixel mask);
zxImage *zxImagePutAlphaBlend(zxImage *canvas, zxImage *img1, zxImage *img2, uint x, uint y, double alpha);
zxImage *zxImagePutAlphaBlendMasked(zxImage *canvas, zxImage *img1, zxImage *img2, uint x, uint y, double alpha, zxPixel mask);
zxImage *zxImagePutSuperimpose(zxImage *canvas, zxImage *img, uint x, uint y);
#define zxImagePutSuperimposeDefault(c,img) zxImagePutSuperimpose( c, img, 0, 0 )

zxImage *zxImageGet(zxImage *src, zxImage *dest, uint x, uint y);

/* image manipulation */

zxImage *zxImageVertFlip(zxImage *src, zxImage *dest);
zxImage *zxImageHorizFlip(zxImage *src, zxImage *dest);
zxImage *zxImageRotRight(zxImage *src, zxImage *dest);
zxImage *zxImageRotLeft(zxImage *src, zxImage *dest);
zxImage *zxImageRot(zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double theta);
zxImage *zxImageRotFilt(zxImage *src, zxImage *dest, uint ox, uint oy, uint x, uint y, double theta);

zxImage *zxImageResize(zxImage *src, zxImage *dest);

/* color manipulation */

zxImage *zxImageAbstRGB(zxImage *src, zxImage *rimg, zxImage *gimg, zxImage *bimg);
zxImage *zxImageGrayscalize(zxImage *src, zxImage *dest);
zxImage *zxImageNegate(zxImage *src, zxImage *dest);

zxImage *zxImageBrighten(zxImage *src, zxImage *dest, double rate);
zxImage *zxImageContrast(zxImage *src, zxImage *dest, double rate);
zxImage *zxImageCorrectGamma(zxImage *src, zxImage *dest, double gamma);

zxImage *zxImageNormalize(zxImage *src, zxImage *dest);
zxImage *zxImageEqualize(zxImage *src, zxImage *dest);

zxImage *zxImageDither(zxImage *src, zxImage *dest);

#define zxImageGrayscalizeDRC(img)   zxImageGrayscalize( img, img )
#define zxImageNegateDRC(img)        zxImageNegate( img, img )

#define zxImageBrightenDRC(img,rate)      zxImageBrighten( img, img, rate )
#define zxImageContrastDRC(img,rate)      zxImageContrast( img, img, rate )
#define zxImageCorrectGammaDRC(img,gamma) zxImageCorrectGamma( img, img, gamma )

#define zxImageNormalizeDRC(img)     zxImageNormalize( img, img )
#define zxImageEqualizeDRC(img)      zxImageEqualize( img, img )
#define zxImageDitherDRC(img)        zxImageDither( img, img )

/* general filter */

zxImage *zxImageFilter(zxImage *src, zxImage *dest, double f[], int size);
zxImage *zxImageFilter2(zxImage *src, zxImage *dest, double f1[], double f2[], int size);

/* blur */

zxImage *zxImageMedian(zxImage *src, zxImage *dest, int size);
zxImage *zxImageAntialias(zxImage *src, zxImage *dest);
zxImage *zxImageGaussian(zxImage *src, zxImage *dest);

/* edge detection */

zxImage *zxImageDiff(zxImage *src, zxImage *dest);
zxImage *zxImageIntegral(zxImage *src, zxImage *dest);
zxImage *zxImagePrewittH(zxImage *src, zxImage *dest);
zxImage *zxImagePrewittV(zxImage *src, zxImage *dest);
zxImage *zxImagePrewitt(zxImage *src, zxImage *dest);
zxImage *zxImageSobelH(zxImage *src, zxImage *dest);
zxImage *zxImageSobelV(zxImage *src, zxImage *dest);
zxImage *zxImageSobel(zxImage *src, zxImage *dest);
zxImage *zxImageLaplacian(zxImage *src, zxImage *dest);

/* Hough transformation */

typedef struct{
  double theta;
  double dist;
  int count;
} zxHoughBinData;
zListClass( zxHoughBinList, zxHoughBinListCell, zxHoughBinData );

zxHoughBinList *zxImageHoughLines(zxHoughBinList *bin_list, zxImage *src, uint theta_div, uint dist_div);

/* normal map */

#define _zxNormalize(x) ( 0.5 * ( (x) + 1.0 ) )
void zxImageNormalVec(zxImage *img, double depth, uint j, uint i, double *x, double *y, double *z);
zxImage *zxImageNormalMap(zxImage *src, double depth, zxImage *dest);

/* special effect */

zxImage *zxImageMosaic(zxImage *img, uint x, uint y, uint w, uint h, uint nx, uint ny);

/* drawing */

Pixmap zxImageToPixmap(zxWindow *win, zxImage *img, Pixmap pmap, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageToPixmapAll(win,img,pixmap) \
  zxImageToPixmap( (win), (img), (pixmap), 0, 0, (img)->width, (img)->height, 0, 0 )
Pixmap zxImageCreatePixmap(zxWindow *win, zxImage *img);
Pixmap zxImageCreatePixmapMask(zxWindow *win, zxImage *img);

void zxImageDraw(zxWindow *win, zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageDrawAll(win,img,x,y) \
  zxImageDraw( (win), (img), 0, 0, (img)->width, (img)->height, (x), (y) )

bool zxImageDrawMask(zxWindow *win, zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageDrawMaskAll(win,img,x,y) \
  zxImageDrawMask( (win), (img), 0, 0, (img)->width, (img)->height, (x), (y) )

zxImage *zxImageFromPixmap(zxImage *img, Pixmap pmap, uint w, uint h);

/* file format checker and generalized I/O */

bool zxImageFileIdent(const char *filename, const unsigned char ident[], uint size);

int zxImageReadFile(zxImage *img, const char *filename);
int zxImageWriteFile(zxImage *img, const char *filename);

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

#endif /* __ZX_IMAGE_H__ */

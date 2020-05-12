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
  ubyte *buf;
  ubyte *mask_buf; /* mask buffer */
} zxImage;

/* pixel manipulation */

#define zxImagePosIsValid(i,x,y) \
  ( (x)<(i)->width && (y)<(i)->height )

#define zxImageAddr(i,x,y) ((i)->buf+(i)->bpp*((i)->width*(y)+(x)))
#define zxImageCell(i,x,y) *zxImageAddr(i,x,y)

zxPixel zxImageCellPixel(zxImage *img, uint x, uint y);
zxPixel zxImageCellPixelCheck(zxImage *img, uint x, uint y);
void zxImageCellFromPixel(zxImage *img, uint x, uint y, zxPixel pixel);
void zxImageCellFromPixelCheck(zxImage *img, uint x, uint y, zxPixel pixel);

#define zxImageCellRGB(i,p,x,y,r,g,b) \
  (p)->PixelRGB( zxImageCellPixel(i,x,y), r, g, b )
#define zxImageCellFromRGB(i,p,x,y,r,g,b) \
  zxImageCellFromPixel( i, x, y, (p)->PixelFromRGB(r,g,b) )
#define zxImageCellFromGS(i,p,x,y,v) \
  zxImageCellFromPixel( i, x, y, zxPixelFromGS(p,v) )
#define zxImageCellNegate(i,p,x,y) \
  zxPixelNegate( p, zxImageCellPixel(i,x,y) )

#define zxImageMaskAddr(i,x,y)  ((i)->mask_buf+((i)->width*(y)+(x)))
#define zxImageMask(i,x,y)      *zxImageMaskAddr(i,x,y)
#define zxImageSetMask(i,x,y,m) ( zxImageMask(i,x,y) = (m) )

ubyte zxImageBPP(ubyte depth);
#define zxImageBPPDefault() zxImageBPP( zxdepth )

/* buffer manipulation */

zxImage *zxImageInit(zxImage *img);
zxImage *zxImageAlloc(zxImage *img, uint width, uint height, ubyte depth);
#define zxImageAllocDefault(i,w,h)  zxImageAlloc( i, w, h, zxdepth )
zxImage *zxImageAllocMask(zxImage *img);
zxImage *zxImageCreateMask(zxImage *img, zxPixel mask);
#define zxImageCreateMaskDefault(i) \
  zxImageCreateMask( i, WhitePixel( zxdisplay, 0 ) )
zxImage *zxImageDestroy(zxImage *img);
zxImage *zxImageClear(zxImage *img);
zxImage *zxImageFill(zxImage *img, zxPixel pixel);
int zxImageSizeEqual(zxImage *d1, zxImage *d2);
zxImage *zxImageCopy(zxImage *src, zxImage *dest);
zxImage *zxImageClone(zxImage *src, zxImage *dest);

/* image pasting */

void zxImageCanvasRange(zxImage *canvas, zxImage *img, uint x, uint y, uint *w, uint *h);

zxImage *zxImagePut(zxImage *canvas, zxImage *img, uint x, uint y);
zxImage *zxImagePutMasked(zxImage *canvas, zxImage *img, uint x, uint y, zxPixel mask);
zxImage *zxImagePutAlphaBlend(zxImage *canvas, zxImage *img1, zxImage *img2, uint x, uint y, double alpha);
zxImage *zxImagePutAlphaBlendMasked(zxImage *canvas, zxImage *img1, zxImage *img2, uint x, uint y, double alpha, zxPixel mask);
zxImage *zxImagePutSuperimpose(zxImage *canvas, zxImage *img, uint x, uint y);
#define zxImagePutSuperimposeDefault(c,i) zxImagePutSuperimpose( c, i, 0, 0 )

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

zxImage *zxImageAntialias(zxImage *src, zxImage *dest);
zxImage *zxImageGrayscalize(zxImage *src, zxImage *dest);
zxImage *zxImageDiff(zxImage *src, zxImage *dest);
zxImage *zxImageIntegral(zxImage *src, zxImage *dest);
zxImage *zxImageNegate(zxImage *src, zxImage *dest);
zxImage *zxImageToneDown(zxImage *src, zxImage *dest, double rate);

/* special effect */

zxPixel zxImageAverage(zxImage *img, zxPixelManip *pm, uint x, uint y, uint w, uint h);
zxImage *zxImageAveragate(zxImage *img, zxPixelManip *pm, uint x, uint y, uint w, uint h);
zxImage *zxImageMosaic(zxImage *img, uint x, uint y, uint w, uint h, uint nx, uint ny);

/* drawing */

Pixmap zxImageToPixmap(zxWindow *win, zxImage *img, Pixmap pmap, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageToPixmapAll(win,i,p) \
  zxImageToPixmap( (win), (i), (p), 0, 0, (i)->width, (i)->height, 0, 0 )
Pixmap zxImageCreatePixmap(zxWindow *win, zxImage *img);
Pixmap zxImageCreatePixmapMask(zxWindow *win, zxImage *img);
void zxImageDraw(zxWindow *win, zxImage *img, uint src_x, uint src_y, uint w, uint h, uint dest_x, uint dest_y);
#define zxImageDrawAll(win,i,x,y) \
  zxImageDraw( (win), (i), 0, 0, (i)->width, (i)->height, (x), (y) )

zxImage *zxImageFromPixmap(zxImage *img, Pixmap pmap, uint w, uint h);

/* file format checker and generalized I/O */

bool zxImageFileIdent(char filename[], const char ident[], int size);

int zxImageReadFile(zxImage *img, char filename[]);

__END_DECLS

#endif /* __ZX_IMAGE_H__ */

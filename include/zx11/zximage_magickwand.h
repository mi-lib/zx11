/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_magickwand - MagickWand (C API of ImageMagick) support.
 */

#ifndef __ZX_IMAGE_MAGICKWAND_H__
#define __ZX_IMAGE_MAGICKWAND_H__

#include <zx11/zximage.h>
#include <wand/MagickWand.h>

__BEGIN_DECLS

zxImage *zxImageFromMagickWand(zxImage *img, MagickWand *wand);
MagickWand *zxImageToMagickWand(zxImage *img, MagickWand *wand);

int zxImageReadFileMagickWand(zxImage *img, const char *filename);
int zxImageWriteFileMagickWand(zxImage *img, const char *filename);

__END_DECLS

#endif /* __ZX_IMAGE_MAGICKWAND_H__ */

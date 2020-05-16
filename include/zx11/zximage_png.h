/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_png - PNG support on zxImage (requires libpng).
 */

#ifndef __ZX_IMAGE_PNG_H__
#define __ZX_IMAGE_PNG_H__

#include <zx11/zximage.h>
#include <png.h>

__BEGIN_DECLS

#define ZX_PNG_SUFFIX "png"

bool zxImageFileIsPNG(char filename[]);

int zxPNGCheck(FILE *fp);
int zxPNGCheckFile(const char filename[]);

int zxImageReadPNG(FILE *fp, zxImage *img);
int zxImageReadPNGFile(zxImage *img, char filename[]);

int zxImageWritePNG(FILE *fp, zxImage *img);
int zxImageWritePNGFile(zxImage *img, char filename[]);

__END_DECLS

#endif /* __ZX_IMAGE_PNG_H__ */

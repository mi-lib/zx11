/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_dib - Microsoft BMP(Device Independent Bitmap, DIB) support.
 */

#ifndef __ZX_IMAGE_DIB_H__
#define __ZX_IMAGE_DIB_H__

#include <zx11/zximage.h>

__BEGIN_DECLS

#define ZX_DIB_SUFFIX "bmp"

bool zxImageFileIsBMP(char filename[]);

int zxImageReadBMP(FILE *fp, zxImage *img);
int zxImageReadBMPFile(zxImage *img, char filename[]);

int zxImageWriteBMP(FILE *fp, zxImage *img);
int zxImageWriteBMPFile(zxImage *img, char filename[]);

__END_DECLS

#endif /* __ZX_IMAGE_DIB_H__ */

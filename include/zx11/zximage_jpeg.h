/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_jpeg - JPEG support (requires libjpeg)
 */

#ifndef __ZX_IMAGE_JPEG_H__
#define __ZX_IMAGE_JPEG_H__

#include <zx11/zximage.h>
#include <jpeglib.h>
#include <jerror.h>

__BEGIN_DECLS

#define ZX_JPEG_SUFFIX "jpg"

#define ZX_JPEG_DEFAULT_QUALITY 75

bool zxImageFileIsJPEG(char filename[]);

int zxImageReadJPEG(FILE *fp, zxImage *img);
int zxImageReadJPEGFile(zxImage *img, char filename[]);

int zxImageWriteJPEG(FILE *fp, zxImage *img, int quality);
int zxImageWriteJPEGFile(zxImage *img, char filename[], int quality);

__END_DECLS

#endif /* __ZX_IMAGE_JPEG_H__ */

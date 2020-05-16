/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_mag - MAG (MA_KIchan Graphic format) support.
 */

#ifndef __ZX_IMAGE_MAG_H__
#define __ZX_IMAGE_MAG_H__

#include <zx11/zximage.h>

__BEGIN_DECLS

#define ZX_MAG_SUFFIX "mag"

bool zxImageFileIsMAG(char filename[]);

int zxMAGDispComment(FILE *fp);
int zxMAGDispCommentFile(const char filename[]);

int zxImageReadMAG(FILE *fp, zxImage *img);
int zxImageReadMAGFile(zxImage *img, char filename[]);

__END_DECLS

#endif /* __ZX_IMAGE_MAG_H__ */

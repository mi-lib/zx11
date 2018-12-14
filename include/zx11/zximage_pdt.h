/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_pdt - PDT(graphic format for AVG32) support.
 *
 * AVG32 is a product by versammeln(an online distribution
 * brand of VisualArt's).
 * Special thanks to JAGARL.
 */

#ifndef __ZX_IMAGE_PDT_H__
#define __ZX_IMAGE_PDT_H__

#include <zx11/zximage.h>

__BEGIN_DECLS

#define ZX_PDT_SUFFIX "pdt"

bool zxImageFileIsPDT(char filename[]);

bool zxImageReadPDT(FILE *fp, zxImage *img);
int zxImageReadPDTFile(zxImage *img, char filename[]);

__END_DECLS

#endif /* __ZX_IMAGE_PDT_H__ */

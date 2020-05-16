/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_tiff - TIFF (Tag Image File Format) support on zxImage (requires libtiff).
 */

#ifndef __ZX_IMAGE_TIFF_H__
#define __ZX_IMAGE_TIFF_H__

#include <zx11/zximage.h>
#include <tiffio.h>

__BEGIN_DECLS

#define ZX_TIFF_SUFFIX "tiff"

#define ZXTIFF_COMPRESSION_DEFAULT COMPRESSION_NONE

#define ZXTIFF_RESOLUTION_DEFAULT 72.0

bool zxImageFileIsTIFF(char filename[]);

int zxImageReadTIFFFile(zxImage *img, char filename[]);
int zxImageWriteTIFFFile(zxImage *img, char filename[], int cmpmethod);

__END_DECLS

#endif /* __ZX_IMAGE_TIFF_H__ */

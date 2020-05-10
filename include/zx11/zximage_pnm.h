/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_pnm - image I/O as Portable Bitmap/Graymap/Pixmap on X11
 */

#ifndef __ZX_IMAGE_PNM_H__
#define __ZX_IMAGE_PNM_H__

#include <zx11/zximage.h>

__BEGIN_DECLS

/* file suffices */
#define ZX_PBM_SUFFIX "pbm"
#define ZX_PGM_SUFFIX "pgm"
#define ZX_PPM_SUFFIX "ppm"

/* file types */
#define ZX_PNM_INVALID 0
#define ZX_PBM_ASCII   1
#define ZX_PBM_BIN     4
#define ZX_PGM_ASCII   2
#define ZX_PGM_BIN     5
#define ZX_PPM_ASCII   3
#define ZX_PPM_BIN     6

/* file format identification */

bool zxImageFileIsPBM(char filename[]);
bool zxImageFileIsPGM(char filename[]);
bool zxImageFileIsPPM(char filename[]);
bool zxImageFileIsPNM(char filename[]);

/* input of PNM file */

int zxImageReadPNMHeader(FILE *fp, zxImage *img);
int zxImageReadPNM(FILE *fp, zxImage *img);
int zxImageReadPNMFile(zxImage *img, char filename[]);

/* output of PNM file */

int zxImageWritePBM(FILE *fp, zxImage *img);
int zxImageWritePBMFile(zxImage *img, char filename[]);

int zxImageWritePGM(FILE *fp, zxImage *img);
int zxImageWritePGMFile(zxImage *img, char filename[]);

int zxImageWritePPM(FILE *fp, zxImage *img);
int zxImageWritePPMFile(zxImage *img, char filename[]);

__END_DECLS

#endif /* __ZX_IMAGE_PNM_H__ */

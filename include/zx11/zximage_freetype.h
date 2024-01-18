/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_freetype - Freetype2 support.
 */

#ifndef __ZX_IMAGE_FREETYPE_H__
#define __ZX_IMAGE_FREETYPE_H__

#include <zx11/zximage.h>

#include <ft2build.h>
#include FT_FREETYPE_H

__BEGIN_DECLS

bool zxFTImageInitLibrary(void);
void zxFTImageExitLibrary(void);

typedef struct{
  FT_Face face;      /* font face */
  FT_Matrix matrix;  /* transformation matrix */
  FT_Vector pen;     /* untransformed origin  */
} zxFTImage;

void zxFTImageSetCoord(zxFTImage *font, double x, double y, double angle);

bool zxFTImageLoadFont(zxFTImage *font, const char *fontfilename, uint size, uint dpi);
void zxFTImageUnloadFont(zxFTImage *font);

void zxImageDrawString(zxImage *img, zxFTImage *font, int x, int y, const uword *string, float red, float green, float blue);

__END_DECLS

#endif /* __ZX_IMAGE_FREETYPE_H__ */

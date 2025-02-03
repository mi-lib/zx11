/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxpixel - pixel conversion
 */

#ifndef __ZX_PIXEL_H__
#define __ZX_PIXEL_H__

#include <zx11/zxutil.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zxPixel
 * RGB pixel
 * ********************************************************** */

typedef uint32_t zxPixel;

typedef struct{
  ubyte b;
  ubyte g;
  ubyte r;
  ubyte none;
} zxPixel24;

typedef struct{
  ubyte alpha;
  ubyte b;
  ubyte g;
  ubyte r;
} zxPixel32;

typedef struct{
  int depth;
  void (*PixelRGB)(zxPixel p, ubyte *r, ubyte *g, ubyte *b);
  void (*PixelFRGB)(zxPixel p, float *r, float *g, float *b);
  zxPixel (*PixelFromRGB)(ubyte r, ubyte g, ubyte b);
  zxPixel (*PixelFromFRGB)(float r, float g, float b);
  zxPixel (*PixelNegate)(zxPixel p);
} zxPixelManip;

zxPixelManip *zxPixelManipFind(int depth);
#define zxPixelManipDefault() zxPixelManipFind( zxdepth )

zxPixel zxPixelConv(zxPixel pixel, zxPixelManip *src, zxPixelManip *dest);
zxPixel zxPixelBlend(zxPixelManip *pm, zxPixel p[], double w[], uint n);
zxPixel zxPixelAlphaBlend(zxPixelManip *pm, zxPixel p1, zxPixel p2, double alpha);
zxPixel zxPixelNorm2(zxPixelManip *pm, zxPixel p1, zxPixel p2);
#define zxPixelNegate(pm,p) (pm)->PixelNegate( (p) )

zxPixel zxPixelFromGS(zxPixelManip *pm, ubyte intensity);
zxPixel zxPixelFromFGS(zxPixelManip *pm, float intensity);

/* strings -> pixel conversion */

zxPixel zxPixelFromHex(char *hex);
zxPixel zxPixelFromStr(zxWindow *win, char *str);

__END_DECLS

#endif /* __ZX_PIXEL_H__ */

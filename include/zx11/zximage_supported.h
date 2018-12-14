/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_supported
 * - header file collection of supported image formats
 */

#ifndef __ZX_IMAGE_SUPPORTED_H__
#define __ZX_IMAGE_SUPPORTED_H__

/* PNM support */
#include <zx11/zximage_pnm.h>

/* PNG support */
#ifdef __ZX11_USE_PNG
# include <zx11/zximage_png.h>
#endif /* __ZX11_USE_PNG */

/* JPEG support */
#ifdef __ZX11_USE_JPEG
# include <zx11/zximage_jpeg.h>
#endif /* __ZX11_USE_JPEG */

/* TIFF support */
#ifdef __ZX11_USE_TIFF
#endif /* __ZX11_USE_TIFF */

/* Microsoft Bitmap(Device Independent Bitmap) support */
#ifdef __ZX11_USE_BMP
# include <zx11/zximage_dib.h>
#endif /* __ZX11_USE_BMP */

/* MAG(MA_KIchan Graphic format) support */
#ifdef __ZX11_USE_MAG
# include <zx11/zximage_mag.h>
#endif /* __ZX11_USE_MAG */

/* PDT(graphic format for AVG32) support */
#ifdef __ZX11_USE_PDT
# include <zx11/zximage_pdt.h>
#endif /* __ZX11_USE_PDT */

#endif /* __ZX_IMAGE_SUPPORTED_H__ */

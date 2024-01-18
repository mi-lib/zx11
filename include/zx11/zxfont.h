/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxfont - font management for X11
 */

#ifndef __ZX_FONT_H__
#define __ZX_FONT_H__

#include <zx11/zxutil.h>
#include <X11/Xlocale.h>
#include <wctype.h>
#include <wchar.h>

__BEGIN_DECLS

/* font & text drawing method */

extern XFontStruct *zxfontstruct;

bool zxFontSet(const char *fontname);
void zxFontUnset(void);
bool zxWindowSetFont(zxWindow *win, const char *fontname);

#define zxTextNWidth(s,n) ( zxfontstruct ? XTextWidth( zxfontstruct, (s), (n) ) : 0 )
#define zxTextWidth(s)    zxTextNWidth( s, strlen(s) )

XCharStruct *zxCharStruct(char c);
int zxCharWidth(char c);
int zxCharHeight(char c);

int zxTextNExtents(const char *str, int n, XCharStruct *cx);
short zxTextNHeight(const char *str, int n, short *ascent, short *descent);
int zxTextNArea(const char *str, int n, short x, short y, zxRegion *reg);

#define zxTextExtents(s,c)   zxTextNExtents( s, strlen(s), c )
#define zxTextHeight(s,a,d)  zxTextNHeight( s, strlen(s), a, d )
#define zxTextArea(s,x,y,r)  zxTextNArea( s, strlen(s), x, y, r )

#define zxDrawNString(win,x,y,s,n) \
  XDrawString( zxdisplay, zxWindowCanvas(win), zxWindowGC(win), (x), (y), (s), (n) )
#define zxDrawImageNString(win,x,y,s,n) \
  XDrawImageString( zxdisplay, zxWindowCanvas(win), zxWindowGC(win), (x), (y), (s), (n) )

#define zxDrawString(win,x,y,s)      zxDrawNString( win, x, y, s, strlen(s) )
#define zxDrawImageString(win,x,y,s) zxDrawImageNString( win, x, y, s, strlen(s) )

/* multibyte-text drawing under i18n */

extern XFontSet zxfontset;
extern XFontSetExtents *zxfontset_extents;

int zxSetLocale(void);
XFontSet zxFontSetCreate(const char *fontname);

#define zxFontName()        XBaseFontNameListOfFontSet( zxfontset )
#define zxLocaleName()      XLocaleOfFontSet( zxfontset )
#define zxTextEscapement(s) XmbTextEscapement( zxfontset, (s), strlen(s) )

#define zxFontWidth()   ( zxfontset_extents->max_logical_extent.width )
#define zxFontHeight()  ( zxfontset_extents->max_logical_extent.height )
#define zxFontAscent()  (-zxfontset_extents->max_logical_extent.y )
#define zxFontDescent() ( zxfontset_extents->max_logical_extent.height - zxFontAscent() )

#define zxWindowSetTitleMB(win,t) XmbSetWMProperties( zxdisplay, zxWindowBody(win), t, NULL, NULL, 0, NULL, NULL, NULL )

void zxTextAreaMB(const char *str, zxRegion *area);
void zxTextAreaWC(const wchar_t *str, zxRegion *area);

#define zxDrawNStringMB(win,x,y,s,n) \
  XmbDrawString( zxdisplay, zxWindowCanvas(win), zxfontset, zxWindowGC(win), (x), (y), (s), (n) )
#define zxDrawStringMB(win,x,y,s) zxDrawNStringMB( win, x, y, s, strlen(s) )

#define zxDrawNStringWC(win,x,y,s,n) \
  XwcDrawString( zxdisplay, zxWindowCanvas(win), zxfontset, zxWindowGC(win), (x), (y), (s), (n) )
#define zxDrawStringMWC(win,x,y,s) zxDrawNStringWC( win, x, y, s, strlen(s) )

/* Xft and fontconfig support */
#ifdef __ZX11_USE_XFT

#include <X11/Xft/Xft.h>

typedef struct{
  XftFont *font;
  XftDraw *draw;
} zxFTData;

extern const int zxft_weight[];
typedef enum{
  ZXFT_MEDIUM = 0,
  ZXFT_BOLD,
  ZXFT_LIGHT,
} zxFTWeight;

extern const int zxft_slant[];
typedef enum{
  ZXFT_ROMAN = 0,
  ZXFT_ITALIC,
  ZXFT_OBLIQUE,
} zxFTSlant;

bool zxFTCreate(zxFTData *data, zxWindow *win, const char *fontname, double size, zxFTWeight weight, zxFTSlant slant);
#define zxFTCreateSimple(data,win,fontname,size) \
  zxFTCreate( data, win, fontname, size, ZXFT_MEDIUM, ZXFT_ROMAN )

void zxFTDestroy(zxFTData *data);

bool zxFTDrawString(zxWindow *win, zxFTData *data, int x, int y, const char *string, ubyte red, ubyte green, ubyte blue, ubyte alpha);

#endif /* __ZX11_USE_XFT */

__END_DECLS

#endif /* __ZX_FONT_H__ */

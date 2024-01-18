/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxfont - font management for X11
 */

#include <zx11/zxfont.h>

/* font &text drawing method */

XFontStruct *zxfontstruct = NULL;

#define ZX_ERR_FONT_NOTFOUND "font %s not found"

#define ZX_DEFAULT_FONT "-*-*-*-*-*-*-*-*-*-*-*-*-*-*"

bool zxFontSet(const char *fontname)
{
  if( fontname ){
    if( ( zxfontstruct = XLoadQueryFont( zxdisplay, fontname ) ) )
      return true;
    ZRUNWARN( ZX_ERR_FONT_NOTFOUND, fontname );
  }
  if( ( zxfontstruct = XLoadQueryFont( zxdisplay, ZX_DEFAULT_FONT ) ) )
    return true;
  ZRUNERROR( ZX_ERR_FONT_NOTFOUND, ZX_DEFAULT_FONT );
  return false;
}

void zxFontUnset(void)
{
  if( zxfontstruct ){
    XFreeFont( zxdisplay, zxfontstruct );
    zxfontstruct = NULL;
  }
}

bool zxWindowSetFont(zxWindow *win, const char *fontname)
{
  if( !zxFontSet( fontname ) ) return false;
  XSetFont( zxdisplay, zxWindowGC(win), zxfontstruct->fid );
  return true;
}

#define ZX_WARN_UNDEF_CHAR "metric undefined for the specified charactor"

XCharStruct *zxCharStruct(char c)
{
  if( !zxfontstruct->per_char ) /* fixed charactor metric */
    return &zxfontstruct->min_bounds;
  /* variable charactor metric */
  if( (uint)c < zxfontstruct->min_char_or_byte2 ||
      (uint)c > zxfontstruct->max_char_or_byte2 ){
    ZRUNWARN( ZX_WARN_UNDEF_CHAR );
    return NULL;
  }
  return &zxfontstruct->per_char[c-zxfontstruct->min_char_or_byte2];
}

int zxCharWidth(char c)
{
  XCharStruct *cs;

  if( !( cs = zxCharStruct( c ) ) ) return 0;
  return cs->width;
}

int zxCharHeight(char c)
{
  XCharStruct *cs;

  if( !( cs = zxCharStruct( c ) ) ) return 0;
  return cs->ascent + cs->descent;
}

int zxTextNExtents(const char *str, int n, XCharStruct *cs)
{
  int dir, asc, des; /* dummy */

  if( !zxfontstruct ){
    ZRUNWARN( "no font set detected" );
    return -1;
  }
  XTextExtents( zxfontstruct, str, n, &dir, &asc, &des, cs );
  return 0;
}

short zxTextNHeight(const char *str, int n, short *ascent, short *descent)
{
  XCharStruct cs;
  int err;

  if( ( err = zxTextNExtents( str, n, &cs ) ) < 0 ) return err;
  *ascent = cs.ascent;
  *descent = cs.descent;
  return *ascent + *descent;
}

int zxTextNArea(const char *str, int n, short x, short y, zxRegion *reg)
{
  XCharStruct cs;
  int err;

  if( ( err = zxTextNExtents( str, n, &cs ) ) < 0 ) return err;
  zxRegionSet( reg,
    (uword)( x-cs.lbearing ), (uword)( y-cs.ascent ),
    cs.lbearing+cs.rbearing, cs.ascent+cs.descent );
  return 0;
}

/* text drawing under i18n */

XFontSet zxfontset;
XFontSetExtents *zxfontset_extents;

int zxSetLocale(void)
{
  if( !setlocale( LC_ALL, "" ) ){
    ZRUNERROR( "cannot set locale" );
    return -1;
  }
  if( XSupportsLocale() == false ){
    ZRUNERROR( "X doesn't support locale" );
    return -1;
  }
  if( XSetLocaleModifiers( "" ) == NULL ){
    ZRUNERROR( "cannot set locale modifiers" );
    return -1;
  }
  return 0;
}

XFontSet zxFontSetCreate(const char *fontname)
{
  char **miss, *def;
  int n_miss;

  if( !( zxfontset = XCreateFontSet( zxdisplay, fontname, &miss, &n_miss, &def ) ) ){
    ZRUNERROR( "cannot find font set for %s", fontname );
    return NULL;
  }
  XFreeStringList( miss );   
  zxfontset_extents = XExtentsOfFontSet( zxfontset );
  return zxfontset;
}

void zxTextAreaMB(const char *str, zxRegion *area)
{
  zxRegion ink;

  XmbTextExtents( zxfontset, str, strlen(str), &ink, area );
  area->x = -area->x;
  area->y = -area->y;
}

void zxTextAreaWC(const wchar_t *str, zxRegion *area)
{
  zxRegion ink;

  XwcTextExtents( zxfontset, str, wcslen(str), &ink, area );
  area->x = -area->x;
  area->y = -area->y;
}

/* Xft and fontconfig support */
#ifdef __ZX11_USE_XFT

const int zxft_weight[] = {
  XFT_WEIGHT_MEDIUM,
  XFT_WEIGHT_BOLD,
  XFT_WEIGHT_LIGHT,
};

const int zxft_slant[] = {
  XFT_SLANT_ROMAN,
  XFT_SLANT_ITALIC,
  XFT_SLANT_OBLIQUE,
};

bool zxFTCreate(zxFTData *data, zxWindow *win, const char *fontname, double size, zxFTWeight weight, zxFTSlant slant)
{
  FcChar8 *fontname_match;

  if( fontname ){
    fontname_match = (FcChar8 *)fontname;
  } else{ /* need to find default font */
    FcPattern *pat, *pat_default;
    FcResult result;

    FcInit();
    pat = FcPatternCreate();
    FcConfigSubstitute( 0, pat, FcMatchPattern );
    FcDefaultSubstitute( pat );
    pat_default = FcFontMatch( 0, pat, &result );
    if( result == FcResultNoMatch ){
      ZRUNERROR( "default font is unavailable" );
      return false;
    }
    FcPatternGetString( pat_default, "file", 0, &fontname_match );
    FcPatternDestroy( pat );
    FcPatternDestroy( pat_default );
    FcFini();
  }
  data->font = XftFontOpen( zxdisplay, 0,
    XFT_FAMILY, XftTypeString,  fontname_match,
    XFT_SIZE,   XftTypeDouble,  size,
    XFT_WEIGHT, XftTypeInteger, zxft_weight[weight],
    XFT_SLANT,  XftTypeInteger, zxft_slant[slant],
    NULL );
  if( !( data->draw = XftDrawCreate( zxdisplay, zxWindowBody(win), zxvisual, zxcmap ) ) ){
    ZRUNERROR( "cannot create drawable for a string" );
    return false;
  }
  return true;
}

void zxFTDestroy(zxFTData *data)
{
  XftDrawDestroy( data->draw );
  XftFontClose( zxdisplay, data->font );
}

static XRenderColor *_zxSetRenderColor(XRenderColor *rc, ubyte red, ubyte green, ubyte blue, ubyte alpha)
{
  rc->red   = ( red   << 8 ) | red;
  rc->green = ( green << 8 ) | green;
  rc->blue  = ( blue  << 8 ) | blue;
  rc->alpha = ( alpha << 8 ) | alpha;
  return rc;
}

bool zxFTDrawString(zxWindow *win, zxFTData *data, int x, int y, const char *string, ubyte red, ubyte green, ubyte blue, ubyte alpha)
{
  XRenderColor rc;
  XftColor color;

  XftColorAllocValue( zxdisplay, zxvisual, zxcmap, _zxSetRenderColor( &rc, red, green, blue, alpha ), &color );
  XftDrawStringUtf8( data->draw, &color, data->font, x, y, (FcChar8*)string, strlen(string) );
  return true;
}

#endif /* __ZX11_USE_XFT */

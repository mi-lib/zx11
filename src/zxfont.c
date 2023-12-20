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

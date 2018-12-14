/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxinput - keyboard/mouse operations on X11
 */

#include <zx11/zxinput.h>

/* key event operation */

/* modifiers */
byte __zx_modkey = ZX_MODKEY_NONE;

/* METHOD: zxKeySymbol
 * return key symbol accepted by event.
 */
KeySym zxKeySymbol(void)
{
  return XkbKeycodeToKeysym( zxdisplay, zxevent.xkey.keycode, 0, 0 );
}

/* zxKeySymbolAndString
 * - get key symbol and the input string.
 */
int zxKeySymbolAndString(char *str, size_t size, KeySym *key)
{
  return XLookupString( &zxevent.xkey, str, size, key, NULL );
}

byte zxModkey(void){ return __zx_modkey; }

bool zxModkeyOn(KeySym key)
{
  switch( key ){
  case XK_Control_L: case XK_Control_R:
    zxModkeyCtrlOn(); return true;
  case XK_Shift_L: case XK_Shift_R:
    zxModkeyShiftOn(); return true;
  case XK_Alt_L: case XK_Alt_R:
    zxModkeyAltOn(); return true;
  }
  return false;
}

bool zxModkeyOff(KeySym key)
{
  switch( key ){
  case XK_Control_L: case XK_Control_R:
    zxModkeyCtrlOff(); return true;
  case XK_Shift_L: case XK_Shift_R:
    zxModkeyShiftOff(); return true;
  case XK_Alt_L: case XK_Alt_R:
    zxModkeyAltOff(); return true;
  }
  return false;
}

/* mouse event operation */

bool zxMouseInRegion(zxRegion *reg)
{
  return zxRegionIsIn( reg, zxMouseX, zxMouseY );
}

/* input method & input context */

static XIM _zxim; /* input method */
static XIC _zxic; /* input context */

int zxIMInit(zxWindow *win)
{
  unsigned long fevent; /* events filtered to input method */

  if( !( _zxim = XOpenIM( zxdisplay, NULL, NULL, NULL ) ) ){
    ZRUNERROR( "cannot open input method" );
    return -1;
  }
  if( !( _zxic = XCreateIC( _zxim,
      XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
      XNClientWindow, zxWindowBody(win), NULL ) ) ){
    ZRUNERROR( "cannot create input context" );
    XCloseIM( _zxim );
    return -1;
  }
  XGetICValues( _zxic, XNFilterEvents, &fevent, NULL );
  zxAddEventMask( win, fevent );
  return 0;
}

int zxIMInitOffspot(zxWindow *win, zxRegion *preg, zxRegion *sreg)
{
  XVaNestedList pa, sa;

  if( !( _zxim = XOpenIM( zxdisplay, NULL, NULL, NULL ) ) ){
    ZRUNERROR( "cannot open input method" );
    return -1;
  }

  pa = XVaCreateNestedList( 0, XNArea, preg, XNFontSet, zxfontset, NULL );
  sa = XVaCreateNestedList( 0, XNArea, sreg, XNFontSet, zxfontset, NULL );
  _zxic = XCreateIC( _zxim,
    XNInputStyle, XIMPreeditArea | XIMStatusArea,
    XNClientWindow, zxWindowBody(win),
    XNPreeditAttributes, pa, XNStatusAttributes, sa, NULL );
  XFree( pa );
  XFree( sa );

  if( !_zxic ){
    ZRUNERROR( "cannot create input context" );
    XCloseIM( _zxim );
    return -1;
  }
  return 0;
}

void zxIMClose(void)
{
  XDestroyIC( _zxic );
  XCloseIM( _zxim );
}

void zxICFocus(void)
{
  XSetICFocus( _zxic );
}

void zxICUnfocus(void)
{
  XUnsetICFocus( _zxic );
}

int zxICGetString(wchar_t *buf, size_t size, KeySym *ks, Status *st)
{
  return XwcLookupString( _zxic, (XKeyPressedEvent*)&zxevent, buf, size, ks, st );
}

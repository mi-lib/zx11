/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxinput - keyboard/mouse operations on X11
 */

#include <zx11/zxinput.h>

/* key event operations */

/* key modifiers */
byte zxmodkey = ZX_MODKEY_NONE;

/* return key symbol accepted by event. */
KeySym zxKeySymbol(void)
{
  return XkbKeycodeToKeysym( zxdisplay, zxevent.xkey.keycode, 0, 0 );
}

/* get key symbol and the input string. */
int zxKeySymbolAndString(char *str, size_t size, KeySym *key)
{
  return XLookupString( &zxevent.xkey, str, size, key, NULL );
}

/* return the key modifier */
byte zxModkey(void){ return zxmodkey; }

/* detect the key modifier */
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

/* unset the key modifier */
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

/* mouse event operations */

/* check if the mouse pointer is inside of a specified region */
bool zxMouseInRegion(zxRegion *reg)
{
  return zxRegionIsIn( reg, zxMouseX, zxMouseY );
}

/* input method & input context */

static XIM _zxim; /* input method */

/* initialize input method */
int zxIMInit(void)
{
  if( !( _zxim = XOpenIM( zxdisplay, NULL, NULL, NULL ) ) ){
    ZRUNERROR( "cannot open input method" );
    return -1;
  }
  return 0;
}

/* enable input method of a window */
int zxWindowIMEnable(zxWindow *win)
{
  unsigned long fevent; /* events filtered to input method */

  if( !( win->ic = XCreateIC( _zxim,
      XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
      XNClientWindow, zxWindowBody(win), NULL ) ) ){
    ZRUNERROR( "cannot create input context" );
    XCloseIM( _zxim );
    return -1;
  }
  XGetICValues( win->ic, XNFilterEvents, &fevent, NULL );
  zxWindowAddEvent( win, fevent );
  return 0;
}

/* enable off-spot input method of a window */
int zxWindowIMOffspotEnable(zxWindow *win, zxRegion *preg, zxRegion *sreg)
{
  XVaNestedList pa, sa;

  pa = XVaCreateNestedList( 0, XNArea, preg, XNFontSet, zxfontset, NULL );
  sa = XVaCreateNestedList( 0, XNArea, sreg, XNFontSet, zxfontset, NULL );
  win->ic = XCreateIC( _zxim,
    XNInputStyle, XIMPreeditArea | XIMStatusArea,
    XNClientWindow, zxWindowBody(win),
    XNPreeditAttributes, pa, XNStatusAttributes, sa, NULL );
  XFree( pa );
  XFree( sa );

  if( !win->ic ){
    ZRUNERROR( "cannot create input context" );
    XCloseIM( _zxim );
    return -1;
  }
  return 0;
}

/* disable input method of a window */
void zxWindowIMDisable(zxWindow *win)
{
  XDestroyIC( win->ic );
  win->ic = NULL;
}

/* exit input method */
void zxIMExit(void)
{
  if( _zxim ){
    XCloseIM( _zxim );
    _zxim = NULL;
  }
}

/* set input context focus */
void zxWindowSetICFocus(zxWindow *win)
{
  XSetICFocus( win->ic );
}

/* unset input context focus */
void zxWindowUnsetICFocus(zxWindow *win)
{
  XUnsetICFocus( win->ic );
}

/* get a string from input context */
int zxWindowICGetString(zxWindow *win, wchar_t *buf, size_t size, KeySym *ks, Status *st)
{
  return XwcLookupString( win->ic, (XKeyPressedEvent*)&zxevent, buf, size, ks, st );
}

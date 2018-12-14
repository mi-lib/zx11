/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxinput - keyboard/mouse operations on X11
 */

#ifndef __ZX_INPUT_H__
#define __ZX_INPUT_H__

#include <zx11/zxfont.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>

__BEGIN_DECLS

/* key event operation */

#define zxKeyEnable(win) \
  zxAddEventMask( win, KeyPressMask | KeyReleaseMask )

KeySym zxKeySymbol(void);
int zxKeySymbolAndString(char *str, size_t size, KeySym *key);

/* modifiers */

#define ZX_MODKEY_NONE  0x0000
#define ZX_MODKEY_CTRL  0x0001
#define ZX_MODKEY_SHIFT 0x0002
#define ZX_MODKEY_ALT   0x0004

extern byte __zx_modkey;

#define zxModkeyCtrlOn()    ( __zx_modkey |= ZX_MODKEY_CTRL )
#define zxModkeyShiftOn()   ( __zx_modkey |= ZX_MODKEY_SHIFT )
#define zxModkeyAltOn()     ( __zx_modkey |= ZX_MODKEY_ALT )

#define zxModkeyCtrlOff()   ( __zx_modkey &= ~ZX_MODKEY_CTRL )
#define zxModkeyShiftOff()  ( __zx_modkey &= ~ZX_MODKEY_SHIFT )
#define zxModkeyAltOff()    ( __zx_modkey &= ~ZX_MODKEY_ALT )

#define zxModkeyCtrlIsOn()  ( __zx_modkey & ZX_MODKEY_CTRL )
#define zxModkeyShiftIsOn() ( __zx_modkey & ZX_MODKEY_SHIFT )
#define zxModkeyAltIsOn()   ( __zx_modkey & ZX_MODKEY_ALT )

byte zxModkey(void);
bool zxModkeyOn(KeySym key);
bool zxModkeyOff(KeySym key);

/* mouse event operation */

#define zxMouseEnable(win) \
  zxAddEventMask( win, ButtonPressMask | ButtonReleaseMask | PointerMotionMask )
#define zxMouseX      zxevent.xbutton.x
#define zxMouseY      zxevent.xbutton.y
#define zxMouseButton zxevent.xbutton.button

bool zxMouseInRegion(zxRegion *reg);

/* input method & input context */

int zxIMInit(zxWindow *win);
int zxIMInitOffspot(zxWindow *win, zxRegion *preg, zxRegion *sreg);
void zxIMClose(void);

void zxICFocus(void);
void zxICUnfocus(void);

int zxICGetString(wchar_t *buf, size_t size, KeySym *ks, Status *st);

__END_DECLS

#endif /* __ZX_INPUT_H__ */

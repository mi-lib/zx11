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

/* key event operations */

/* enable key actions of a window */
#define zxWindowKeyEnable(win) \
  zxWindowAddEvent( win, KeyPressMask | KeyReleaseMask )

/* return key symbol accepted by event. */
KeySym zxKeySymbol(void);

/* key code */
#define zxKeyCode() zxevent.xkey.keycode

/* get key symbol and the input string. */
int zxKeySymbolAndString(char *str, size_t size, KeySym *key);

/* key modifiers */
extern byte zxmodkey;

#define ZX_MODKEY_NONE  0x0000
#define ZX_MODKEY_CTRL  0x0001
#define ZX_MODKEY_SHIFT 0x0002
#define ZX_MODKEY_ALT   0x0004

#define zxModkeyCtrlOn()    ( zxmodkey |= ZX_MODKEY_CTRL )
#define zxModkeyShiftOn()   ( zxmodkey |= ZX_MODKEY_SHIFT )
#define zxModkeyAltOn()     ( zxmodkey |= ZX_MODKEY_ALT )

#define zxModkeyCtrlOff()   ( zxmodkey &= ~ZX_MODKEY_CTRL )
#define zxModkeyShiftOff()  ( zxmodkey &= ~ZX_MODKEY_SHIFT )
#define zxModkeyAltOff()    ( zxmodkey &= ~ZX_MODKEY_ALT )

#define zxModkeyCtrlIsOn()  ( zxmodkey & ZX_MODKEY_CTRL )
#define zxModkeyShiftIsOn() ( zxmodkey & ZX_MODKEY_SHIFT )
#define zxModkeyAltIsOn()   ( zxmodkey & ZX_MODKEY_ALT )

/* return the key modifier */
byte zxModkey(void);
/* detect the key modifier */
bool zxModkeyOn(KeySym key);
/* unset the key modifier */
bool zxModkeyOff(KeySym key);

/* mouse event operations */

/* enable mouse actions of a window */
#define zxWindowMouseEnable(win) \
  zxWindowAddEvent( win, ButtonPressMask | ButtonReleaseMask | PointerMotionMask )

#define zxMouseX      zxevent.xbutton.x
#define zxMouseY      zxevent.xbutton.y
#define zxMouseButton zxevent.xbutton.button

/* check if the mouse pointer is inside of a specified region */
bool zxMouseInRegion(zxRegion *reg);

/* input method & input context */

/* initialize input method */
int zxIMInit(void);
/* enable input method of a window */
int zxWindowIMEnable(zxWindow *win);
/* enable off-spot input method of a window */
int zxWindowIMOffspotEnable(zxWindow *win, zxRegion *preg, zxRegion *sreg);
/* disable input method of a window */
void zxWindowIMDisable(zxWindow *win);
/* exit input method */
void zxIMExit(void);

/* set input context focus */
void zxWindowSetICFocus(zxWindow *win);
/* unset input context focus */
void zxWindowUnsetICFocus(zxWindow *win);

/* get a string from input context */
int zxWindowICGetString(zxWindow *win, wchar_t *buf, size_t size, KeySym *ks, Status *st);

__END_DECLS

#endif /* __ZX_INPUT_H__ */

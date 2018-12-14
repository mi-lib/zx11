/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxutil - utilities for X11
 */

/* virtual root window operation */
#ifdef __USE_VROOT__
#include <vroot.h>
#endif /* __USE_VROOT__ */

#include <ctype.h>
#include <zx11/zxutil.h>

/* OBJECT: zxdisplay
 * display handler
 */
Display *zxdisplay;

/* OBJECT: zxrootwindow
 * default root window
 */
Window zxrootwindow;

/* OBJECT: zxscreen
 * default screen
 */
Screen *zxscreen;

/* OBJECT: zxcmap
 * default colormap
 */
Colormap zxcmap;

/* OBJECT: zxvisual
 * default visual type
 */
Visual *zxvisual;

/* OBJECT: zxdepth
 * default color depth
 */
int zxdepth;

/* OBJECT: zxevent
 * event capturer
 */
XEvent zxevent;

/* ********************************************************** */
/* CLASS: zxRegion
 * redefinition of XRectangle
 * ********************************************************** */

bool zxRegionIsIn(zxRegion *reg, short x, short y)
{
  return ( x >= reg->x && x < reg->x + reg->width &&
           y >= reg->y && y < reg->y + reg->height ) ? true : false;
}

void zxRegionToStr(zxRegion *reg, char *str)
{
  sprintf( str, "(%dx%d)+%d+%d", reg->width, reg->height, reg->x, reg->y );
}

/* for debug */
void zxRegionWrite(zxRegion *reg)
{
  char buf[BUFSIZ];

  zxRegionToStr( reg, buf );
  puts( buf );
}

/* ********************************************************** */
/* CLASS: zxWindow
 * window and its contents
 * ********************************************************** */

static Atom zx_wm_protocols;
static Atom zx_wm_delete_window;

/* basic methods */

/* METHOD: void zxInit(void)
 * initialize X11 system - get information of display & rootwindow
 */
void zxInit(void)
{
  zxdisplay = XOpenDisplay( NULL );
  if( zxdisplay == NULL ){
    ZRUNERROR( "cannot connect to X server" );
    exit( 1 );
  }
  zxrootwindow = DefaultRootWindow( zxdisplay );
  zxscreen     = DefaultScreenOfDisplay( zxdisplay );
  zxcmap       = DefaultColormap( zxdisplay, Z_DEFAULT_SCREEN );
  zxvisual     = DefaultVisual( zxdisplay, Z_DEFAULT_SCREEN );
  zxdepth      = DefaultDepth( zxdisplay, Z_DEFAULT_SCREEN );

  zx_wm_protocols = XInternAtom( zxdisplay, "WM_PROTOCOLS", False );
  zx_wm_delete_window = XInternAtom( zxdisplay, "WM_DELETE_WINDOW", False );
}

void zxDefaultSetWindowAttributes(XSetWindowAttributes *attr)
{
  attr->background_pixmap = None;
  attr->background_pixel = BlackPixel( zxdisplay, 0 );
  attr->border_pixmap = None;
  attr->border_pixel = 0;
  attr->bit_gravity = 0;
  attr->win_gravity = 0;
  attr->backing_store = Always;
  attr->backing_planes = 0;
  attr->backing_pixel = 0;
  attr->save_under = False;
  attr->event_mask = ExposureMask | StructureNotifyMask | SubstructureNotifyMask | FocusChangeMask;
  attr->do_not_propagate_mask = 0;
  attr->override_redirect = False;
  attr->colormap = zxcmap;
  attr->cursor = None;
}

static void _zxWindowCreate(zxWindow *win, Window parent, int x, int y, int w, int h, int border);
static void _zxWindowCreateDefaultGC(zxWindow *win);

void _zxWindowCreateDefaultGC(zxWindow *win)
{
  zxDefaultGC(win) = XCreateGC( zxdisplay, zxWindowBody(win), 0, &win->gv );
  zxSetDefaultGC( win );
  zxSetCanvas( win, zxWindowBody(win) );
  zxSetBGColor( win, BlackPixel( zxdisplay, 0 ) );
  zxSetFGColor( win, WhitePixel( zxdisplay, 0 ) );
}

void _zxWindowCreate(zxWindow *win, Window parent, int x, int y, int w, int h, int border)
{
  zxWindowSetParent( win, parent );
  zxDefaultSetWindowAttributes( &win->attr );
  zxWindowSetMask( win, CWBackPixel | CWBorderPixel | CWColormap | CWEventMask |CWBackingStore | CWOverrideRedirect );
  zxWindowSetBody( win,
    XCreateWindow( zxdisplay, zxWindowParent(win), x, y, w, h, border,
      zxdepth, InputOutput, zxvisual, zxWindowMask(win), &win->attr ) );
  zxWindowSetGeometry( win, x, y, w, h );
  _zxWindowCreateDefaultGC( win );
  XSetWMProtocols( zxdisplay, zxWindowBody(win), &zx_wm_delete_window, 1 );
  zxWindowClear( win );
}

/* METHOD: void zxWindowCreate(zxWindow *win, int x, int y, int w, int h)
 * create simple X-window.
 */
void zxWindowCreate(zxWindow *win, int x, int y, int w, int h)
{
  _zxWindowCreate( win, zxrootwindow, x, y, w, h, 2 );
}

void zxWindowCreateChild(zxWindow *win, zxWindow *parent, int x, int y, int w, int h)
{
  _zxWindowCreate( win, zxWindowBody(parent), x, y, w, h, 0 );
}

/* METHOD: zxWindowCreateRoot
 * create X-window as a root window.
 */
void zxWindowCreateRoot(zxWindow *win)
{
  zxWindowSetParent( win, zxrootwindow );
  zxWindowSetBody( win, zxrootwindow );
  zxWindowSetGeometry( win, 0, 0, zxScreenWidth(), zxScreenHeight() );
  _zxWindowCreateDefaultGC( win );
  zxSetEventMask( win, ExposureMask );
  XMapWindow( zxdisplay, zxWindowBody(win) );
}

void zxWindowDestroy(zxWindow *win)
{
  zxWindowClose( win );
  XFreeGC( zxdisplay, zxDefaultGC(win) );
  XDestroyWindow( zxdisplay, zxWindowBody(win) );
}

/* METHOD: void zxWindowClear(zxWindow *win)
 * clear window with background color.
 */
void zxWindowClear(zxWindow *win)
{
  XSetWindowBackground( zxdisplay, zxWindowBody(win), zxBGColor(win) );
  XClearWindow( zxdisplay, zxWindowBody(win) );
  zxFlush();
}

void zxWindowClearArea(zxWindow *win, int x, int y, int w, int h)
{
  XClearArea( zxdisplay, zxWindowBody(win), x, y, w, h, False );
}

void zxWindowFill(zxWindow *win, Pixmap pattern)
{
  zxWindowClose( win );
  XSetWindowBackgroundPixmap( zxdisplay, zxWindowBody(win), pattern );
  zxWindowOpen( win );
}

bool zxWindowUpdateRegion(zxWindow *win)
{
  return zxGetGeometry( zxWindowBody(win), zxWindowRegion(win) );
}

/* METHOD: zxBackingStoreOn
 * void zxBackingStoreOn(zxWindow *win)
 */
void zxBackingStoreOn(zxWindow *win)
{
  win->attr.backing_store = Always;
  XChangeWindowAttributes( zxdisplay,
    zxWindowBody(win), CWBackingStore, &win->attr );
}

/* METHOD: zxBackingStoreOff
 * void zxBackingStoreOff(zxWindow *win)
 */
void zxBackingStoreOff(zxWindow *win)
{
  win->attr.backing_store = NotUseful;
  XChangeWindowAttributes( zxdisplay,
    zxWindowBody(win), CWBackingStore, &win->attr );
}

/* METHOD: void zxOverrideRedirect(zxWindow *win)
 * override redirection to avoid the interrupt from window manager.
 */
void zxOverrideRedirect(zxWindow *win)
{
  win->attr.override_redirect = True;
  XChangeWindowAttributes( zxdisplay,
    zxWindowBody(win), CWOverrideRedirect, &win->attr );
}

/* zxDeleteWindowEvent
 * - receive the delete-window event.
 */
bool zxDeleteWindowEvent(void)
{
  return zxevent.xclient.message_type == zx_wm_protocols &&
    (Atom)zxevent.xclient.data.l[0] == zx_wm_delete_window ? true : false;
}

/* pixmaps */

Pixmap zxPixmapClone(zxWindow *win, Pixmap src, int x, int y, int width, int height)
{
  Pixmap dest;

  if( ( dest = zxPixmapCreate( win, width, height ) ) )
    zxPixmapCopy( win, src, dest, x, y, width, height, 0, 0 );
  return dest;
}

bool zxPixmapGetSize(Drawable drw, int *width, int *height)
{
  uint x, y, border, depth;
  Window rootwin;

  return XGetGeometry( zxdisplay, drw, &rootwin,
    (int *)&x, (int *)&y, (uint *)width, (uint *)height, &border, &depth ) ? true : false;
}

/* double bufferring */

void zxDoubleBufferEnable(zxWindow *win)
{
  win->db = zxPixmapCreate( win, zxWindowWidth(win), zxWindowHeight(win) );
  zxSetCanvas( win, win->db );
}

void zxDoubleBufferDisable(zxWindow *win)
{
  zxSetCanvas( win, zxWindowBody(win) );
  zxPixmapDestroy( win->db );
}

/* color operation methods */

void zxColormapCreate(zxWindow *win)
{
  win->attr.colormap = XCreateColormap( zxdisplay,
    zxWindowParent(win), zxvisual, AllocNone);
  XChangeWindowAttributes( zxdisplay,
    zxWindowBody(win), CWColormap, &win->attr );
}

/* METHOD: ulong zxGetColor(char color[])
 * get color with a name 'color'.
 */
ulong zxGetColor(zxWindow *win, char color[])
{
  XColor c0, c1;

  XAllocNamedColor( zxdisplay, win->attr.colormap, color, &c1, &c0 );
  return c1.pixel;
}

/* METHOD: ulong zxGetRGBColor(uword red, uword green, uword blue)
 * get color with RGB parameter.
 */
ulong zxGetRGBColor(zxWindow *win, uword red, uword green, uword blue)
{
  XColor c;

  c.red = red;
  c.green = green;
  c.blue = blue;
  c.flags = DoRed | DoGreen | DoBlue;
  XAllocColor( zxdisplay, win->attr.colormap, &c );
  return c.pixel;
}

/* event operation methods */

/* zxNextEvent
 */
int zxNextEvent(void)
{
  XNextEvent( zxdisplay, &zxevent );
  return zxevent.type;
}

/* METHOD: zxGetEvent
 */
int zxGetEvent(void)
{
  return XPending( zxdisplay ) > 0 ? zxNextEvent() : None;
}

/* METHOD: zxDequeueEvent
 */
int zxDequeueEvent(void)
{
  return XEventsQueued( zxdisplay, QueuedAfterReading ) > 0 ?
    zxNextEvent() : None;
}

/* METHOD: zxCheckEvent
 */
bool zxCheckEvent(int type)
{
  return zxGetEvent() == type ? true : false;
}

/* stacked window manipulation */

/* zxGetInputFocus
 */
Window zxGetInputFocus(void)
{
  Window focused;
  int revert;

  XGetInputFocus( zxdisplay, &focused, &revert );
  return focused;
}

/* zxGetGeometry
 */
bool zxGetGeometry(Drawable drw, zxRegion *reg)
{
  uint x, y, w, h, border, depth;
  Window rootwin, child;

  if( XGetGeometry( zxdisplay, drw, &rootwin,
        (int *)&x, (int *)&y, &w, &h, &border, &depth ) ){
    XTranslateCoordinates( zxdisplay, drw, rootwin,
      0, 0, (int *)&x, (int *)&y, &child );
    zxRegionSet( reg, x, y, w, h );
    return true;
  }
  return false;
}

/* drawing methods */

/* METHOD: void zxSetLineWidth(zxWindow *win, int width)
 * set line-width.
 */
void zxSetLineWidth(zxWindow *win, int width)
{
  win->gv.line_width = width;
  XChangeGC( zxdisplay, zxGC(win), GCLineWidth, &win->gv );
}

/* METHOD: void zxSetLineStyle(zxWindow *win, int style)
 * set line-style.
 */
void zxSetLineStyle(zxWindow *win, int style)
{
  win->gv.line_style = style;
  XChangeGC( zxdisplay, zxGC(win), GCLineStyle, &win->gv );
}

/* METHOD: void zxSetLineCap(zxWindow *win, int cap)
 * set line-cap.
 */
void zxSetLineCap(zxWindow *win, int cap)
{
  win->gv.cap_style = cap;
  XChangeGC( zxdisplay, zxGC(win), GCCapStyle, &win->gv );
}

/* METHOD: void zxSetLineJoin(zxWindow *win, int join)
 * set line-join.
 */
void zxSetLineJoin(zxWindow *win, int join)
{
  win->gv.join_style = join;
  XChangeGC( zxdisplay, zxGC(win), GCJoinStyle, &win->gv );
}

void zxPixmapPolygon(zxWindow *win, Pixmap canvas, XPoint *points, int n, Pixmap pattern)
{
  XSetFillStyle( zxdisplay, zxGC(win), FillTiled );
  XSetTile( zxdisplay, zxGC(win), pattern );
  zxSetFillRule( win, EvenOddRule );
  zxFillPolygonComplex( win, canvas, points, n );
}

void zxClear(zxWindow *win)
{
  zxSetColorMap( win, zxBGColor(win) );
  zxDrawFillRect( win, 0, 0, zxWindowWidth(win), zxWindowHeight(win) );
  zxSetColorMap( win, zxFGColor(win) );
}

/* METHOD: void zxPutBuffer(zxWindow *win, int x, int y, int w, int h, ulong *buf)
 * put pixel buffer 'buf' on the X contents 'win'.
 * 'x' and 'y' is the point of origin on the window.
 * 'w' is a width of 'buf'.
 * 'h' is a height of 'buf'.
 */
void zxPutBuffer(zxWindow *win, int x, int y, int w, int h, ulong *buf)
{
  register int i, j, k;

  for( i=0; i<h; i++ ){
    k = i * w;
    for( j=0; j<w; j++ ){
      zxSetColorMap( win, buf[k+j] );
      zxDrawPoint( win, x+j, y+i );
    }
  }
}

/* command line option utility */

int zxParseGeometry(char *str, zxRegion *reg)
{
  char op[] = { 'x', '+', '+' };
  int ret;

  if( isdigit( str[0] ) ){
    ret = sscanf( str, "%hd%c%hd%c%hd%c%hd",
      &reg->width, &op[0], &reg->height,
      &op[1], &reg->x, &op[2], &reg->y );
    switch( ret ){
    case 1:
      reg->height = 0;
    case 3:
      if( op[0] != 'x' ) goto ERROR;
      reg->x = ZX_GEOMETRY_NONE;
    case 5:
      reg->y = ZX_GEOMETRY_NONE; break;
    case 2: case 4: case 6:
      goto ERROR;
    default: ;
    }
  } else{
    reg->width = reg->height = 0;
    ret = sscanf( str, "%c%hd%c%hd", &op[1], &reg->x, &op[2], &reg->y );
    switch( ret ){
    case 2: reg->y = ZX_GEOMETRY_NONE; break;
    case 1: case 3:
      goto ERROR;
    default: ;
    }
  }
  if( op[1] == '-' ) reg->x = zxScreenWidth() - reg->x;
  else if( op[1] != '+' ) goto ERROR;
  if( op[2] == '-' ) reg->y = zxScreenHeight() - reg->y;
  else if( op[2] != '+' ) goto ERROR;
  return 1;

 ERROR:
  ZRUNERROR( "invalid geometry expression" );
  reg->x = reg->y = ZX_GEOMETRY_NONE;
  reg->width = reg->height = 0;
  return 0;
}

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

/* display handler */
Display *zxdisplay = NULL;

/* default root window */
Window zxrootwindow;

/* default colormap */
Colormap zxcmap;

/* default visual type */
Visual *zxvisual;

/* default color depth */
int zxdepth;

/* event capturer */
XEvent zxevent;

/* ********************************************************** */
/* alias of XRectangle
 * ********************************************************** */

/* check if a point is in a region. */
bool zxRegionIsIn(zxRegion *reg, short x, short y)
{
  return ( x >= reg->x && x < reg->x + reg->width &&
           y >= reg->y && y < reg->y + reg->height ) ? true : false;
}

/* make a string that represents a specified region. */
void zxRegionToStr(zxRegion *reg, char *str)
{
  sprintf( str, "(%dx%d)+%d+%d", reg->width, reg->height, reg->x, reg->y );
}

/* for debug */
void zxRegionPrint(zxRegion *reg)
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

/* initialize X11 system and get information of display and the root window */
zxInitStatus zxInit(void)
{
  if( zxdisplay ){
    ZRUNWARN( "already connected to X server" );
    return ZXINIT_DUP;
  }
  if( !( zxdisplay = XOpenDisplay( NULL ) ) ){
    ZRUNERROR( "cannot connect to X server" );
    return ZXINIT_FAILURE;
  }
  zxrootwindow = DefaultRootWindow( zxdisplay );
  zxcmap       = DefaultColormap( zxdisplay, 0 );
  zxvisual     = DefaultVisual( zxdisplay, 0 );
  zxdepth      = DefaultDepth( zxdisplay, 0 );

  zx_wm_protocols = XInternAtom( zxdisplay, "WM_PROTOCOLS", False );
  zx_wm_delete_window = XInternAtom( zxdisplay, "WM_DELETE_WINDOW", False );
  return ZXINIT_SUCCESS;
}

/* exit from connection with X11 system */
void zxExit(void)
{
  if( zxdisplay ){
    XCloseDisplay( zxdisplay );
    zxdisplay = NULL;
  }
}

/* set default window attributes */
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

/* create default graphics context */
static void _zxWindowCreateDefaultGC(zxWindow *win)
{
  zxWindowDefaultGC(win) = XCreateGC( zxdisplay, zxWindowBody(win), 0, &win->gv );
  zxWindowSetDefaultGC( win );
  zxWindowSetCanvas( win, zxWindowBody(win) );
  win->db = None;
  win->bgcolor = BlackPixel( zxdisplay, 0 );
  zxWindowSetColor( win, WhitePixel( zxdisplay, 0 ) );
}

/* create a window */
static void _zxWindowCreate(zxWindow *win, Window parent, int x, int y, int w, int h, int border)
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
  win->ic = NULL; /* input context operations defined in zxinput. */
}

/* create a primary window */
void zxWindowCreate(zxWindow *win, int x, int y, int w, int h)
{
  if( !zxdisplay && !zxInit() ) return;
  _zxWindowCreate( win, zxrootwindow, x, y, w, h, 2 );
}

/* create a child window of another */
void zxWindowCreateChild(zxWindow *win, zxWindow *parent, int x, int y, int w, int h)
{
  _zxWindowCreate( win, zxWindowBody(parent), x, y, w, h, 0 );
  win->attr.background_pixmap = ParentRelative;
  XChangeWindowAttributes( zxdisplay, zxWindowBody(win), CWBackingPixel, &win->attr );
  XSetWindowBackgroundPixmap( zxdisplay, zxWindowBody(win), CopyFromParent );
  XClearWindow( zxdisplay, zxWindowBody(win) );
  zxWindowSetBGColor( win, parent->bgcolor );
}

/* create a full-size window as a fake root */
void zxWindowCreateRoot(zxWindow *win)
{
  if( !zxdisplay && !zxInit() ) return;
  zxWindowSetParent( win, zxrootwindow );
  zxWindowSetBody( win, zxrootwindow );
  zxWindowSetGeometry( win, 0, 0, zxScreenWidth(), zxScreenHeight() );
  _zxWindowCreateDefaultGC( win );
  zxWindowAddEvent( win, ExposureMask );
  XMapWindow( zxdisplay, zxWindowBody(win) );
}

/* destroy a window and an associated graphics context */
void zxWindowDestroy(zxWindow *win)
{
  zxWindowClose( win );
  zxPixmapDestroy( win->db );
  XFreeGC( zxdisplay, zxWindowDefaultGC(win) );
  XDestroyWindow( zxdisplay, zxWindowBody(win) );
}

/* clear window with background color */
void zxWindowClear(zxWindow *win)
{
  ulong fgcolor;

  fgcolor = win->fgcolor;
  zxWindowSetColor( win, win->bgcolor );
  zxDrawFillRect( win, 0, 0, zxWindowWidth(win), zxWindowHeight(win) );
  zxWindowSetColor( win, fgcolor );
  zxFlush();
}

/* clear specified area of a window */
void zxWindowClearArea(zxWindow *win, int x, int y, int w, int h)
{
  XClearArea( zxdisplay, zxWindowBody(win), x, y, w, h, False );
}

/* fill a window with a specified pixmap pattern */
void zxWindowFill(zxWindow *win, Pixmap pattern)
{
  zxWindowClose( win );
  XSetWindowBackgroundPixmap( zxdisplay, zxWindowBody(win), pattern );
  zxWindowOpen( win );
}

/* update geometry information of a window */
bool zxWindowUpdateRegion(zxWindow *win)
{
  return zxGetGeometry( zxWindowBody(win), zxWindowRegion(win) );
}

/* enable backing-store */
void zxWindowBackingStoreEnable(zxWindow *win)
{
  win->attr.backing_store = Always;
  XChangeWindowAttributes( zxdisplay, zxWindowBody(win), CWBackingStore, &win->attr );
}

/* disable backing-store */
void zxWindowBackingStoreDisable(zxWindow *win)
{
  win->attr.backing_store = NotUseful;
  XChangeWindowAttributes( zxdisplay, zxWindowBody(win), CWBackingStore, &win->attr );
}

/* enable override redirection to avoid the interrupt from window manager */
void zxWindowOverrideRedirectEnable(zxWindow *win)
{
  win->attr.override_redirect = True;
  XChangeWindowAttributes( zxdisplay, zxWindowBody(win), CWOverrideRedirect, &win->attr );
}

/* receive the event to delete a window */
bool zxWindowIsReceivedDeleteMsg(zxWindow *win)
{
  return zxevent.xclient.message_type == zx_wm_protocols &&
    (Atom)zxevent.xclient.data.l[0] == zx_wm_delete_window &&
    zxevent.xclient.window == zxWindowBody(win) ? true : false;
}

/* pixmaps */

/* clone a pixmap */
Pixmap zxPixmapClone(zxWindow *win, Pixmap src, int x, int y, int width, int height)
{
  Pixmap dest;

  if( ( dest = zxPixmapCreate( win, width, height ) ) )
    zxPixmapCopy( win, src, dest, x, y, width, height, 0, 0 );
  return dest;
}

/* get size of a pixmap */
bool zxPixmapGetSize(Drawable drw, int *width, int *height)
{
  uint x, y, border, depth;
  Window rootwin;

  return XGetGeometry( zxdisplay, drw, &rootwin,
    (int *)&x, (int *)&y, (uint *)width, (uint *)height, &border, &depth ) ? true : false;
}

/* color operation methods */

/* create a color map */
void zxWindowColorMapCreate(zxWindow *win)
{
  win->attr.colormap = XCreateColormap( zxdisplay, zxWindowParent(win), zxvisual, AllocNone);
  XChangeWindowAttributes( zxdisplay, zxWindowBody(win), CWColormap, &win->attr );
}

/* get color by a name. */
ulong zxGetColor(zxWindow *win, const char *color)
{
  XColor c0, c1;

  XAllocNamedColor( zxdisplay, win->attr.colormap, color, &c1, &c0 );
  return c1.pixel;
}

/* get color by RGB parameters. */
ulong zxGetColorFromRGB(zxWindow *win, uword red, uword green, uword blue)
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

void zxWindowAddEvent(zxWindow *win, long event)
{
  XWindowAttributes attr;

  XGetWindowAttributes( zxdisplay, zxWindowBody(win), &attr );
  XSelectInput( zxdisplay, zxWindowBody(win), attr.all_event_masks | attr.your_event_mask | event );
}

/* get next event */
int zxNextEvent(void)
{
  XNextEvent( zxdisplay, &zxevent );
  return zxevent.type;
}

/* get a pended event */
int zxGetEvent(void)
{
  return XPending( zxdisplay ) > 0 ? zxNextEvent() : None;
}

/* dequeue an event */
int zxDequeueEvent(void)
{
  return XEventsQueued( zxdisplay, QueuedAfterReading ) > 0 ?
    zxNextEvent() : None;
}

/* check if an event occured */
bool zxCheckEvent(int type)
{
  return zxGetEvent() == type ? true : false;
}

/* stacked window manipulation */

/* get the input focus */
Window zxGetInputFocus(void)
{
  Window focused;
  int revert;

  XGetInputFocus( zxdisplay, &focused, &revert );
  return focused;
}

/* get geometry of a drawable */
bool zxGetGeometry(Drawable drw, zxRegion *reg)
{
  uint x, y, w, h, border, depth;
  Window rootwin, child;

  if( XGetGeometry( zxdisplay, drw, &rootwin, (int *)&x, (int *)&y, &w, &h, &border, &depth ) ){
    XTranslateCoordinates( zxdisplay, drw, rootwin, 0, 0, (int *)&x, (int *)&y, &child );
    zxRegionSet( reg, x, y, w, h );
    return true;
  }
  return false;
}

/* drawing methods */

/* set line-width. */
void zxSetLineWidth(zxWindow *win, int width)
{
  win->gv.line_width = width;
  XChangeGC( zxdisplay, zxWindowGC(win), GCLineWidth, &win->gv );
}

/* set line-style. */
void zxSetLineStyle(zxWindow *win, int style)
{
  win->gv.line_style = style;
  XChangeGC( zxdisplay, zxWindowGC(win), GCLineStyle, &win->gv );
}

/* set line-cap. */
void zxSetLineCap(zxWindow *win, int cap)
{
  win->gv.cap_style = cap;
  XChangeGC( zxdisplay, zxWindowGC(win), GCCapStyle, &win->gv );
}

/* set line-join. */
void zxSetLineJoin(zxWindow *win, int join)
{
  win->gv.join_style = join;
  XChangeGC( zxdisplay, zxWindowGC(win), GCJoinStyle, &win->gv );
}

/* draw a filled triangle */
int zxFillTriangle(zxWindow *win, Drawable drw, int x1, int y1, int x2, int y2, int x3, int y3)
{
  XPoint v[3];

  v[0].x = x1; v[0].y = y1;
  v[1].x = x2; v[1].y = y2;
  v[2].x = x3; v[2].y = y3;
  return zxFillPolygon( win, drw, v, 3 );
}

/* draw a polygon filled with a pixmap pattern */
void zxPixmapPolygon(zxWindow *win, Pixmap canvas, XPoint *points, int n, Pixmap pattern)
{
  XSetFillStyle( zxdisplay, zxWindowGC(win), FillTiled );
  XSetTile( zxdisplay, zxWindowGC(win), pattern );
  zxSetFillRule( win, EvenOddRule );
  zxFillPolygonComplex( win, canvas, points, n );
}

/* command line option utility */

bool zxParseGeometry(char *str, zxRegion *reg)
{
  char op[] = { 'x', '+', '+' };

  zxRegionSet( reg, 0, 0, 0, 0 );
  if( isdigit( str[0] ) ){
    switch( sscanf( str, "%hd%c%hd%c%hd%c%hd",
      &reg->width, &op[0], &reg->height, &op[1], &reg->x, &op[2], &reg->y ) ){
    case 2: case 3: if( op[0] != 'x' ) goto ERROR; break;
    case 1: case 4: case 6: goto ERROR;
    default: ;
    }
  } else{
    if( str[0] == 'x' ){
      switch( sscanf( str, "%c%hd%c%hd%c%hd",
        &op[0], &reg->height, &op[1], &reg->x, &op[2], &reg->y ) ){
      case 1: case 3: case 5: goto ERROR;
      default: ;
      }
    } else{
      switch( sscanf( str, "%c%hd%c%hd", &op[1], &reg->x, &op[2], &reg->y ) ){
      case 1: case 3: goto ERROR;
      default: ;
      }
    }
  }
  if( op[1] == '-' ) reg->x = -reg->x;
  else if( op[1] != '+' ) goto ERROR;
  if( op[2] == '-' ) reg->y = -reg->y;
  else if( op[2] != '+' ) goto ERROR;
  return true;

 ERROR:
  ZRUNERROR( "invalid geometry expression: %s", str );
  return false;
}

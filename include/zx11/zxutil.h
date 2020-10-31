/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxutil - utilities for X11
 */

#ifndef __ZX_UTIL_H__
#define __ZX_UTIL_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <zeda/zeda.h>

__BEGIN_DECLS

/* global parameters and macroes */

extern Display *zxdisplay;
extern Window zxrootwindow;
extern Screen *zxscreen;
extern Colormap zxcmap;
extern Visual *zxvisual;
extern int zxdepth;
extern XEvent zxevent;

#define zxScreenWidth()   WidthOfScreen( zxscreen )
#define zxScreenHeight()  HeightOfScreen( zxscreen )

/*! \brief initialize X11 system and get information of display and the root window */
bool zxInit(void);

/*! \brief exit from connection with X11 system */
#define zxExit() do{\
  XCloseDisplay( zxdisplay );\
  zxdisplay = NULL;\
} while( 0 )

/*! \brief flush all events in queue */
#define zxFlush() XFlush( zxdisplay )
/*! \brief synchronize things appearing on display and events */
#define zxSync()  XSync( zxdisplay, true )

/*! \brief set default window attributes */
void zxDefaultSetWindowAttributes(XSetWindowAttributes *attr);

/* aliases of some types in Xlib.h */

typedef XSegment zxSeg;
#define zxSegSet(s,_x1,_y1,_x2,_y2) do{\
  (s)->x1 = (_x1);\
  (s)->y1 = (_y1);\
  (s)->x2 = (_x2);\
  (s)->y2 = (_y2);\
} while(0)

typedef XPoint zxVec;
#define zxVecSet(v,_x,_y) do{\
  (v)->x = (_x);\
  (v)->y = (_y);\
} while(0)

typedef XRectangle zxRegion;
#define zxRegionSetPos(r,_x,_y) do{\
  (r)->x = (_x);\
  (r)->y = (_y);\
} while(0)
#define zxRegionSetSize(r,_w,_h) do{\
  (r)->width = (_w);\
  (r)->height = (_h);\
} while(0)
#define zxRegionSet(r,_x,_y,_w,_h) do{\
  zxRegionSetPos( r, _x, _y );\
  zxRegionSetSize( r, _w, _h );\
} while(0)

/*! \brief check if a point is in a region. */
bool zxRegionIsIn(zxRegion *reg, short x, short y);

/*! \brief make a string that represents a specified region. */
void zxRegionToStr(zxRegion *reg, char *str);

/* for debug */
void zxRegionPrint(zxRegion *reg);

/* ********************************************************** */
/*! \brief window and its contents
 * ********************************************************** */
typedef struct{
  Window body;
  Window parent;   /*!< parent window */
  Drawable canvas; /*!< canvas instance to be put on window */
  Pixmap db;       /*!< drawable for double buffering */
  zxRegion reg;    /*!< window geometry */
  int win_mask;    /*!< event mask */
  ulong fgcolor;   /*!< current foreground color */
  ulong bgcolor;   /*!< background color */
  GC gc;           /*!< graphics context */
  GC dgc;          /*!< default graphics context */
  XGCValues gv;    /*!< values of graphics context */
  XSetWindowAttributes attr; /*!< window attributes */
  XIC ic;          /*!< input context */
} zxWindow;

#define zxWindowBody(win)        (win)->body
#define zxWindowParent(win)      (win)->parent
#define zxWindowOX(win)          (win)->reg.x
#define zxWindowOY(win)          (win)->reg.y
#define zxWindowWidth(win)       (win)->reg.width
#define zxWindowHeight(win)      (win)->reg.height
#define zxWindowRegion(win)      ( &(win)->reg )
#define zxWindowMask(win)        (win)->win_mask

#define zxWindowSetParent(win,p) ( zxWindowParent(win) = (p) )
#define zxWindowSetBody(win,m)   ( zxWindowBody(win) = (m) )
#define zxWindowSetOX(win,x)     ( zxWindowOX(win) = (x) )
#define zxWindowSetOY(win,y)     ( zxWindowOY(win) = (y) )
#define zxWindowSetPos(win,x,y)  zxRegionSetPos( zxWindowRegion(win), x, y )
#define zxWindowSetSize(win,w,h) zxRegionSetSize( zxWindowRegion(win), w, h )
#define zxWindowSetGeometry(win,x,y,width,height) zxRegionSet( zxWindowRegion(win), x, y, width, height )
#define zxWindowSetRegion(win,r) ( *zxWindowRegion(win) = *(r) )
#define zxWindowSetWidth(win,w)  ( zxWindowWidth(win) = (w) )
#define zxWindowSetHeight(win,h) ( zxWindowHeight(win) = (h) )
#define zxWindowSetMask(win,m)   ( zxWindowMask(win) = (m) )

#define zxWindowGC(win)                (win)->gc
#define zxWindowDefaultGC(win)         (win)->dgc
#define zxWindowCanvas(win)            (win)->canvas

#define zxWindowSetGC(win,g)           ( (win)->gc = (g) )
#define zxWindowSetDefaultGC(win)      ( (win)->gc = (win)->dgc )
#define zxWindowSetCanvas(win,c)       ( zxWindowCanvas(win) = (c) )

/*! \brief create a primary window */
void zxWindowCreate(zxWindow *win, int x, int y, int w, int h);
/*! \brief create a child window of another */
void zxWindowCreateChild(zxWindow *win, zxWindow *parent, int x, int y, int w, int h);
/*! \brief create a full-size window as a fake root */
void zxWindowCreateRoot(zxWindow *win);
/*! \brief destroy a window and an associated graphics context */
void zxWindowDestroy(zxWindow *win);

/*! \brief open a window */
#define zxWindowOpen(win) do{\
  XMapWindow( zxdisplay, zxWindowBody(win) );\
  zxWindowMove( win, zxWindowOX(win), zxWindowOY(win) );\
  zxWindowResize( win, zxWindowWidth(win), zxWindowHeight(win) );\
  while( zxNextEvent() != Expose );\
} while(0)
/*! \brief create and open a window */
#define zxWindowCreateAndOpen(win,x,y,w,h) do{\
  zxWindowCreate( win, x, y, w, h );\
  zxWindowOpen( win );\
} while(0)
/*! \brief create and open a child window */
#define zxWindowCreateAndOpenChild(win,parent,x,y,w,h) do{\
  zxWindowCreateChild( win, parent, x, y, w, h );\
  zxWindowOpen( win );\
} while(0)

/*! \brief clear window with background color */
void zxWindowClear(zxWindow *win);
/*! \brief clear specified area of a window */
void zxWindowClearArea(zxWindow *win, int x, int y, int w, int h);

/*! \brief set background color of a window */
#define zxWindowSetBGColor(win,b) do{\
  (win)->bgcolor = (b);\
  zxWindowClear( win );\
} while(0)
/*! \brief set background color of a window by name */
#define zxWindowSetBGColorByName(win,b) zxWindowSetBGColor( win, zxGetColor( (win), b ) )

/*! \brief fill a window with a specified pixmap pattern */
void zxWindowFill(zxWindow *win, Pixmap pattern);

/*! \brief close a window */
#define zxWindowClose(win) XUnmapWindow( zxdisplay, zxWindowBody(win) )

/*! \brief move a window to a specified position */
#define zxWindowMove(win,x,y) do{\
  zxWindowSetPos( win, x, y );\
  XMoveWindow( zxdisplay, zxWindowBody(win), (x), (y) );\
} while(0)
/*! \brief resize a window to specified width and height */
#define zxWindowResize(win,w,h) do{\
  zxWindowSetSize( win, w, h );\
  XResizeWindow( zxdisplay, zxWindowBody(win), (w), (h) );\
} while(0)
/*! \brief move and resize a window to a specified geometry */
#define zxWindowMoveResize(win,r) do{\
  zxWindowMove( (win), (r)->x, (r)->y );\
  zxWindowResize( (win), (r)->width, (r)->height );\
} while(0)
/*! \brief raise a window on the top of visible stack */
#define zxWindowRaise(win) XRaiseWindow( zxdisplay, zxWindowBody(win) )
/*! \brief lower a window to the bottom of visible stack */
#define zxWindowLower(win) XLowerWindow( zxdisplay, zxWindowBody(win) )
/*! \brief set position and size of a window according to event */
#define zxWindowMoveResizeEvent(win) do{\
  zxWindowSetPos( win, zxevent.xexpose.x, zxevent.xexpose.y );\
  zxWindowSetSize( win, zxevent.xexpose.width, zxevent.xexpose.height );\
} while(0)

/*! \brief update geometry information of a window */
bool zxWindowUpdateRegion(zxWindow *win);

/*! \brief set the title of a window */
#define zxWindowSetTitle(win,t) XStoreName( zxdisplay, zxWindowBody(win), t )
/*! \brief get the title of a window */
#define zxWindowGetTitle(win,t) XFetchName( zxdisplay, zxWindowBody(win), t )

/*! \brief set a border color of a window */
#define zxWindowSetBorderColor(win,b) XSetWindowBorder( zxdisplay, zxWindowBody(win), b )
/*! \brief set a border width of a window */
#define zxWindowSetBorderWidth(win,w) XSetWindowBorderWidth( zxdisplay, zxWindowBody(win), w )

/*! \brief enable backing-store */
void zxWindowBackingStoreEnable(zxWindow *win);
/*! \brief disable backing-store */
void zxWindowBackingStoreDisable(zxWindow *win);
/*! \brief enable override redirection to avoid the interrupt from window manager */
void zxWindowOverrideRedirectEnable(zxWindow *win);

/*! \brief receive event to delete a window */
bool zxWindowIsReceivedDeleteMsg(zxWindow *win);

/* pixmaps */

/*! \brief create a pixmap */
#define zxPixmapCreate(win,w,h) \
  XCreatePixmap( zxdisplay, zxWindowBody(win), (w), (h), zxdepth )
/*! \brief create a mask on a pixmap */
#define zxPixmapCreateMask(win,w,h) \
  XCreatePixmap( zxdisplay, zxWindowBody(win), (w), (h), 1 )
/*! \brief destroy a pixmap */
#define zxPixmapDestroy(p) do{\
  if( (p) != None ) XFreePixmap( zxdisplay, (p) );\
} while(0)
/*! \brief copy a pixmap to another */
#define zxPixmapCopy(win,s,d,src_x,src_y,w,h,dest_x,dest_y) \
  XCopyArea( zxdisplay, (s), (d), zxWindowGC(win), (src_x), (src_y), (w), (h), (dest_x), (dest_y) )
/*! \brief clone a pixmap */
Pixmap zxPixmapClone(zxWindow *win, Pixmap src, int x, int y, int width, int height);

/*! \brief get size of a pixmap */
bool zxPixmapGetSize(Drawable drw, int *width, int *height);

#define zxClipSetOrigin(win,x,y) \
  XSetClipOrigin( zxdisplay, zxWindowGC(win), (x), (y) )
#define zxClipSetMask(win,m) \
  XSetClipMask( zxdisplay, zxWindowGC(win), (m) )
#define zxClipUnsetMask(win) \
  zxClipSetMask( (win), None )

#define zxPixmapDraw(win,p,src_x,src_y,w,h,dest_x,dest_y) \
  zxPixmapCopy( win, p, zxWindowCanvas(win), src_x, src_y, w, h, dest_x, dest_y )
#define zxPixmapDrawDirect(win,p,src_x,src_y,w,h,dest_x,dest_y) \
  zxPixmapCopy( win, p, zxWindowBody(win), src_x, src_y, w, h, dest_x, dest_y )

/* double bufferring */

/* set double-buffer */
#define zxWindowDoubleBufferSet(win,buf) do{\
  if( ( (win)->db = (buf) ) != None ) zxWindowSetCanvas( win, (win)->db );\
} while(0)

/* enable double buffering */
#define zxWindowDoubleBufferEnable(win) \
  zxWindowDoubleBufferSet( win, zxPixmapCreate( win, zxWindowWidth(win), zxWindowHeight(win) ) )

/* disable double buffering */
#define zxWindowDoubleBufferDisable(win) do{\
  zxWindowSetCanvas( win, zxWindowBody(win) );\
  zxPixmapDestroy( (win)->db );\
} while(0)

/* expose double buffer */
#define zxWindowDoubleBufferAppear(win) \
  zxPixmapDrawDirect( win, (win)->db, 0, 0, zxWindowWidth(win), zxWindowHeight(win), 0, 0 )

/* partially expose double buffer */
#define zxWindowDoubleBufferPartAppear(win,ox,oy,w,h) \
  zxPixmapDrawDirect( win, (win)->db, ox, oy, w, h, ox, oy )

/* color operation methods */

/* create a color map */
void zxWindowColorMapCreate(zxWindow *win);

/* get color by a name. */
ulong zxGetColor(zxWindow *win, char color[]);
/* get color by RGB parameters. */
ulong zxGetColorFromRGB(zxWindow *win, uword red, uword green, uword blue);

/*! \brief set foreground color of a window */
#define zxWindowSetColor(win,i) XSetForeground( zxdisplay, zxWindowGC(win), ( (win)->fgcolor = i ) )
/*! \brief set foreground color of a window by name */
#define zxWindowSetColorByName(win,name) zxWindowSetColor( win, zxGetColor((win),name) )

enum{ ZX_RGB, ZX_GS };
typedef char zxColorMode;

/* event operation methods */

void zxWindowAddEvent(zxWindow *win, long event);

/* get next event */
int zxNextEvent(void);
/* get a pended event */
int zxGetEvent(void);
/* dequeue an event */
int zxDequeueEvent(void);
/* check if an event occured */
bool zxCheckEvent(int type);

/* stacked window manipulation */

/* get the input focus */
Window zxGetInputFocus(void);

/* get geometry of a drawable */
bool zxGetGeometry(Drawable drw, zxRegion *reg);

/* drawing methods */

/* set line-width. */
void zxSetLineWidth(zxWindow *win, int width);
/* set line-style. */
void zxSetLineStyle(zxWindow *win, int style);
/* set line-cap. */
void zxSetLineCap(zxWindow *win, int cap);
/* set line-join. */
void zxSetLineJoin(zxWindow *win, int join);

#define zxPoint(win,p,x,y) \
  XDrawPoint( zxdisplay, (p), zxWindowGC(win), (x), (y) )
#define zxDrawPoint(win,x,y) \
  zxPoint( win, zxWindowCanvas(win), x, y )

#define zxLine(win,p,x1,y1,x2,y2) \
  XDrawLine( zxdisplay, (p), zxWindowGC(win), (x1), (y1), (x2), (y2) )
#define zxDrawLine(win,x1,y1,x2,y2) \
  zxLine( win, zxWindowCanvas(win), x1, y1, x2, y2 )

#define zxRect(win,p,x,y,w,h) \
  XDrawRectangle( zxdisplay, p, zxWindowGC(win), (x), (y), (w)-1, (h)-1 )
#define zxDrawRect(win,x,y,w,h) \
  zxRect( win, zxWindowCanvas(win), x, y, w, h )

#define zxFillRect(win,p,x,y,w,h) \
  XFillRectangle( zxdisplay, p, zxWindowGC(win), (x), (y), (w), (h) )
#define zxDrawFillRect(win,x,y,w,h) \
  zxFillRect( win, zxWindowCanvas(win), x, y, w, h )

#define zxRegionRect(win,p,r) \
  zxRect( win, p, (r)->x, (r)->y, (r)->width, (r)->height )
#define zxDrawRegionRect(win,r) \
  zxRegionRect( win, zxWindowCanvas(win), r )

#define zxRegionFillRect(win,p,r) \
  zxFillRect( win, p, (r)->x, (r)->y, (r)->width, (r)->height )
#define zxDrawRegionFillRect(win,r) \
  zxRegionFillRect( win, zxWindowCanvas(win), r )

#define zxArc(win,p,x,y,w,h,a1,a2) \
  XDrawArc( zxdisplay, (p), zxWindowGC(win), (x), (y), (w), (h), (a1)*64, (a2)*64 )
#define zxDrawArc(win,x,y,w,h,a1,a2) \
  zxArc( win, zxWindowCanvas(win), x, y, w, h, a1, a2 )

#define zxFillArc(win,p,x,y,w,h,a1,a2) \
  XFillArc( zxdisplay, (p), zxWindowGC(win), (x), (y), (w), (h), (a1)*64, (a2)*64 )
#define zxDrawFillArc(win,x,y,w,h,a1,a2) \
  zxFillArc( win, zxWindowCanvas(win), x, y, w, h, a1, a2 )

#define zxCircle(win,p,x,y,r) \
  zxArc( win, (p), (x)-(r), (y)-(r), 2*(r), 2*(r), 0, 360 )
#define zxDrawCircle(win,x,y,r) \
  zxCircle( win, zxWindowCanvas(win), x, y, r )

#define zxFillCircle(win,p,x,y,r) \
  zxFillArc( win, (p), (x)-(r), (y)-(r), 2*(r), 2*(r), 0, 360 )
#define zxDrawFillCircle(win,x,y,r) \
  zxFillCircle( win, zxWindowCanvas(win), x, y, r )

#define zxLines(win,p,v,n) \
  XDrawLines( zxdisplay, (p), zxWindowGC(win), (XPoint*)(v), (n), CoordModeOrigin )
#define zxDrawLines(win,v,n) \
  zxLines( win, zxWindowCanvas(win), v, n )

/* polygon-filling rule (EvenOddRule/WindingRule) */
#define zxSetFillRule(win,r) \
  XSetFillRule( zxdisplay, zxWindowGC(win), r )

#define zxFillPolygon(win,p,v,n) \
  XFillPolygon( zxdisplay, (p), zxWindowGC(win), (XPoint*)(v), (n), Convex, CoordModeOrigin )
#define zxDrawFillPolygon(win,v,n) \
  zxFillPolygon( win, zxWindowCanvas(win), v, n )

#define zxFillPolygonNonConvex(win,p,v,n) \
  XFillPolygon( zxdisplay, (p), zxWindowGC(win), (XPoint*)(v), (n), Nonconvex, CoordModeOrigin )
#define zxDrawFillPolygonNonConvex(win,v,n) \
  zxFillPolygonNonConvex( win, zxWindowCanvas(win), v, n )

#define zxFillPolygonComplex(win,p,v,n) \
  XFillPolygon( zxdisplay, (p), zxWindowGC(win), (XPoint*)(v), (n), Complex, CoordModeOrigin )
#define zxDrawFillPolygonComplex(win,v,n) \
  zxFillPolygonComplex( win, zxWindowCanvas(win), v, n )

/* draw a polygon filled with a pixmap patter */
void zxPixmapPolygon(zxWindow *win, Pixmap canvas, XPoint *points, int n, Pixmap pattern);
#define zxDrawPixmapPolygon(win,v,n,pat) \
  zxPixmapPolygon( (win), zxWindowCanvas(win), (v), (n), (pat) )

/* command line option utility */

/*! \brief parse a string for geometry parameters.
 *
 * zxParseGeometry() parses a string \a str that represents geometry
 * parameters into a region \a reg. Valid forms of the strings are:
 *  Wx
 *  WxH
 *  WxH(+/-)X
 *  WxH(+/-)X(+/-)Y
 *  xH
 *  xH(+/-)X
 *  xH(+/-)X(+/-)Y
 *  (+/-)X
 *  (+/-)X(+/-)Y
 * \return
 * zxParseGeometry() returns the true value if it succeeds to parse
 * \a str. Otherwise, the false value is returned.
 * \sa XParseGeometry
 */
bool zxParseGeometry(char *str, zxRegion *reg);

__END_DECLS

#endif /* __ZX_UTIL_H__ */

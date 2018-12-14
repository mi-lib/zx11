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

/* global structures and macroes */

extern Display *zxdisplay;
extern Window zxrootwindow;
extern Screen *zxscreen;
extern Colormap zxcmap;
extern Visual *zxvisual;
extern int zxdepth;
extern XEvent zxevent;

#define Z_DEFAULT_SCREEN 0

#define zxScreenWidth()   WidthOfScreen( zxscreen )
#define zxScreenHeight()  HeightOfScreen( zxscreen )

/* types redefinitions(see 'Xlib.h') */

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

bool zxRegionIsIn(zxRegion *reg, short x, short y);
void zxRegionToStr(zxRegion *reg, char *str);
/* for debug */
void zxRegionWrite(zxRegion *reg);

/* ********************************************************** */
/* CLASS: zxWindow
 * window and its contents
 * ********************************************************** */

typedef struct{
  Window parent, body;
  Drawable canvas;
  Pixmap db; /* for double buffering */
  zxRegion reg;
  int win_mask;
  ulong fg, bg;
  GC gc, dgc;
  XGCValues gv;
  XSetWindowAttributes attr;
} zxWindow;

#define zxWindowBody(win)        (win)->body
#define zxWindowParent(win)      (win)->parent
#define zxWindowOX(win)          (win)->reg.x
#define zxWindowOY(win)          (win)->reg.y
#define zxWindowWidth(win)       (win)->reg.width
#define zxWindowHeight(win)      (win)->reg.height
#define zxWindowRegion(win)      ( &(win)->reg )
#define zxWindowMask(win)        (win)->win_mask
#define zxFGColor(win)           (win)->fg
#define zxBGColor(win)           (win)->bg
#define zxGC(win)                (win)->gc
#define zxDefaultGC(win)         (win)->dgc
#define zxCanvas(win)            (win)->canvas

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

#define zxSetFGColor(win,f)      zxSetColorMap( (win), ( zxFGColor(win) = (f) ) )
#define zxSetBGColor(win,b)      ( zxBGColor(win) = (b) )
#define zxSetGC(win,g)           ( (win)->gc = (g) )
#define zxSetDefaultGC(win)      ( (win)->gc = (win)->dgc )
#define zxSetCanvas(win,c)       ( zxCanvas(win) = (c) )

/* basic methods */

void zxInit(void);
#define zxClose() XCloseDisplay( zxdisplay )
#define zxFlush() XFlush( zxdisplay )
#define zxSync()  XSync( zxdisplay, true )

void zxDefaultSetWindowAttributes(XSetWindowAttributes *attr);

void zxWindowCreate(zxWindow *win, int x, int y, int w, int h);
void zxWindowCreateChild(zxWindow *win, zxWindow *parent, int x, int y, int w, int h);
void zxWindowCreateRoot(zxWindow *win);
void zxWindowDestroy(zxWindow *win);

#define zxWindowOpen(win) do{\
  XMapWindow( zxdisplay, zxWindowBody(win) );\
  zxWindowMove( win, zxWindowOX(win), zxWindowOY(win) );\
  zxWindowResize( win, zxWindowWidth(win), zxWindowHeight(win) );\
  while( zxNextEvent() != Expose );\
} while(0)
#define zxWindowCreateAndOpen(win,x,y,w,h) do{\
  zxWindowCreate( win, x, y, w, h );\
  zxWindowOpen( win );\
} while(0)

void zxWindowClear(zxWindow *win);
void zxWindowClearArea(zxWindow *win, int x, int y, int w, int h);

#define zxWindowSetFG(win,f) zxSetFGColor( win, zxGetColor( (win), f ) )
#define zxWindowSetBG(win,b) do{\
  zxSetBGColor( win, zxGetColor( (win), b ) );\
  zxWindowClear( win );\
} while(0)
void zxWindowFill(zxWindow *win, Pixmap pattern);
#define zxWindowClose(win) XUnmapWindow( zxdisplay, zxWindowBody(win) )

#define zxWindowMove(win,x,y) do{\
  zxWindowSetPos( win, x, y );\
  XMoveWindow( zxdisplay, zxWindowBody(win), (x), (y) );\
} while(0)
#define zxWindowResize(win,w,h) do{\
  zxWindowSetSize( win, w, h );\
  XResizeWindow( zxdisplay, zxWindowBody(win), (w), (h) );\
} while(0)
#define zxWindowMoveResize(win,r) do{\
  zxWindowMove( (win), (r)->x, (r)->y );\
  zxWindowResize( (win), (r)->width, (r)->height );\
} while(0)
#define zxWindowRaise(win) XRaiseWindow( zxdisplay, zxWindowBody(win) )
#define zxWindowLower(win) XLowerWindow( zxdisplay, zxWindowBody(win) )

bool zxWindowUpdateRegion(zxWindow *win);

#define zxWindowSetTitle(win,t) \
  XStoreName( zxdisplay, zxWindowBody(win), t )
#define zxWindowGetTitle(win,t) \
  XFetchName( zxdisplay, zxWindowBody(win), t )

#define zxWindowSetBorder(win,b) \
  XSetWindowBorder( zxdisplay, zxWindowBody(win), b )
#define zxWindowSetBorderWidth(win,w) \
  XSetWindowBorderWidth( zxdisplay, zxWindowBody(win), w )

void zxBackingStoreOn(zxWindow *win);
void zxBackingStoreOff(zxWindow *win);
void zxOverrideRedirect(zxWindow *win);

bool zxDeleteWindowEvent(void);

/* pixmaps */

#define zxPixmapCreate(win,w,h) \
  XCreatePixmap( zxdisplay, zxWindowBody(win), (w), (h), zxdepth )
#define zxPixmapCreateMask(win,w,h) \
  XCreatePixmap( zxdisplay, zxWindowBody(win), (w), (h), 1 )
#define zxPixmapDestroy(p) \
  XFreePixmap( zxdisplay, (p) )
#define zxPixmapCopy(win,s,d,src_x,src_y,w,h,dest_x,dest_y) \
  XCopyArea( zxdisplay, (s), (d), zxGC(win), (src_x), (src_y), (w), (h), (dest_x), (dest_y) )
Pixmap zxPixmapClone(zxWindow *win, Pixmap src, int x, int y, int width, int height);

bool zxPixmapGetSize(Drawable drw, int *width, int *height);

#define zxClipSetOrigin(win,x,y) \
  XSetClipOrigin( zxdisplay, zxGC(win), (x), (y) )
#define zxClipSetMask(win,m) \
  XSetClipMask( zxdisplay, zxGC(win), (m) )
#define zxClipUnsetMask(win) \
  zxClipSetMask( (win), None )

#define zxPixmapDraw(win,p,src_x,src_y,w,h,dest_x,dest_y) \
  zxPixmapCopy( win, p, zxCanvas(win), src_x, src_y, w, h, dest_x, dest_y )
#define zxPixmapDrawDirect(win,p,src_x,src_y,w,h,dest_x,dest_y) \
  zxPixmapCopy( win, p, zxWindowBody(win), src_x, src_y, w, h, dest_x, dest_y )

/* double bufferring */

void zxDoubleBufferEnable(zxWindow *win);
void zxDoubleBufferDisable(zxWindow *win);

#define zxDoubleBufferAppear(win) \
  zxPixmapDrawDirect( win, (win)->db, 0, 0, zxWindowWidth(win), zxWindowHeight(win), 0, 0 )
#define zxDoubleBufferPartAppear(win,ox,oy,w,h) \
  zxPixmapDrawDirect( win, (win)->db, ox, oy, w, h, ox, oy )

/* color operation methods */

void zxColormapCreate(zxWindow *win);
ulong zxGetColor(zxWindow *win, char color[]);
ulong zxGetRGBColor(zxWindow *win, uword red, uword green, uword blue);

#define zxSetColorMap(win,i) XSetForeground( zxdisplay, zxGC(win), i )
#define zxSetColor(win,name) zxSetColorMap( win, zxGetColor((win),name) )

enum{ ZX_RGB, ZX_GS };
typedef char zxColorMode;

/* event operation methods */

#define zxSetEventMask(win,m) \
  XSelectInput( zxdisplay, zxWindowBody(win), ( (win)->attr.event_mask = (m) ) )
#define zxAddEventMask(win,m) \
  XSelectInput( zxdisplay, zxWindowBody(win), ( (win)->attr.event_mask |= (m) ) )

#define zxEventAccept(win) ( zxevent.xany.window == zxWindowBody(win) ? true : false )

int zxNextEvent(void);
int zxGetEvent(void);
int zxDequeueEvent(void);
bool zxCheckEvent(int type);

/* stacked window manipulation */

Window zxGetInputFocus(void);

bool zxGetGeometry(Drawable drw, zxRegion *reg);

/* drawing methods */

void zxSetLineWidth(zxWindow *win, int width);
void zxSetLineStyle(zxWindow *win, int style);
void zxSetLineCap(zxWindow *win, int cap);
void zxSetLineJoin(zxWindow *win, int join);

#define zxPoint(win,p,x,y) \
  XDrawPoint( zxdisplay, (p), zxGC(win), (x), (y) )
#define zxDrawPoint(win,x,y) \
  zxPoint( win, zxCanvas(win), x, y )

#define zxLine(win,p,x1,y1,x2,y2) \
  XDrawLine( zxdisplay, (p), zxGC(win), (x1), (y1), (x2), (y2) )
#define zxDrawLine(win,x1,y1,x2,y2) \
  zxLine( win, zxCanvas(win), x1, y1, x2, y2 )

#define zxRect(win,p,x,y,w,h) \
  XDrawRectangle( zxdisplay, p, zxGC(win), (x), (y), (w)-1, (h)-1 )
#define zxDrawRect(win,x,y,w,h) \
  zxRect( win, zxCanvas(win), x, y, w, h )

#define zxFillRect(win,p,x,y,w,h) \
  XFillRectangle( zxdisplay, p, zxGC(win), (x), (y), (w), (h) )
#define zxDrawFillRect(win,x,y,w,h) \
  zxFillRect( win, zxCanvas(win), x, y, w, h )

#define zxRegionRect(win,p,r) \
  zxRect( win, p, (r)->x, (r)->y, (r)->width, (r)->height )
#define zxDrawRegionRect(win,r) \
  zxRegionRect( win, zxCanvas(win), r )

#define zxRegionFillRect(win,p,r) \
  zxFillRect( win, p, (r)->x, (r)->y, (r)->width, (r)->height )
#define zxDrawRegionFillRect(win,r) \
  zxRegionFillRect( win, zxCanvas(win), r )

#define zxArc(win,p,x,y,w,h,a1,a2) \
  XDrawArc( zxdisplay, (p), zxGC(win), (x), (y), (w), (h), (a1)*64, (a2)*64 )
#define zxDrawArc(win,x,y,w,h,a1,a2) \
  zxArc( win, zxCanvas(win), x, y, w, h, a1, a2 )

#define zxFillArc(win,p,x,y,w,h,a1,a2) \
  XFillArc( zxdisplay, (p), zxGC(win), (x), (y), (w), (h), (a1)*64, (a2)*64 )
#define zxDrawFillArc(win,x,y,w,h,a1,a2) \
  zxFillArc( win, zxCanvas(win), x, y, w, h, a1, a2 )

#define zxCircle(win,p,x,y,r) \
  zxArc( win, (p), (x)-(r), (y)-(r), 2*(r), 2*(r), 0, 360 )
#define zxDrawCircle(win,x,y,r) \
  zxCircle( win, zxCanvas(win), x, y, r )

#define zxFillCircle(win,p,x,y,r) \
  zxFillArc( win, (p), (x)-(r), (y)-(r), 2*(r), 2*(r), 0, 360 )
#define zxDrawFillCircle(win,x,y,r) \
  zxFillCircle( win, zxCanvas(win), x, y, r )

#define zxLines(win,p,v,n) \
  XDrawLines( zxdisplay, (p), zxGC(win), (XPoint*)(v), (n), CoordModeOrigin )
#define zxDrawLines(win,v,n) \
  zxLines( win, zxCanvas(win), v, n )

/* polygon-filling rule (EvenOddRule/WindingRule) */
#define zxSetFillRule(win,r) \
  XSetFillRule( zxdisplay, zxGC(win), r )

#define zxFillPolygon(win,p,v,n) \
  XFillPolygon( zxdisplay, (p), zxGC(win), (XPoint*)(v), (n), Convex, CoordModeOrigin )
#define zxDrawFillPolygon(win,v,n) \
  zxFillPolygon( win, zxCanvas(win), v, n )

#define zxFillPolygonNonConvex(win,p,v,n) \
  XFillPolygon( zxdisplay, (p), zxGC(win), (XPoint*)(v), (n), Nonconvex, CoordModeOrigin )
#define zxDrawFillPolygonNonConvex(win,v,n) \
  zxFillPolygonNonConvex( win, zxCanvas(win), v, n )

#define zxFillPolygonComplex(win,p,v,n) \
  XFillPolygon( zxdisplay, (p), zxGC(win), (XPoint*)(v), (n), Complex, CoordModeOrigin )
#define zxDrawFillPolygonComplex(win,v,n) \
  zxFillPolygonComplex( win, zxCanvas(win), v, n )

void zxPixmapPolygon(zxWindow *win, Pixmap canvas, XPoint *points, int n, Pixmap pattern);
#define zxDrawPixmapPolygon(win,v,n,pat) \
  zxPixmapPolygon( (win), zxCanvas(win), (v), (n), (pat) )

void zxClear(zxWindow *win);

void zxPutBuffer(zxWindow *win, int x, int y, int w, int h, ulong *buf);

/* command line option utility */

#define ZX_GEOMETRY_NONE -1
int zxParseGeometry(char *str, zxRegion *reg);

__END_DECLS

#endif /* __ZX_UTIL_H__ */

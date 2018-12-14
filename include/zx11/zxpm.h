/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxpm - Xpm support(requires libXpm).
 */

#ifndef __ZX_XPM_H__
#define __ZX_XPM_H__

#include <zx11/zximage.h>
#ifdef __ZX11_USE_XPM
#include <X11/xpm.h>
#endif /* __ZX11_USE_XPM */

__BEGIN_DECLS

typedef struct{
  int width, height;
  Pixmap pmap, mask;
} zxPM;

#define zxPMInit(p) do{\
  (p)->width = (p)->height = 0;\
  (p)->pmap = (p)->mask = None;\
} while(0)

void zxPMCreate(zxWindow *win, zxPM *pm, int width, int height);
#define zxPMCreateMask(win,p) ( (p)->mask = zxPixmapCreateMask(win,(p)->width,(p)->height) )
void zxPMClear(zxWindow *win, zxPM *pm);
#define zxPMDestroy(p) do{\
  if( (p)->pmap != None ) XFreePixmap( zxdisplay, (p)->pmap );\
  if( (p)->mask != None ) XFreePixmap( zxdisplay, (p)->mask );\
} while(0)

void zxPMCopy(zxWindow *win, zxPM *pm, Pixmap pmap, int src_x, int src_y, int w, int h, int dest_x, int dest_y);
#define zxPMCopyRegion(win,pm,p,r,x,y) \
  zxPMCopy( (win), (pm), (p), (r)->x, (r)->y, (r)->width, (r)->height, (x), (y) )
#define zxPMCopyAll(win,pm,p,x,y) \
  zxPMCopy( (win), (pm), (p), 0, 0, (p)->width, (p)->height, (x), (y) )

#define zxPMDraw(win,p,src_x,src_y,w,h,dest_x,dest_y) do{\
  zxPMCopy(win,p,zxCanvas(win),src_x,src_y,w,h,dest_x,dest_y);\
  zxFlush();\
} while(0)
#define zxPMDrawRegion(win,p,r,x,y) \
  zxPMDraw( (win), (p), (r)->x, (r)->y, (r)->width, (r)->height, (x), (y) )
#define zxPMDrawAll(win,p,x,y) \
  zxPMDraw( (win), (p), 0, 0, (p)->width, (p)->height, (x), (y) )

void zxPMCromachy(zxWindow *win, Pixmap canvas, zxPM *pm, int src_x, int src_y, Pixmap bgmap, int src_bx, int src_by, int width, int height, int dest_x, int dest_y);
#define zxPMDrawCromachy(win,p,sx,sy,b,sbx,sby,w,h,dx,dy) do{\
  zxPMCromachy( (win), zxCanvas(win), (p), (sx), (sy), (b), (sbx), (sby), (w), (h), (dx), (dy) );\
  zxFlush();\
} while(0)
#define zxPMDrawCromachyAll(win,p,b,sbx,sby,dx,dy) \
  zxPMDrawCromachy(win,p,0,0,b,sbx,sby,(p)->width,(p)->height,dx,dy)

#ifdef __ZX11_USE_XEXT
void zxPMClipShape(zxWindow *win, zxPM *pm);
#endif /* __ZX11_USE_XEXT */

#ifdef __ZX11_USE_XPM
int zxPMReadFile(zxWindow *win, zxPM *pm, char name[]);
int zxPMWriteFile(zxPM *pm, char name[]);
int zxPMReadData(zxWindow *win, zxPM *pm, char *data[]);
int zxPMWriteData(zxPM *pm, char **data[]);
#endif /* __ZX11_USE_XPM */

void zxImageToPM(zxWindow *win, zxImage *img, zxPM *pm);

__END_DECLS

#endif /* __ZX_XPM_H__ */

/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxpm - Xpm support(requires libXpm).
 */

#include <zx11/zxpm.h>

void zxPMCreate(zxWindow *win, zxPM *pm, int width, int height)
{
  pm->width = width;
  pm->height = height;
  pm->pmap = zxPixmapCreate( win, width, height );
  pm->mask = None;
}

void zxPMClear(zxWindow *win, zxPM *pm)
{
  zxSetColorMap( win, zxBGColor(win) );
  zxFillRect( win, pm->pmap, 0, 0, pm->width, pm->height );
}

void zxPMCopy(zxWindow *win, zxPM *pm, Pixmap pmap, int src_x, int src_y, int w, int h, int dest_x, int dest_y)
{
  if( pm->mask != None ){
    zxClipSetOrigin( win, dest_x - src_x, dest_y - src_y );
    zxClipSetMask( win, pm->mask );
  }
  zxPixmapCopy( win, pm->pmap, pmap, src_x, src_y, w, h, dest_x, dest_y );
  if( pm->mask != None )
    zxClipUnsetMask( win );
}

void zxPMCromachy(zxWindow *win, Pixmap canvas, zxPM *pm, int src_x, int src_y, Pixmap bgmap, int src_bx, int src_by, int w, int h, int dest_x, int dest_y)
{
  Pixmap unif;

  if( ( unif = zxPixmapCreate( win, w, h ) ) == None )
    return;
  zxPixmapCopy( win, bgmap, unif, src_bx, src_by, w, h, 0, 0 );
  zxPMCopy( win, pm, unif, src_x, src_y, w, h, 0, 0 );
  zxPixmapCopy( win, unif, canvas, 0, 0, w, h, dest_x, dest_y );
  zxPixmapDestroy( unif );
}

#ifdef __ZX11_USE_XEXT
#include <X11/extensions/shape.h>

void zxPMClipShape(zxWindow *win, zxPM *pm)
{
  XShapeCombineMask( zxdisplay, zxWindowBody(win), ShapeBounding,
    0, 0, (pm)->mask, ShapeSet );
}
#endif /* __ZX11_USE_XEXT */

#define _zxPMSetAttr(a) do{\
  (a)->valuemask = XpmCloseness;\
  (a)->depth = zxdepth;\
} while(0)

#define _zxPMGetAttr(a,p) do{\
  (p)->width = (a)->width;\
  (p)->height = (a)->height;\
} while(0)

#define _zxPMAssert(s) do{\
  if( (s) != XpmSuccess )\
    ZRUNERROR( "%s", XpmGetErrorString( (s) ) );\
} while(0)

int zxPMReadFile(zxWindow *win, zxPM *pm, char name[])
{
  int status;
  XpmAttributes attr;

  _zxPMSetAttr( &attr );
  status = XpmReadFileToPixmap( zxdisplay, zxWindowBody(win), name,
    &pm->pmap, &pm->mask, &attr );
  _zxPMGetAttr( &attr, pm );
  _zxPMAssert( status );
  return status;
}

int zxPMWriteFile(zxPM *pm, char name[])
{
  int status;

  status = XpmWriteFileFromPixmap( zxdisplay, name,
    pm->pmap, pm->mask, NULL );
  _zxPMAssert( status );
  return status;
}

int zxPMReadData(zxWindow *win, zxPM *pm, char *data[])
{
  int status;
  XpmAttributes attr;

  _zxPMSetAttr( &attr );
  status = XpmCreatePixmapFromData( zxdisplay, zxWindowBody(win), data,
    &pm->pmap, &pm->mask, &attr );
  _zxPMGetAttr( &attr, pm );
  _zxPMAssert( status );
  return status;
}

int zxPMWriteData(zxPM *pm, char **data[])
{
  int status;

  status = XpmCreateDataFromPixmap( zxdisplay, data,
    pm->pmap, pm->mask, NULL );
  _zxPMAssert( status );
  return status;
}

void zxImageToPM(zxWindow *win, zxImage *img, zxPM *pm)
{
  zxPMInit( pm );
  zxPMCreate( win, pm, img->width, img->height );
  zxImageToPixmapAll( win, img, pm->pmap );
  pm->mask = zxImageCreatePixmapMask( win, img );
}

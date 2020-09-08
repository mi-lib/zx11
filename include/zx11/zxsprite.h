/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxsprite - sprite pattern on X
 */

#ifndef __ZX_SPRITE_H__
#define __ZX_SPRITE_H__

#include <zx11/zxpm.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zxsPattern
 * ********************************************************** */

typedef struct{
  zxPM *pix;
  uword row, col; /* number of row and column of pix */
  uword dx, dy; /* width and height of pattern */
  struct{
    uword ox, oy; /* offset of original point in pix */
    short x, y;   /* drawing destination position */
  } cur, cur_pre; /* present and previous cursor */
} zxsPatternData;
zListClass( zxsPatternList, zxsPattern, zxsPatternData );

zxsPattern *zxsPatternInit(zxsPattern *pat);
#define zxsPatternSetPixmap(p,m) ( (p)->data.pix = (m) )
#define zxsPatternDestroy(p) zxsPatternInit(p)

#define zxsPatternSetRow(p,r) ( (p)->data.row = (r) )
#define zxsPatternSetCol(p,c) ( (p)->data.col = (c) )
#define zxsPatternSetMatrix(p,r,c) {\
  zxsPatternSetRow( p, r );\
  zxsPatternSetCol( p, c ); }

#define zxsPatternSetDX(p,d) ( (p)->data.dx = (d) )
#define zxsPatternSetDY(p,d) ( (p)->data.dy = (d) )
#define zxsPatternSetSize(p,x,y) {\
  zxsPatternSetDX( p, x );\
  zxsPatternSetDY( p, y ); }

#define zxsPatternDivMatrix(p,r,c) {\
  zxsPatternSetMatrix( p, r, c );\
  zxsPatternSetSize( p, (p)->data.pix->width/(c), (p)->data.pix->height/(r) ); }

#define zxsPatternSet(p,r,c) {\
  if( (r) < (p)->data.row && (c) < (p)->data.col ){\
    (p)->data.cur.ox = (p)->data.dx * (c);\
    (p)->data.cur.oy = (p)->data.dy * (r); } }

#define zxsPatternRecPos(p) ( (p)->data.cur_pre = (p)->data.cur )
#define zxsPatternSetX(p,v) ( (p)->data.cur.x = (v) )
#define zxsPatternSetY(p,v) ( (p)->data.cur.y = (v) )
#define _zxsPatternSetPos(p,x,y) {\
  zxsPatternSetX( p, x );\
  zxsPatternSetY( p, y ); }
#define zxsPatternSetPos(p,x,y) {\
  zxsPatternRecPos( p );\
  _zxsPatternSetPos( p, x, y ); }
#define zxsPatternAddX(p,v) ( (p)->data.cur.x += (v) )
#define zxsPatternAddY(p,v) ( (p)->data.cur.y += (v) )
#define _zxsPatternAddPos(p,x,y) {\
  zxsPatternAddX( p, x );\
  zxsPatternAddY( p, y ); }
#define zxsPatternAddPos(p,x,y) {\
  zxsPatternRecPos( p );\
  _zxsPatternAddPos( p, x, y ); }

void zxsPatternDraw(zxWindow *win, zxsPattern *pat, Pixmap canvas);

#define zxsPatternListInit(l)    zListInit(l)
#define zxsPatternListDestroy(l) zListDestroy( zxsPattern, l )

zxsPattern *zxsPatternListAddHead(zxsPatternList *list, zxPM *pix, int row, int col, int ix, int iy);
zxsPattern *zxsPatternListAddTail(zxsPatternList *list, zxPM *pix, int row, int col, int ix, int iy);

/* ********************************************************** */
/* CLASS: zxsRegion
 * partitioning for a fast drawing of double buffer
 * ********************************************************** */

typedef struct{
  zxsPattern *pat; /* pattern drawn in the region */
  zxRegion reg;
} zxsRegionData;
zListClass( zxsRegionList, zxsRegion, zxsRegionData );

#define zxsRegionSet(r,x,y,w,h) \
  zxRegionSet(&(r)->reg,x,y,w,h)

#define zxsRegionListInit(l)    zListInit(l)
#define zxsRegionListDestroy(l) zListDestroy( zxsRegion, l )

zxsRegion *zxsRegionListAdd(zxsRegionList *list, zxsPattern *pat, zxRegion *reg);
void zxsRegionListAppear(zxWindow *win, zxsRegionList *list, Pixmap canvas, short ox, short oy);

/* ********************************************************** */
/* CLASS: zxsLayer
 * ********************************************************** */

typedef struct{
  zxPM bg; /* background pixmap */
  zxRegion bg_reg; /* background drawing region on canvas */
  zxPM *pix; /* pattern pixmaps */
  int n_pix; /* number of pattern pixmaps */
  zxsPatternList plist;
  zxsRegionList rlist_draw, rlist_erase;
} zxsLayerData;
zListClass( zxsLayerList, zxsLayer, zxsLayerData );

#define zxsLayerListInit(l)    zListInit(l)
#define zxsLayerListDestroy(l) zListDestroy( zxsLayer, l )

zxsLayer *zxsLayerInit(zxsLayer *layer);
void zxsLayerDestroy(zxsLayer *layer);

bool zxsLayerCreatePixArray(zxsLayer *layer, int num);
#define zxsLayerReadBGPixFile(c,l,n) zxPMReadFile( (c), &(l)->data.bg, n )
int zxsLayerReadPixFile(zxWindow *win, zxsLayer *layer, int id, char filename[]);
int zxsLayerReadPixData(zxWindow *win, zxsLayer *layer, int id, char *data[]);

#define zxsLayerBGSetRegion(l,x,y,w,h) zxRegionSet(&(l)->data.bg_reg,x,y,w,h)

zxsPattern *zxsLayerPatternListAddHead(zxsLayer *layer, int id, int row, int col, int ix, int iy);
zxsPattern *zxsLayerPatternListAddTail(zxsLayer *layer, int id, int row, int col, int ix, int iy);

zxRegion *zxsLayerClipRegion(zxsLayer *layer, zxRegion *reg);

void zxsLayerUpdate(zxsLayer *layer);
void zxsLayerDrawBG(zxWindow *win, zxsLayer *layer, Pixmap canvas);
void zxsLayerDrawPattern(zxWindow *win, zxsLayer *layer, Pixmap canvas);
void zxsLayerDraw(zxWindow *win, zxsLayer *layer, Pixmap canvas);
void zxsLayerDrawAll(zxWindow *win, zxsLayer *layer, Pixmap canvas);

/* ********************************************************** */
/* CLASS: zxSprite
 * ********************************************************** */

typedef struct{
  zxRegion reg;
  Pixmap canvas;
  zxsLayerList llist;
} zxSprite;

#define zxSpriteCanvas(s)      (s)->canvas
#define zxSpriteSetCanvas(s,c) ( zxSpriteCanvas(s) = (c) )

zxSprite *zxSpriteCreate(zxWindow *win, zxSprite *spr, int ox, int oy, int width, int height);
void zxSpriteDestroy(zxSprite *spr);
void zxSpriteAddLayer(zxSprite *spr, zxsLayer *layer);

void zxSpriteUpdate(zxSprite *spr);
void zxSpriteDraw(zxWindow *win, zxSprite *spr);
void zxSpriteDrawAll(zxWindow *win, zxSprite *spr);
void zxSpriteAppear(zxWindow *win, zxSprite *spr);
void zxSpriteAppearAll(zxWindow *win, zxSprite *spr);

#define zxWindowDoubleBufferSetSprite(win,spr) zxWindowDoubleBufferSet( win, zxSpriteCanvas(spr) )

/* ********************************************************** */
/* CLASS: zxsMap
 * ********************************************************** */

typedef struct{
  struct _zxs_map_cell{
    ubyte r, c;
  } **array;
  uint width, height; /* whole map size */
  zxPM pix; /* map cell pixmap */
  zxsPattern pat; /* map cell pattern */
  zxRegion reg; /* appearing region */
} zxsMap;

zxsMap *zxsMapCreate(zxsMap *map, uint width, uint height);
void zxsMapDestroy(zxsMap *map);

#define zxsMapSetRegion(m,_x,_y,_w,_h) zxRegionSet( &(m)->reg, _x, _y, _w, _h )

bool zxsMapReadArray(zxsMap *map, char filename[]);
bool zxsMapReadPixFile(zxWindow *win, zxsMap *map, char filename[], int row, int col);
bool zxsMapReadPixData(zxWindow *win, zxsMap *map, char *data[], int row, int col);

void zxsMapDraw(zxWindow *win, zxsMap *map, Pixmap canvas);

__END_DECLS

#endif /* __ZX_SPRITE_H__ */

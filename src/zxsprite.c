/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxsprite - sprite pattern on X
 */

#include <zx11/zxsprite.h>
#include <ctype.h>

/* ********************************************************** */
/* CLASS: zxsPatternList
 * ********************************************************** */

zxsPattern *zxsPatternInit(zxsPattern *pat)
{
  pat->data.pix = NULL;
  pat->data.dx = pat->data.dy = 0;
  pat->data.cur.ox = pat->data.cur.oy = pat->data.cur.x = pat->data.cur.y = 0;
  pat->data.cur_pre = pat->data.cur;
  return pat;
}

void zxsPatternDraw(zxWindow *win, zxsPattern *pat, Pixmap canvas)
{
  zxPMCopy( win, pat->data.pix, canvas,
    pat->data.cur.ox, pat->data.cur.oy, pat->data.dx, pat->data.dy, pat->data.cur.x, pat->data.cur.y );
}

static zxsPattern *_zxsPatternListCreate(zxsPatternList *list, zxPM *pix, int row, int col, int ix, int iy)
{
  zxsPattern *pat;

  if( ( pat = zAlloc( zxsPattern, 1 ) ) == NULL ){
    ZALLOCERROR();
    return NULL;
  }
  zxsPatternSetPixmap( pat, pix );
  zxsPatternDivMatrix( pat, row, col );
  zxsPatternSetPos( pat, ix, iy );
  return pat;
}

zxsPattern *zxsPatternListAddHead(zxsPatternList *list, zxPM *pix, int row, int col, int ix, int iy)
{
  zxsPattern *pat;

  if( ( pat = _zxsPatternListCreate( list, pix, row, col, ix, iy ) ) )
    zListInsertHead( list, pat );
  return pat;
}

zxsPattern *zxsPatternListAddTail(zxsPatternList *list, zxPM *pix, int row, int col, int ix, int iy)
{
  zxsPattern *pat;

  if( ( pat = _zxsPatternListCreate( list, pix, row, col, ix, iy ) ) )
    zListInsertTail( list, pat );
  return pat;
}

/* ********************************************************** */
/* CLASS: zxsRegionList
 * partitioning for a fast drawing of double buffer
 * ********************************************************** */

zxsRegion *zxsRegionListAdd(zxsRegionList *list, zxsPattern *pat, zxRegion *reg)
{
  zxsRegion *rp;

  if( ( rp = zAlloc( zxsRegion, 1 ) ) == NULL ){
    ZALLOCERROR();
    return NULL;
  }
  zListInsertHead( list, rp );
  rp->data.pat = pat;
  rp->data.reg = *reg;
  return rp;
}

void zxsRegionListAppear(zxWindow *win, zxsRegionList *list, Pixmap canvas, short ox, short oy)
{
  zxsRegion *reg = NULL;

  while( !zListIsEmpty(list) ){
    zListDeleteTail( list, &reg );
    zxPixmapDrawDirect( win, canvas,
      reg->data.reg.x, reg->data.reg.y, reg->data.reg.width, reg->data.reg.height,
      reg->data.reg.x + ox, reg->data.reg.y + oy );
    zFree( reg );
  }
}

/* ********************************************************** */
/* CLASS: zxsLayer
 * ********************************************************** */

zxsLayer *zxsLayerInit(zxsLayer *layer)
{
  zxPMInit( &layer->data.bg );
  zxsLayerBGSetRegion( layer, 0, 0, 0, 0 );
  layer->data.pix = NULL;
  layer->data.n_pix = 0;
  zxsPatternListInit( &layer->data.plist );
  zxsRegionListInit( &layer->data.rlist_draw );
  zxsRegionListInit( &layer->data.rlist_erase );
  return layer;
}

void zxsLayerDestroy(zxsLayer *layer)
{
  zxPMDestroy( &layer->data.bg );
  zxsLayerBGSetRegion( layer, 0, 0, 0, 0 );
  zFree( layer->data.pix );
  layer->data.n_pix = 0;
  zxsPatternListDestroy( &layer->data.plist );
  zxsRegionListDestroy( &layer->data.rlist_draw );
  zxsRegionListDestroy( &layer->data.rlist_erase );
}

bool zxsLayerCreatePixArray(zxsLayer *layer, int num)
{
  if( ( layer->data.pix = zAlloc( zxPM, num ) ) == NULL ){
    ZALLOCERROR();
    return false;
  }
  layer->data.n_pix = num;
  return true;
}

#define _zxsLayerAssert(l,i,f,r) \
if( (i) >= (l)->data.n_pix ){\
  ZRUNERROR( "invalid pattern id in the layer(" f ")" );\
  r;\
}

int zxsLayerReadPixFile(zxWindow *win, zxsLayer *layer, int id, char filename[])
{
  _zxsLayerAssert( layer, id, "zxsLayerReadPixFile", return 0 );
  return zxPMReadFile( win, &layer->data.pix[id], filename );
}

int zxsLayerReadPixData(zxWindow *win, zxsLayer *layer, int id, char *data[])
{
  _zxsLayerAssert( layer, id, "zxsLayerReadPixData", return 0 );
  return zxPMReadData( win, &layer->data.pix[id], data );
}

zxsPattern *zxsLayerPatternListAddHead(zxsLayer *layer, int id, int row, int col, int ix, int iy)
{
  _zxsLayerAssert( layer, id, "zxsLayerPatternListAddHead", return NULL );
  return zxsPatternListAddHead( &layer->data.plist, &layer->data.pix[id],
    row, col, ix, iy );
}

zxsPattern *zxsLayerPatternListAddTail(zxsLayer *layer, int id, int row, int col, int ix, int iy)
{
  _zxsLayerAssert( layer, id, "zxsLayerPatternListAddTail", return NULL );
  return zxsPatternListAddTail( &layer->data.plist, &layer->data.pix[id],
    row, col, ix, iy );
}

zxRegion *zxsLayerClipRegion(zxsLayer *layer, zxRegion *reg)
{
  if( reg->x < -reg->width  || reg->x >= layer->data.bg_reg.width ||
      reg->y < -reg->height || reg->y >= layer->data.bg_reg.height )
    return NULL;
  if( reg->x < 0 ){
    reg->width += reg->x; reg->x = 0;
  }
  if( reg->x + reg->width > layer->data.bg_reg.width )
    reg->width = layer->data.bg_reg.width - reg->x;
  if( reg->y < 0 ){
    reg->height += reg->y; reg->y = 0;
  }
  if( reg->y + reg->height > layer->data.bg_reg.height )
    reg->height = layer->data.bg_reg.height - reg->y;
  return reg;
}

void zxsLayerUpdate(zxsLayer *layer)
{
  zxsPattern *pat;
  zxRegion reg;

  for( pat=zListTail(&layer->data.plist);
       pat!=zListRoot(&layer->data.plist);
       pat=zListCellNext(pat) ){
    if( memcmp( &pat->data.cur, &pat->data.cur_pre, sizeof(pat->data.cur) ) ){
      zxRegionSetSize( &reg, pat->data.dx, pat->data.dy );
      zxRegionSetPos( &reg, pat->data.cur.x, pat->data.cur.y );
      if( zxsLayerClipRegion( layer, &reg ) )
        zxsRegionListAdd( &layer->data.rlist_draw, pat, &reg );
      zxRegionSetPos( &reg, pat->data.cur_pre.x, pat->data.cur_pre.y );
      if( zxsLayerClipRegion( layer, &reg ) )
        zxsRegionListAdd( &layer->data.rlist_erase, NULL, &reg );
    }
  }
}

void zxsLayerDrawBG(zxWindow *win, zxsLayer *layer, Pixmap canvas)
{
  zxsRegion *r;

  zListForEach( &layer->data.rlist_draw, r )
    zxPMCopy( win, &layer->data.bg, canvas,
      layer->data.bg_reg.x+r->data.reg.x, layer->data.bg_reg.y+r->data.reg.y,
      r->data.reg.width, r->data.reg.height,
      r->data.reg.x, r->data.reg.y );
  zListForEach( &layer->data.rlist_erase, r )
    zxPMCopy( win, &layer->data.bg, canvas,
      layer->data.bg_reg.x+r->data.reg.x, layer->data.bg_reg.y+r->data.reg.y,
      r->data.reg.width, r->data.reg.height,
      r->data.reg.x, r->data.reg.y );
}

void zxsLayerDrawPattern(zxWindow *win, zxsLayer *layer, Pixmap canvas)
{
  zxsRegion *reg;

  zListForEach( &layer->data.rlist_draw, reg )
    zxsPatternDraw( win, reg->data.pat, canvas );
}

void zxsLayerDraw(zxWindow *win, zxsLayer *layer, Pixmap canvas)
{
  zxsLayerDrawBG( win, layer, canvas );
  zxsLayerDrawPattern( win, layer, canvas );
}

void zxsLayerDrawAll(zxWindow *win, zxsLayer *layer, Pixmap canvas)
{
  zxsPattern *pat;

  zxPMCopy( win, &layer->data.bg, canvas,
    layer->data.bg_reg.x, layer->data.bg_reg.y,
    layer->data.bg_reg.width, layer->data.bg_reg.height,
    0, 0 );
  zListForEach( &layer->data.plist, pat ){
    if( memcmp( &pat->data.cur, &pat->data.cur_pre, sizeof(pat->data.cur) ) )
      zxsPatternDraw( win, pat, canvas );
  }
}

/* ********************************************************** */
/* CLASS: zxSprite
 * ********************************************************** */

zxSprite *zxSpriteCreate(zxWindow *win, zxSprite *spr, int ox, int oy, int width, int height)
{
  zxRegionSet( &spr->reg, ox, oy, width, height );
  zxSpriteSetCanvas( spr, zxPixmapCreate( win, width, height ) );
  zxsLayerListInit( &spr->llist );
  return spr;
}

void zxSpriteDestroy(zxSprite *spr)
{
  zxRegionSet( &spr->reg, 0, 0, 0, 0 );
  zxPixmapDestroy( zxSpriteCanvas( spr ) );
  zxsLayerListDestroy( &spr->llist );
}

void zxSpriteAddLayer(zxSprite *spr, zxsLayer *layer)
{
  zListInsertHead( &spr->llist, layer );
}

void zxSpriteUpdate(zxSprite *spr)
{
  zxsLayer *lp;

  zListForEach( &spr->llist, lp )
    zxsLayerUpdate( lp );
}

void zxSpriteDraw(zxWindow *win, zxSprite *spr)
{
  zxsLayer *lp;

  zListForEach( &spr->llist, lp )
    zxsLayerDraw( win, lp, zxSpriteCanvas( spr ) );
}

void zxSpriteDrawAll(zxWindow *win, zxSprite *spr)
{
  zxsLayer *lp;

  zListForEach( &spr->llist, lp )
    zxsLayerDrawAll( win, lp, zxSpriteCanvas( spr ) );
}

void zxSpriteAppear(zxWindow *win, zxSprite *spr)
{
  zxsLayer *lp;

  zxClipUnsetMask( win );
  zListForEach( &spr->llist, lp ){
    zxsRegionListAppear( win, &lp->data.rlist_draw, zxSpriteCanvas( spr ),
      spr->reg.x, spr->reg.y );
    zxsRegionListAppear( win, &lp->data.rlist_erase, zxSpriteCanvas( spr ),
      spr->reg.x, spr->reg.y );
  }
}

void zxSpriteAppearAll(zxWindow *win, zxSprite *spr)
{
  zxClipUnsetMask( win );
  zxPixmapDrawDirect( win, zxSpriteCanvas( spr ),
    0, 0, spr->reg.width, spr->reg.height, spr->reg.x, spr->reg.y );
}

/* ********************************************************** */
/* CLASS: zxsMap
 * ********************************************************** */

zxsMap *zxsMapCreate(zxsMap *map, uint width, uint height)
{
  int i;

  if( !( map->array = zAlloc( struct _zxs_map_cell*, height ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  for( i=0; i<height; i++ )
    if( !( map->array[i] = zAlloc( struct _zxs_map_cell, width ) ) ){
      ZALLOCERROR();
      return NULL;
    }
  map->width = width;
  map->height = height;
  return map;
}

void zxsMapDestroy(zxsMap *map)
{
  int i;

  for( i=0; i<map->height; i++ )
    zFree( map->array[i] );
  zFree( map->array );
  map->width = map->height = 0;
}

static ubyte _zxsMapReadInt(FILE *fp)
{
  char ch;

  while( !isdigit( ch = fgetc( fp ) ) );
  return ch - '0';
}

bool zxsMapReadArray(zxsMap *map, char filename[])
{
  FILE *fp;
  int i, j;

  if( !( fp = fopen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  if( fscanf( fp, "%d %d", &map->width, &map->height ) != 2 );
  for( i=0; i<map->height; i++ )
    for( j=0; j<map->width; j++ ){
      map->array[i][j].r = _zxsMapReadInt( fp );
      map->array[i][j].c = _zxsMapReadInt( fp );
    }
  fclose( fp );
  return true;
}

bool zxsMapReadPixFile(zxWindow *win, zxsMap *map, char filename[], int row, int col)
{
  if( zxPMReadFile( win, &map->pix, filename ) != XpmSuccess )
    return false;
  zxsPatternSetPixmap( &map->pat, &map->pix );
  zxsPatternDivMatrix( &map->pat, row, col );
  return true;
}

bool zxsMapReadPixData(zxWindow *win, zxsMap *map, char *data[], int row, int col)
{
  if( zxPMReadData( win, &map->pix, data ) != XpmSuccess )
    return false;
  zxsPatternSetPixmap( &map->pat, &map->pix );
  zxsPatternDivMatrix( &map->pat, row, col );
  return true;
}

void zxsMapDraw(zxWindow *win, zxsMap *map, Pixmap canvas)
{
  int i, j, x, y;
  struct _zxs_map_cell *cp;

  for( i=0, y=0; i<map->reg.height; i++, y+=map->pat.data.dy )
    for( j=0, x=0; j<map->reg.width; j++, x+=map->pat.data.dx ){
      cp = &map->array[map->reg.y+i][map->reg.x+j];
      zxsPatternSet( &map->pat, cp->r, cp->c );
      _zxsPatternSetPos( &map->pat, x, y );
      zxsPatternDraw( win, &map->pat, canvas );
    }
}

/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxwidget - original widget set
 */

#include <zx11/zxwidget.h>

/* ********************************************************** */
/* default color set
 * ********************************************************** */

zxWidgetTheme *zxw_theme, zxw_default_theme;

void zxWidgetInit(zxWindow *win)
{
  zxw_default_theme.color_front    = zxGetColor( win, "lightgray" );
  zxw_default_theme.color_back     = zxGetColor( win, "lightgray" );
  zxw_default_theme.color_back_rev = zxGetColor( win, "midnightblue" );
  zxw_default_theme.color_hilit    = zxGetColor( win, "white" );
  zxw_default_theme.color_shade    = zxGetColor( win, "darkgray" );
  zxw_default_theme.color_edit_e   = zxGetColor( win, "white" );
  zxw_default_theme.color_edit_d   = zxGetColor( win, "lightgray" );
  zxw_default_theme.color_text_e   = zxGetColor( win, "black" );
  zxw_default_theme.color_text_d   = zxGetColor( win, "darkgray" );
  zxw_default_theme.color_text_rev = zxGetColor( win, "white" );
  zxw_default_theme.color_gauge_e  = zxGetColor( win, "white" );
  zxw_default_theme.color_gauge_d  = zxGetColor( win, "lightgray" );
  zxw_default_theme.color_gauge_g  = zxGetColor( win, "black" );
  zxw_default_theme.color_bar      = zxGetColor( win, "darkgray" );
  zxw_default_theme.color_knob     = zxGetColor( win, "lightgray" );
  zxw_default_theme.color_radio    = zxGetColor( win, "black" );
  zxWidgetSetDefaultTheme();
  zxWindowSetBGColor( win, zxw_theme->color_back );

  zxWindowSetFont( win, ZXW_DEFAULT_FONT );
}

/* ********************************************************** */
/* shade/hilight bar
 * ********************************************************** */

void zxwSepHoriz(zxWindow *win, short x, short y, short width)
{
  zxWindowSetColor( win, zxw_theme->color_shade );
  zxDrawLine( win, x, y, x+width-1, y );
  y++;
  zxWindowSetColor( win, zxw_theme->color_hilit );
  zxDrawLine( win, x, y, x+width-1, y );
}

void zxwSepVert(zxWindow *win, short x, short y, short height)
{
  zxWindowSetColor( win, zxw_theme->color_shade );
  zxDrawLine( win, x, y, x, y+height-1 );
  zxWindowSetColor( win, zxw_theme->color_hilit );
  x++;
  zxDrawLine( win, x, y, x, y+height-1 );
}

void zxwSepBoxLower(zxWindow *win, short x, short y, short width, short height)
{
  short x2, y2;

  x2 = x + width;
  y2 = y + height;
  zxWindowSetColor( win, zxw_theme->color_shade );
  zxDrawLine( win, x,  y,  x2, y );
  zxDrawLine( win, x,  y,  x, y2 );
  zxWindowSetColor( win, zxw_theme->color_hilit );
  zxDrawLine( win, x+1,  y2,  x2, y2 );
  zxDrawLine( win, x2,  y,  x2,  y2-1 );
}

void zxwSepBoxRaise(zxWindow *win, short x, short y, short width, short height)
{
  short x2, y2;

  x2 = x + width;
  y2 = y + height;
  zxWindowSetColor( win, zxw_theme->color_hilit );
  zxDrawLine( win, x,  y,  x2, y );
  zxDrawLine( win, x,  y,  x, y2 );
  zxWindowSetColor( win, zxw_theme->color_shade );
  zxDrawLine( win, x+1,  y2,  x2, y2 );
  zxDrawLine( win, x2,  y,  x2,  y2-1 );
}

void zxwSepBoxRelief(zxWindow *win, short x, short y, short width, short height)
{
  zxwSepBoxLower( win, x,   y,   width,   height );
  zxwSepBoxRaise( win, x+1, y+1, width-2, height-2 );
}

/* ********************************************************** */
/* box widget
 * ********************************************************** */

static void _zxwBoxDrawUpperLimb(zxWindow *win, short *x1, short *y1, short *x2, short *y2, long color, int h)
{
  int i;

  zxWindowSetColor( win, color );
  for( i=0; i<h; i++ ){
    zxDrawLine( win, *x1, *y1, *x2, *y1 );
    (*y1)++;
    zxDrawLine( win, *x1, *y1, *x1, *y2 );
    (*x1)++; (*x2)--; (*y2)--;
  }
}

static void _zxwBoxDrawLowerLimb(zxWindow *win, short *x1, short *y1, short *x2, short *y2, long color, int h)
{
  int i;

  zxWindowSetColor( win, color );
  for( i=0; i<h; i++ ){
    (*x2)++;
    zxDrawLine( win, *x2, *y2, *x2, *y1 );
    (*y2)++;
    zxDrawLine( win, *x1, *y2, *x2, *y2 );
    (*x1)--; (*y1)--;
  }
}

void zxwBoxDrawShade(zxWindow *win, zxRegion *reg, long color, long limb1, long limb2, short h)
{
  short x1, y1, x2, y2;

  x2 = ( x1 = reg->x ) + reg->width - 1;
  y2 = ( y1 = reg->y ) + reg->height - 1;

  zxSetLineWidth( win, 0 );
  _zxwBoxDrawUpperLimb( win, &x1, &y1, &x2, &y2, limb1, h );

  zxWindowSetColor( win, color );
  zxDrawFillRect( win, x1, y1, reg->width-h*2, reg->height-h*2 );

  _zxwBoxDrawLowerLimb( win, &x1, &y1, &x2, &y2, limb2, h );
}

void zxwBoxDrawRelief(zxWindow *win, zxRegion *reg, long color, short h)
{
  short x1, y1, x2, y2;

  x2 = ( x1 = reg->x ) + reg->width - 1;
  y2 = ( y1 = reg->y ) + reg->height - 1;

  zxSetLineWidth( win, 0 );
  _zxwBoxDrawUpperLimb( win, &x1, &y1, &x2, &y2, zxw_theme->color_shade, h );
  _zxwBoxDrawUpperLimb( win, &x1, &y1, &x2, &y2, zxw_theme->color_hilit, h );

  zxWindowSetColor( win, color );
  zxDrawFillRect( win, x1, y1, reg->width-h*4, reg->height-h*4 );

  _zxwBoxDrawLowerLimb( win, &x1, &y1, &x2, &y2, zxw_theme->color_shade, h );
  _zxwBoxDrawLowerLimb( win, &x1, &y1, &x2, &y2, zxw_theme->color_hilit, h );
}

/* ********************************************************** */
/* label class
 * ********************************************************** */

int zxwLabelHeight(short *asc, short *dsc)
{
  zxRegion reg;
  short _asc, _dsc;
#define ZXWEDITBOX_TEST_STR "Ty"

  if( !asc ) asc = &_asc;
  if( !dsc ) dsc = &_dsc;
  zxTextHeight( ZXWEDITBOX_TEST_STR, asc, dsc );
  zxTextArea( ZXWEDITBOX_TEST_STR, 0, 0, &reg );
  return reg.height + *dsc + ZXW_TEXTBOX_TEXTBORDER * 2;
}

static void _zxwLabelAlign(char *label, int n, short *x, short *y, zxRegion *reg, char halign, char valign, short emboss)
{
  zxRegion txtreg;

  zxTextNArea( label, n, 0, 0, &txtreg );
  switch( halign ){
  case ZXW_ALIGN_LEFT:
    *x = reg->x+emboss+ZXW_TEXTBOX_TEXTBORDER; break;
  case ZXW_ALIGN_CENTER:
    *x = reg->x+(reg->width-txtreg.width)/2; break;
  case ZXW_ALIGN_RIGHT:
    *x = reg->x+reg->width-emboss-ZXW_TEXTBOX_TEXTBORDER-txtreg.width; break;
  default:
    *x = 0;
  }
  switch( valign ){
  case ZXW_ALIGN_TOP:
    *y = reg->y+emboss+ZXW_TEXTBOX_TEXTBORDER+txtreg.height; break;
  case ZXW_ALIGN_MIDDLE:
    *y = reg->y+(reg->height+txtreg.height)/2; break;
  case ZXW_ALIGN_BOTTOM:
    *y = reg->y+reg->height-emboss-ZXW_TEXTBOX_TEXTBORDER; break;
  default:
    *y = 0;
  }
}

void _zxwLabelNDrawColor(zxWindow *win, char *label, int n, zxRegion *reg, char halign, char valign, char color[])
{
  short x, y;

  if( !label ) return;
  _zxwLabelAlign( label, n, &x, &y, reg, halign, valign, 0 );
  zxWindowSetColorByName( win, color );
  zxDrawNString( win, x, y, label, n );
}

/* ********************************************************** */
/* CLASS: zxTextBox
 * text box widget
 * ********************************************************** */

static void _zxwTextBoxAlignFlow(zxWindow *win, char *label, short w, char falign, int *first, int *last)
{
  *first = 0;
  *last = strlen(label) - 1;
  switch( falign ){
  case ZXW_ALIGN_FLOWLEFT:
    for( ; *first<*last && zxTextNWidth(label,*last-*first+1)>=w; (*first)++ );
    break;
  case ZXW_ALIGN_FLOWRIGHT:
    for( ; *first<*last && zxTextNWidth(label,*last-*first+1)>=w; (*last)-- );
    break;
  case ZXW_ALIGN_FLOWBOTH:
    for( ; *first<*last && zxTextNWidth(label,*last-*first+1)>=w; (*first)++, (*last)-- );
  default: ;
  }
}

void _zxwTextBoxDrawLabel(zxWindow *win, char *label, zxRegion *reg, char halign, char valign, char falign, short emboss)
{
  short x, y;
  int _first, _last;

  if( !label ) return;
  _zxwTextBoxAlignFlow( win, label, reg->width-ZXW_TEXTBOX_TEXTBORDER-emboss, falign, &_first, &_last );
  _zxwLabelAlign( label+_first, _last-_first+1, &x, &y, reg, halign, valign, emboss );
  zxDrawNString( win, x, y, label+_first, _last-_first+1 );
}

/* ********************************************************** */
/* CLASS: zxwEditBox
 * text edit box widget
 * ********************************************************** */

void zxwEditBoxSetHeight(zxwEditBox *eb, short height)
{
  short asc; /* dummy */

  /* when 'height' == 0, it is automatically set. */
  eb->reg.height = height > 0 ? height : zxwLabelHeight( &asc, &eb->emboss );
}

static int _zxwEditBoxStrLen(zxwEditBox *eb)
{
  return eb->emboss + ZXW_TEXTBOX_TEXTBORDER - 1
    + zxTextNWidth( eb->label + eb->cursor_str_head, eb->cursor_str_tail - eb->cursor_str_head + 2 );
}

void zxwEditBoxDraw(zxWindow *win, zxwEditBox *eb)
{
  short x, y;
  int len;

  /* locate head and tail */
  if( eb->cursor_str_cur < eb->cursor_str_head )
    eb->cursor_str_head = eb->cursor_str_cur;
  len = strlen( eb->label );
  for( eb->cursor_str_tail=eb->cursor_str_head-1; eb->cursor_str_tail<len-1; eb->cursor_str_tail++ )
    if( _zxwEditBoxStrLen(eb) >= eb->reg.width ) break;
  if( eb->cursor_str_cur - 1 > eb->cursor_str_tail ){
    eb->cursor_str_tail = eb->cursor_str_cur - 1;
    for( eb->cursor_str_head=eb->cursor_str_tail;
         eb->cursor_str_head>0 && _zxwEditBoxStrLen(eb)<eb->reg.width;
         eb->cursor_str_head-- );
  }
  /* draw */
  zxwEmbossDrawLower( win, eb, eb->enable ? zxw_theme->color_edit_e : zxw_theme->color_edit_d );
  if( eb->enable ){
    zxWindowSetColor( win, zxw_theme->color_text_e );
    if( zxwCursorIsVisible( eb ) ){
      x = eb->reg.x + eb->emboss + ZXW_TEXTBOX_TEXTBORDER - 1
        + zxTextNWidth( eb->label + eb->cursor_str_head, eb->cursor_str_cur - eb->cursor_str_head );
      y = eb->reg.y + eb->emboss + ZXW_TEXTBOX_TEXTBORDER;
      zxDrawLine( win, x, y, x, y + eb->reg.height - 2 * ( eb->emboss + ZXW_TEXTBOX_TEXTBORDER ) );
    }
  } else{
    zxWindowSetColor( win, zxw_theme->color_text_d );
  }
  if( !eb->label ) return;
  x = eb->reg.x + eb->emboss + ZXW_TEXTBOX_TEXTBORDER;
  y = eb->reg.y + eb->reg.height - eb->emboss - ZXW_TEXTBOX_TEXTBORDER;
  zxDrawNString( win, x, y, eb->label+eb->cursor_str_head, eb->cursor_str_tail-eb->cursor_str_head+1 );
}

int zxwEditBoxKeyPress(zxWindow *win, zxwEditBox *eb)
{
  switch( zxKeySymbol() ){
  case XK_BackSpace:         if( zStrBSChar( eb->label, eb->cursor_str_cur ) ) eb->cursor_str_cur--; break;
  case XK_Delete:            zStrDelChar( eb->label, eb->cursor_str_cur );                           break;
  case XK_Home: case XK_Up:  eb->cursor_str_cur = 0;                                                 break;
  case XK_Down: case XK_End: eb->cursor_str_cur = strlen( eb->label );                               break;
  case XK_Left:              if( eb->cursor_str_cur > 0 ) eb->cursor_str_cur--;                      break;
  case XK_Right:             if( (size_t)eb->cursor_str_cur < strlen(eb->label) ) eb->cursor_str_cur++;      break;
  case XK_Insert:            zxwCursorInsToggle( eb );                                               break;
  default: ;
    char str[2];

    if( XLookupString( &zxevent.xkey, str, 2, NULL, NULL ) == 0 ) return 0;
    if( str[0] == '\r' ) return -1;
    if( isprint( str[0] ) ){
      if( eb->cursor_ins_mode == ZXW_CURSOR_MODE_INSERT ?
        zStrInsChar( eb->label, eb->label_size, eb->cursor_str_cur, str[0] ) :
        zStrOvrChar( eb->label, eb->label_size, eb->cursor_str_cur, str[0] ) )
        eb->cursor_str_cur++;
    }
  }
  zxwEditBoxDraw( win, eb );
  return 0;
}

/* ********************************************************** */
/* CLASS: zxwGauge
 * gauge widget
 * ********************************************************** */

void zxwGaugeSetRate(zxwGauge *gauge, double rate)
{
  double x;

  x = (double)gauge->guide.x + rate * (double)gauge->x_range;
  zxwKnobSetX( gauge, (short)x );
}

void zxwGaugeSetValue(zxwGauge *gauge, double val)
{
  if( val > gauge->max ) val = gauge->max;
  else if( val < gauge->min ) val = gauge->min;
  zxwGaugeSetRate( gauge, ( val - gauge->min ) / ( gauge->max - gauge->min ) );
}

void zxwGaugeMove(zxwGauge *gauge, int x, int y)
{
  int dx, dy;

  dx = x - gauge->reg.x;
  dy = y - gauge->reg.y;
  gauge->reg.x = x;
  gauge->reg.y = y;
  gauge->knob.x += dx;
  gauge->knob.y += dy;
  gauge->guide.x += dx;
  gauge->guide.y += dy;
}

/* ********************************************************** */
/* CLASS: zxwScrollBar
 * scroll bar widget
 * ********************************************************** */

#include <zx11/pixmaps/zxpm_arrow_up.xpm>
#include <zx11/pixmaps/zxpm_arrow_down.xpm>
#include <zx11/pixmaps/zxpm_arrow_left.xpm>
#include <zx11/pixmaps/zxpm_arrow_right.xpm>

void zxwScrollBarCreateHoriz(zxWindow *win, zxwScrollBar *sb, short x, short y, short length)
{
  zxwScrollBarResizeHoriz( sb, x, y, length );
  zxwUngrab( sb );
  zxwEmbossSetDefault( sb );

  zxwPixButtonReadData( win, &sb->inc, x, y,
    ZXW_SCROLLBAR_W, ZXW_SCROLLBAR_W, __zxpm_arrow_left, NULL );
  zxwPixButtonReadData( win, &sb->dec, x+length-ZXW_SCROLLBAR_W, y,
    ZXW_SCROLLBAR_W, ZXW_SCROLLBAR_W, __zxpm_arrow_right, NULL );
}

void zxwScrollBarCreateVert(zxWindow *win, zxwScrollBar *sb, short x, short y, short length)
{
  zxwScrollBarResizeVert( sb, x, y, length );
  zxwUngrab( sb );
  zxwEmbossSetDefault( sb );

  zxwPixButtonReadData( win, &sb->inc, x, y,
    ZXW_SCROLLBAR_W, ZXW_SCROLLBAR_W, __zxpm_arrow_up, NULL );
  zxwPixButtonReadData( win, &sb->dec, x, y+length-ZXW_SCROLLBAR_W,
    ZXW_SCROLLBAR_W, ZXW_SCROLLBAR_W, __zxpm_arrow_down, NULL );
}

bool zxwScrollBarPress(zxWindow *win, zxwScrollBar *sb, int x, int y)
{
  if( zxRegionIsIn( &sb->inc.reg, x, y ) ){
    zxwPress( &sb->inc );
    zxwPixButtonDrawLower( win, &sb->inc );
    return true;
  }
  if( zxRegionIsIn( &sb->dec.reg, x, y ) ){
    zxwPress( &sb->dec );
    zxwPixButtonDrawLower( win, &sb->dec );
    return true;
  }
  return false;
}

bool zxwScrollBarReleaseX(zxWindow *win, zxwScrollBar *sb)
{
  if( sb->inc.pressed ){
    zxwRelease( &sb->inc );
    zxwPixButtonDrawRaise( win, &sb->inc );
    if( zxwScrollBarDX(sb) > 0 ) zxwKnobX(sb)--;
    return true;
  }
  if( sb->dec.pressed ){
    zxwRelease( &sb->dec );
    zxwPixButtonDrawRaise( win, &sb->dec );
    if( zxwKnobX(sb) < zxwScrollBarXMax(sb) ) zxwKnobX(sb)++;
    return true;
  }
  return false;
}

bool zxwScrollBarReleaseY(zxWindow *win, zxwScrollBar *sb)
{
  if( sb->inc.pressed ){
    zxwRelease( &sb->inc );
    zxwPixButtonDrawRaise( win, &sb->inc );
    if( zxwScrollBarDY(sb) > 0 ) zxwKnobY(sb)--;
    return true;
  }
  if( sb->dec.pressed ){
    zxwRelease( &sb->dec );
    zxwPixButtonDrawRaise( win, &sb->dec );
    if( zxwKnobY(sb) < zxwScrollBarYMax(sb) ) zxwKnobY(sb)++;
    return true;
  }
  return false;
}

/* ********************************************************** */
/* CLASS: zxwScrollRegion
 * scrollable region widget
 * ********************************************************** */

bool zxwScrollRegionCreate(zxWindow *win, zxwScrollRegion *sr, short x, short y, short width, short height, int fullwidth, int fullheight, void (*draw)(zxWindow*,zxwScrollRegion*,void*))
{
  sr->hbar = zAlloc( zxwScrollBar, 1 );
  sr->vbar = zAlloc( zxwScrollBar, 1 );
  if( !sr->hbar || !sr->vbar ){
    zxwScrollRegionDestroy( sr );
    return false;
  }
  zxRegionSet( &sr->fullreg, 0, 0, fullwidth, fullheight );
  zxwBoxSetRegion( sr, x, y, width-ZXW_SCROLLBAR_W, height-ZXW_SCROLLBAR_W );
  zxwScrollBarCreateHoriz( win, sr->hbar, x, y+sr->reg.height, sr->reg.width );
  zxwScrollBarSetKnobHoriz( sr->hbar, width, fullwidth );
  zxwScrollBarCreateVert( win, sr->vbar, x+sr->reg.width, y, sr->reg.height );
  zxwScrollBarSetKnobVert( sr->vbar, height, fullheight );
  sr->draw = draw;
  return true;
}

bool zxwScrollRegionCreateHoriz(zxWindow *win, zxwScrollRegion *sr, short x, short y, short width, short height, int fullwidth, void (*draw)(zxWindow*,zxwScrollRegion*,void*))
{
  if( !( sr->hbar = zAlloc( zxwScrollBar, 1 ) ) ) return false;
  sr->vbar = NULL;
  zxRegionSet( &sr->fullreg, 0, 0, fullwidth, height );
  zxwBoxSetRegion( sr, x, y, width, height-ZXW_SCROLLBAR_W );
  zxwScrollBarCreateHoriz( win, sr->hbar, x, y+sr->reg.height, sr->reg.width );
  zxwScrollBarSetKnobHoriz( sr->hbar, width, fullwidth );
  sr->draw = draw;
  return true;
}

bool zxwScrollRegionCreateVert(zxWindow *win, zxwScrollRegion *sr, short x, short y, short width, short height, int fullheight, void (*draw)(zxWindow*,zxwScrollRegion*,void*))
{
  sr->hbar = NULL;
  if( !( sr->vbar = zAlloc( zxwScrollBar, 1 ) ) ) return false;
  zxRegionSet( &sr->fullreg, 0, 0, width, fullheight );
  zxwBoxSetRegion( sr, x, y, width-ZXW_SCROLLBAR_W, height );
  zxwScrollBarCreateVert( win, sr->vbar, x+sr->reg.width, y, sr->reg.height );
  zxwScrollBarSetKnobVert( sr->vbar, height, fullheight );
  sr->draw = draw;
  return true;
}

/* ********************************************************** */
/* CLASS: zxwPopup
 * pop-up menu widget
 * ********************************************************** */

void zxwPopupAddItem(zxwPopup *popup, char *label, uint hotkey, void *(* callback)(void))
{
  zxRegion reg;
  zxwPopupItem *item;

  if( !( item = zRealloc( popup->item, zxwPopupItem, popup->item_num+1 ) ) ){
    ZALLOCERROR();
    return;
  }
  popup->item = item;
  zxTextArea( label, 0, 0, &reg );
  if( popup->item_num > 0 )
    zxWindowHeight( &popup->w ) += ZXWPOPUPITEM_H;
  if( reg.width > zxWindowWidth( &popup->w ) ){
    zxWindowResize( &popup->w, reg.width, zxWindowHeight(&popup->w) );
    zxwPopupRegSync( popup );
  }

  zxwButtonCreate( zxwItem( popup, popup->item_num ),
    ZXW_EMBOSS_H, ZXWPOPUPITEM_H*popup->item_num+ZXW_EMBOSS_H, 0, ZXWPOPUPITEM_H, label );
  zxwHotkeySet( zxwItem( popup, popup->item_num ), hotkey );
  zxwCallbackSet( zxwItem( popup, popup->item_num ), callback );
  if( zxwLabelIsSep( label ) || !callback )
    zxwDisable( zxwItem( popup, popup->item_num ) );
  zxwItemNumInc( popup );
}

void zxwPopupAddItemList(zxwPopup *popup, zxwPopupItemEntry *entry)
{
  zxwPopupItemEntry *ep;

  for( ep=entry; ep->label; ep++ )
    zxwPopupAddItem( popup, ep->label, ep->hotkey, ep->callback );
}

void zxwPopupDraw(zxwPopup *popup)
{
  int i;
  zxRegion reg;

  zxRegionSet( &reg, 0, 0, popup->w.reg.width, popup->w.reg.height );
  zxwBoxDrawRaise( &popup->w, &reg, zxw_theme->color_back, ZXW_EMBOSS_H );
  for( i=0; i<popup->item_num; i++ ){
    if( zxwPopupItemIsSep( zxwItem( popup, i ) ) )
      zxwSepHoriz( &popup->w, zxwItem(popup,i)->reg.x, zxwItem(popup,i)->reg.y+ZXWPOPUPITEM_H/2, zxwItem(popup,i)->reg.width );
    else
      zxwButtonDrawFlat( &popup->w, zxwItem(popup,i) );
  }
  zxFlush();
}

void zxwPopupOpen(zxwPopup *popup, short x, short y)
{
  int i;

  zxWindowMove( &popup->w, 0, 0 );
  zxWindowOpen( &popup->w );
  for( i=0; i<popup->item_num; i++ )
    zxwItem(popup,i)->reg.width = zxWindowWidth( &popup->w ) - ZXW_EMBOSS_H*2;
  zxWindowMove( &popup->w, x, y );
  zxwPopupRegSync( popup );
  zxWindowRaise( &popup->w );
  zxwPopupDraw( popup );
}

/* ********************************************************** */
/* CLASS: zxwMenu
 * menu bar widget
 * ********************************************************** */

void zxwMenuItemCreate(zxwMenuItem *item, int n, char *label, uint hotkey, zxwPopup *popup)
{
  zxwButtonCreate( item, ZXWMENUITEM_W*n+1, 1, ZXWMENUITEM_W-2, ZXWMENUITEM_H-2, label );
  zxwEmbossSet( item, 1 );
  zxwHotkeySet( item, hotkey );
  item->popup  = popup;
}

void zxwMenuAddItem(zxwMenu *menu, char *label, uint hotkey, zxwPopup *popup)
{
  zxwMenuItem *item;

  if( !( item = zRealloc( menu->item, zxwMenuItem, menu->item_num+1 ) ) ){
    ZALLOCERROR();
    return;
  }
  menu->item = item;
  zxwMenuItemCreate( zxwItem(menu,menu->item_num), menu->item_num, label, hotkey, popup );
  zxwItemNumInc( menu );
}

void zxwMenuAddItemList(zxwMenu *menu, zxwMenuItemEntry *entry)
{
  zxwMenuItemEntry *ep;

  for( ep=entry; ep->label; ep++ )
    zxwMenuAddItem( menu, ep->label, ep->hotkey, ep->popup );
}

void zxwMenuDraw(zxWindow *win, zxwMenu *menu)
{
  int i;

  zxwBoxDraw( win, menu, zxw_theme->color_back );
  for( i=0; i<menu->item_num; i++ ){
    zxwButtonDrawFlat( win, zxwItem(menu,i) );
    if( zxwItem(menu,i)->reg.x > menu->reg.width ) break;
  }
  zxwSepHoriz( win, 0, ZXWMENU_H, menu->reg.width );
}

/* ********************************************************** */
/* CLASS: zxwPixButtonGroup
 * grouped pixmap button widget
 * ********************************************************** */

void zxwPixButtonGroupDestroy(zxwPixButtonGroup *pbg)
{
  int i;

  for( i=0; i<pbg->item_num; i++ )
    zxwPixButtonDestroy( zxwItem(pbg,i) );
  zxwItemDestroy( pbg );
}

int zxwPixButtonGroupAdd(zxwPixButtonGroup *pbg, int row, int col)
{
  zxwPixButton *item;

  if( !( item = zRealloc( pbg->item, zxwPixButton, pbg->item_num+1 ) ) ){
    ZALLOCERROR();
    return -1;
  }
  pbg->item = item;
  if( pbg->reg.width < pbg->btn_w * ( col + 1 ) )
    pbg->reg.width = pbg->btn_w * ( col + 1 );
  if( pbg->reg.height < pbg->btn_h * ( row + 1 ) )
    pbg->reg.height = pbg->btn_h * ( row + 1 );
  zxwItemNumInc( pbg );
  return 0;
}

void zxwPixButtonGroupDraw(zxWindow *win, zxwPixButtonGroup *pbg)
{
  int i;

  zxwSepBoxRelief( win, pbg->reg.x - 1, pbg->reg.y - 1,
    pbg->reg.width  + 2, pbg->reg.height + 2 );
  for( i=0; i<pbg->item_num; i++ )
    if( i == pbg->item_selected )
      zxwPixButtonDrawLower( win, zxwItem(pbg,i) );
    else
      zxwPixButtonDrawRaise( win, zxwItem(pbg,i) );
}

/* ********************************************************** */
/* CLASS: zxwRadioButtonGroup
 * grouped radio button widget (exclusive mode assigned)
 * ********************************************************** */

int zxwRadioButtonGroupAdd(zxwRadioButtonGroup *rbg, int x, int y)
{
  zxwRadioButton *item;

  if( !( item = zRealloc( rbg->item, zxwRadioButton, rbg->item_num+1 ) ) ){
    ZALLOCERROR();
    return -1;
  }
  rbg->item = item;
  zxwRadioButtonCreate( zxwItem(rbg,rbg->item_num), rbg->reg.x+x, rbg->reg.y+y );
  zxwItemNumInc( rbg );
  return 0;
}

void zxwRadioButtonGroupDraw(zxWindow *win, zxwRadioButtonGroup *rbg)
{
  int i;

  zxwBoxDrawRelief( win, &rbg->reg, zxw_theme->color_front, 1 );
  for( i=0; i<rbg->item_num; i++ )
    zxwRadioButtonDraw( win, zxwItem(rbg,i) );
}

/* ********************************************************** */
/* CLASS: zxwTabGroup
 * grouped pixmap button widget
 * ********************************************************** */

int zxwTabGroupAdd(zxwTabGroup *tg, char *label, void *(* f)(void))
{
  zxwTab *item;
  int i, x, xn;

  if( !( item = zRealloc( tg->item, zxwTab, tg->item_num+1 ) ) ){
    ZALLOCERROR();
    return -1;
  }
  tg->item = item;
  zxwItemNumInc( tg );
  zxwButtonCreate( zxwItem(tg,tg->item_num-1), 0, tg->reg.y, 0, ZXW_TAB_H, label );
  for( x=tg->reg.x, i=0; i<tg->item_num; i++ ){
    xn = tg->reg.x + tg->reg.width * ( i + 1 ) / tg->item_num;
    zxwItem(tg,i)->reg.x = x;
    zxwItem(tg,i)->reg.width = xn - x - 1;
    x = xn;
  }
  zxwCallbackSet( zxwItem(tg,tg->item_num-1), f );
  return 0;
}

void zxwTabGroupDestroy(zxwTabGroup *tg)
{
  int i;

  for( i=0; i<tg->item_num; i++ )
    zxwButtonDestroy( zxwItem(tg,i) );
  zxwItemDestroy( tg );
}

void zxwTabGroupDraw(zxWindow *win, zxwTabGroup *tg)
{
  int i;

  for( i=0; i<tg->item_num; i++ ){
    if( i == tg->item_active ) continue;
    zxwTabDrawUnselected( win, zxwItem(tg,i) );
  }
  zxwTabGroupDrawSelected( win, tg );
}

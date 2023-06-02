/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxwidget - original widget set
 */

#ifndef __ZX_WIDGET_H__
#define __ZX_WIDGET_H__

#include <zx11/zxpm.h>
#include <zx11/zxinput.h>

__BEGIN_DECLS

/* ********************************************************** */
/* default color and font set
 * ********************************************************** */

extern long zxw_front_color;
extern long zxw_back_color;
extern long zxw_back_rev_color;
extern long zxw_hilit_color;
extern long zxw_shade_color;
extern long zxw_edit_e_color;
extern long zxw_edit_d_color;
extern long zxw_text_e_color;
extern long zxw_text_d_color;
extern long zxw_text_rev_color;
extern long zxw_gauge_e_color;
extern long zxw_gauge_d_color;
extern long zxw_gauge_g_color;
extern long zxw_bar_color;
extern long zxw_knob_color;
extern long zxw_radio_color;

#define ZXW_DEFAULT_FONT \
  "-misc-fixed-medium-*-normal-*-14-*-*-*-*-*-*-*"

void zxWidgetInit(zxWindow *win);

/* ********************************************************** */
/* shade/hilight bar
 * ********************************************************** */

void zxwSepHoriz(zxWindow *win, short x, short y, short width);
void zxwSepVert(zxWindow *win, short x, short y, short height);
void zxwSepBoxLower(zxWindow *win, short x, short y, short width, short height);
void zxwSepBoxRaise(zxWindow *win, short x, short y, short width, short height);
void zxwSepBoxRelief(zxWindow *win, short x, short y, short width, short height);
#define zxwSepRegion(win,r) zxSepBoxRelief( (win), (r)->x, (r)->y, (r)->width, (r)->height )

/* ********************************************************** */
/* enable/disable
 * ********************************************************** */

#define ZXW_ENABLE bool enable

#define zxwEnable(w)  ( (w)->enable = true )
#define zxwDisable(w) ( (w)->enable = false )

/* ********************************************************** */
/* box widget
 * ********************************************************** */

#define ZXW_BOX_CLASS zxRegion reg

#define zxwBoxSetPos(b,x,y)        zxRegionSetPos( &(b)->reg, x, y )
#define zxwBoxSetSize(b,w,h)       zxRegionSetSize( &(b)->reg, w, h )
#define zxwBoxSetRegion(b,x,y,w,h) zxRegionSet( &(b)->reg, x, y, w, h )
#define zxwBoxDraw(win,b,c) do{\
  zxWindowSetColor( (win), (c) );\
  zxDrawRegionFillRect( (win), &(b)->reg );\
} while(0)

void zxwBoxDrawShade(zxWindow *win, zxRegion *reg, long color, long limb1, long limb2, short h);
#define zxwBoxDrawRaise(win,r,c,h) \
  zxwBoxDrawShade( (win), r, (c), zxw_hilit_color, zxw_shade_color, h )
#define zxwBoxDrawLower(win,r,c,h) \
  zxwBoxDrawShade( (win), r, (c), zxw_shade_color, zxw_hilit_color, h )
void zxwBoxDrawRelief(zxWindow *win, zxRegion *reg, long color, short h);

/* ********************************************************** */
/* emboss class
 * ********************************************************** */

#define ZXW_EMBOSS_CLASS short emboss
#define ZXW_EMBOSS_H  2

#define zxwEmbossSet(w,h)      ( (w)->emboss = (h) )
#define zxwEmbossSetDefault(w) zxwEmbossSet( w, ZXW_EMBOSS_H )

#define zxwEmbossDrawRaise(win,e,c) \
  zxwBoxDrawShade( (win), &(e)->reg, (c), zxw_hilit_color, zxw_shade_color, (e)->emboss )
#define zxwEmbossDrawLower(win,e,c) \
  zxwBoxDrawShade( (win), &(e)->reg, (c), zxw_shade_color, zxw_hilit_color, (e)->emboss )

/* ********************************************************** */
/* alignment class
 * ********************************************************** */

/* virtical alignment */
enum{
  ZXW_ALIGN_TOP,
  ZXW_ALIGN_MIDDLE,
  ZXW_ALIGN_BOTTOM,
};
/* horizontal alignment */
enum{
  ZXW_ALIGN_LEFT,
  ZXW_ALIGN_CENTER,
  ZXW_ALIGN_RIGHT,
};
/* flown part alignment */
enum{
  ZXW_ALIGN_FLOWLEFT,
  ZXW_ALIGN_FLOWRIGHT,
  ZXW_ALIGN_FLOWBOTH,
};

#define ZXW_ALIGN_INVALID (-1)

#define ZXW_ALIGN char align_v, align_h, align_flow

#define zxwAlignV(w)      (w)->align_v
#define zxwAlignH(w)      (w)->align_h
#define zxwAlignFlow(w)   (w)->align_flow

#define zxwAlignTop(w)       ( zxwAlignV(w) = ZXW_ALIGN_TOP )
#define zxwAlignMiddle(w)    ( zxwAlignV(w) = ZXW_ALIGN_MIDDLE )
#define zxwAlignBottom(w)    ( zxwAlignV(w) = ZXW_ALIGN_BOTTOM )
#define zxwAlignLeft(w)      ( zxwAlignH(w) = ZXW_ALIGN_LEFT )
#define zxwAlignCenter(w)    ( zxwAlignH(w) = ZXW_ALIGN_CENTER )
#define zxwAlignRight(w)     ( zxwAlignH(w) = ZXW_ALIGN_RIGHT )
#define zxwAlignFlowLeft(w)  ( zxwAlignFlow(w) = ZXW_ALIGN_FLOWLEFT )
#define zxwAlignFlowRight(w) ( zxwAlignFlow(w) = ZXW_ALIGN_FLOWRIGHT )
#define zxwAlignFlowBoth(w)  ( zxwAlignFlow(w) = ZXW_ALIGN_FLOWBOTH )

#define zxwAlignSet(w,v,h,f) do{\
  zxwAlignV(w) = (v);\
  zxwAlignH(w) = (h);\
  zxwAlignFlow(w) = (f);\
} while(0)

/* ********************************************************** */
/* label class
 * ********************************************************** */

#define ZXW_LABEL_CLASS size_t label_size; char *label

typedef struct{
  ZXW_BOX_CLASS;
  ZXW_LABEL_CLASS;
  ZXW_ALIGN;
} zxwLabel;

#define zxwLabelInit(l)    do{\
  (l)->label_size = 0;\
  (l)->label = NULL;\
} while(0)
#define zxwLabelDestroy(l) do{\
  zFree( (l)->label );\
  (l)->label_size = 0;\
} while(0)

#define zxwLabelAlloc(l,s) do{\
  if( !( (l)->label = zAlloc( char, s ) ) )\
    ZALLOCERROR();\
  (l)->label_size = (s);\
} while(0)

#define zxwLabelSet(l,s) do{\
  zxwLabelDestroy( l );\
  if( !( (l)->label = zStrClone( (char *)(s) ) ) )\
    ZALLOCERROR();\
  (l)->label_size = strlen( (l)->label );\
} while(0)

int zxwLabelHeight(short *asc, short *dsc);

void _zxwLabelNDrawColor(zxWindow *win, char *label, int n, zxRegion *reg, char halign, char valign, char color[]);
#define zxwLabelNDrawColor(win,l,n,color) \
  _zxwLabelNDrawColor( win, (l)->label, n, &(l)->reg, zxwAlignH(l), zxwAlignV(l), color )
#define zxwLabelNDraw(win,l,n) zxwLabelNDrawColor( win, l, n, (char *)"black" )

#define zxwLabelDrawColor(win,l,color) zxwLabelNDrawColor( win, l, (l)->label_size, color )
#define zxwLabelDraw(win,l) zxwLabelNDraw( win, l, (l)->label_size )

/* ********************************************************** */
/* pixmap class
 * ********************************************************** */

#define ZXW_PM_CLASS \
  zxPM pic; short pic_src_x, pic_src_y, pic_dst_x, pic_dst_y

#define zxwPixmapAlign(p,x,y,w,h) do{\
  if( ( (p)->pic_src_x = ( (p)->pic.width  - (w) ) / 2 ) >= 0 ){\
    (p)->pic_dst_x = (x);                  (p)->pic.width = (w);\
  } else{\
    (p)->pic_dst_x = (x) - (p)->pic_src_x; (p)->pic_src_x = 0;\
  }\
  if( ( (p)->pic_src_y = ( (p)->pic.height - (h) ) / 2 ) >= 0 ){\
    (p)->pic_dst_y = (y);                  (p)->pic.height = (h);\
  } else{\
    (p)->pic_dst_y = (y) - (p)->pic_src_y; (p)->pic_src_y = 0;\
  }\
} while(0)

/* ********************************************************** */
/* for button class
 * ********************************************************** */

#define ZXW_PRESS bool pressed

#define zxwPress(w)   ( (w)->pressed = true )
#define zxwRelease(w) ( (w)->pressed = false )

/* ********************************************************** */
/* toggle class
 * ********************************************************** */

#define ZXW_TOGGLE bool toggle

#define zxwToggleOn(w)   ( (w)->toggle = true )
#define zxwToggleOff(w)  ( (w)->toggle = false )

#define zxwToggleIsOn(w) (w)->toggle

/* ********************************************************** */
/* grab class
 * ********************************************************** */

#define ZXW_GRAB bool grabbed

#define zxwGrab(w)       ( (w)->grabbed = true )
#define zxwUngrab(w)     ( (w)->grabbed = false )

#define zxwIsGrabbed(w)  (w)->grabbed

/* ********************************************************** */
/* knob class
 * ********************************************************** */

#define ZXW_KNOB_CLASS ZXW_GRAB; zxRegion knob

#define zxwKnob(t)            ( &(t)->knob )

#define zxwKnobX(t)           zxwKnob(t)->x
#define zxwKnobY(t)           zxwKnob(t)->y
#define zxwKnobWidth(t)       zxwKnob(t)->width
#define zxwKnobHeight(t)      zxwKnob(t)->height

#define zxwKnobSet(t,x,y,w,h) zxRegionSet( zxwKnob(t), x, y, w, h )
#define zxwKnobSetX(t,x)      ( zxwKnobX(t) = (x) )
#define zxwKnobSetY(t,y)      ( zxwKnobY(t) = (y) )
#define zxwKnobSetWidth(t,w)  ( zxwKnobWidth(t) = (w) )
#define zxwKnobSetHeight(t,h) ( zxwKnobHeight(t) = (h) )

#define zxwKnobTryGrab(t,x,y) do{\
  if( zxRegionIsIn( zxwKnob(t), x, y ) ) zxwGrab( t );\
} while(0)

/* ********************************************************** */
/* hotkey
 * ********************************************************** */

#define ZXW_HOTKEY uint hotkey

#define zxwHotkeySet(w,k) ( (w)->hotkey = (k) )

/* ********************************************************** */
/* callback
 * ********************************************************** */

#define ZXW_CALLBACK void *(* callback)(void)

#define zxwCallbackSet(w,f) ( (w)->callback = (f) )
#define zxwCallback(w) if( (w)->callback ) (w)->callback()

/* ********************************************************** */
/* CLASS: zxTextBox
 * text box widget
 * ********************************************************** */

#define ZXW_TEXTBOX_CLASS \
  ZXW_BOX_CLASS;\
  ZXW_EMBOSS_CLASS;\
  ZXW_LABEL_CLASS;\
  ZXW_ALIGN;\
  ZXW_ENABLE

typedef struct{
  ZXW_TEXTBOX_CLASS;
} zxwTextBox;

#define ZXW_TEXTBOX_TEXTBORDER 4

#define zxwTextBoxCreate(t,x,y,w,h) do{\
  zxwBoxSetRegion( t, x, y, w, h );\
  zxwEmbossSetDefault( t );\
  zxwLabelInit( t );\
  zxwEnable( t );\
} while(0)
#define zxwTextBoxDestroy(t) zxwLabelDestroy( t )

#define zxwTextBoxSetLabel(t,l) zxwLabelSet(t,l)

void _zxwTextBoxDrawLabel(zxWindow *win, char *label, zxRegion *reg, char halign, char valign, char falign, short emboss);
#define zxwTextBoxDrawLabel(win,t) \
 _zxwTextBoxDrawLabel( (win), (t)->label, &(t)->reg, zxwAlignH(t), zxwAlignV(t), zxwAlignFlow(t), (t)->emboss )

/* ********************************************************** */
/* cursor class (for edit box)
 * ********************************************************** */

#define ZXW_CURSOR_CLASS \
  int cursor_str_head; /* head position of string to draw */\
  int cursor_str_tail; /* tail position of string to draw */\
  int cursor_str_cur;  /* current position of string */\
  int cursor_ins_mode; /* insert/override mode */\
  bool cursor_visible; /* visible/invisible flag */\

enum{
  ZXW_CURSOR_MODE_INSERT = 0,
  ZXW_CURSOR_OVERRIDE = 1,
};

#define zxwCursorInsToggle(c)   ( (c)->cursor_ins_mode = 1 - (c)->cursor_ins_mode )

#define zxwCursorVisible(c)     ( (c)->cursor_visible = true )
#define zxwCursorInvisible(c)   ( (c)->cursor_visible = false )
#define zxwCursorIsVisible(c)   ( (c)->cursor_visible )
#define zxwCursorIsInvisible(c) ( !(c)->cursor_visible )

#define zxwCursorInit(c) do{\
  (c)->cursor_str_head = (c)->cursor_str_cur = 0;\
  (c)->cursor_str_tail = -1;\
  (c)->cursor_ins_mode = ZXW_CURSOR_MODE_INSERT;\
  zxwCursorInvisible( c );\
} while(0)

/* ********************************************************** */
/* CLASS: zxwEditBox
 * text edit box widget
 * ********************************************************** */

typedef struct{
  ZXW_TEXTBOX_CLASS;
  ZXW_CURSOR_CLASS;
} zxwEditBox;

void zxwEditBoxSetHeight(zxwEditBox *eb, short height);

#define zxwEditBoxFit(win,b) do{\
  if( (b)->label )\
    (b)->reg.width = zxTextWidth((b)->label) + ZXW_TEXTBOX_TEXTBORDER + (b)->emboss*2;\
} while(0)

#define zxwEditBoxCreate(e,s,x,y,w,h) do{\
  zxwTextBoxCreate( e, x, y, w, h );\
  if( (s) > 0 ) zxwLabelAlloc( e, s );\
  zxwEditBoxSetHeight( e, h );\
  zxwAlignSet( e, ZXW_ALIGN_BOTTOM, ZXW_ALIGN_LEFT, ZXW_ALIGN_FLOWLEFT );\
  zxwCursorInit( e );\
} while(0)

#define zxwEditBoxDestroy(e) do{\
  zxwTextBoxDestroy( e );\
} while(0)

#define zxwEditBoxSetString(e,s) zxwTextBoxSetLabel(e,s)

void zxwEditBoxDraw(zxWindow *win, zxwEditBox *eb);
int zxwEditBoxKeyPress(zxWindow *win, zxwEditBox *eb);

/* ********************************************************** */
/* CLASS: zxwButton
 * button widget
 * ********************************************************** */

#define ZXW_BUTTON_CLASS \
  ZXW_TEXTBOX_CLASS;\
  ZXW_PRESS;\
  ZXW_TOGGLE

typedef struct{
  ZXW_BUTTON_CLASS;
} zxwButton;

#define zxwButtonSetLabel(b,l) zxwTextBoxSetLabel( b, l )

#define zxwButtonCreate(b,x,y,w,h,l) do{\
  zxwTextBoxCreate( b, x, y, w, h );\
  zxwAlignSet( b, ZXW_ALIGN_MIDDLE, ZXW_ALIGN_CENTER, ZXW_ALIGN_FLOWBOTH );\
  zxwRelease( b );\
  zxwToggleOff( b );\
  if( l ) zxwButtonSetLabel( b, l );\
} while(0)
#define zxwButtonDestroy(b) zxwTextBoxDestroy( b )

#define _zxwButtonDrawLabel(win,b) zxwTextBoxDrawLabel( win, b )

#define zxwButtonDrawLabel(win,b) do{\
  zxWindowSetColor( win, (b)->enable ? zxw_text_e_color:zxw_text_d_color );\
  _zxwButtonDrawLabel( win, b );\
} while(0)
#define zxwButtonDrawLabelRev(win,b) do{\
  zxWindowSetColor( win, zxw_text_rev_color );\
  _zxwButtonDrawLabel( win, b );\
} while(0)

#define zxwButtonDrawFlat(win,b) do{\
  zxwBoxDraw( win, b, zxw_back_color );\
  zxwButtonDrawLabel( win, b );\
} while(0)
#define zxwButtonDrawRev(win,b) do{\
  zxwBoxDraw( win, b, zxw_back_rev_color );\
  zxwButtonDrawLabelRev( win, b );\
} while(0)
#define zxwButtonDrawLower(win,b) do{\
  zxwEmbossDrawLower( win, b, zxw_front_color );\
  zxwButtonDrawLabel( win, b );\
} while(0)
#define zxwButtonDrawRaise(win,b) do{\
  zxwEmbossDrawRaise( win, b, zxw_front_color );\
  zxwButtonDrawLabel( win, b );\
} while(0)

/* ********************************************************** */
/* CLASS: zxwPixButton
 * pixmap button widget
 * ********************************************************** */

typedef struct{
  ZXW_BUTTON_CLASS;
  ZXW_CALLBACK;
  ZXW_PM_CLASS;
} zxwPixButton;

#define zxwPixButtonCreate(p,x,y,w,h,f) do{\
  zxwButtonCreate( p, x, y, w, h, NULL );\
  zxwCallbackSet( p, f );\
  zxwPixmapAlign( p, x, y, w, h );\
} while(0)

#define zxwPixButtonReadFile(win,p,x,y,w,h,n,f) do{\
  zxPMReadFile( (win), &(p)->pic, (n) );\
  zxwPixButtonCreate( p, x, y, w, h, f );\
} while(0)

#define zxwPixButtonReadData(win,p,x,y,w,h,d,f) do{\
  zxPMReadData( (win), &(p)->pic, (char **)(d) );\
  zxwPixButtonCreate( p, x, y, w, h, f );\
} while(0)

#define zxwPixButtonDestroy(p) zxPMDestroy( &(p)->pic )

#define zxwPixButtonDrawPic(win,p) \
  zxPMDraw( win, &(p)->pic, (p)->pic_src_x, (p)->pic_src_y, (p)->pic.width, (p)->pic.height, (p)->pic_dst_x, (p)->pic_dst_y )

#define zxwPixButtonDrawLower(win,p) do{\
  zxwButtonDrawLower( win, p );\
  zxwPixButtonDrawPic( win, p );\
} while(0)

#define zxwPixButtonDrawRaise(win,p) do{\
  zxwButtonDrawRaise( win, p );\
  zxwPixButtonDrawPic( win, p );\
} while(0)

/* ********************************************************** */
/* CLASS: zxwRadioButton
 * radio button widget
 * ********************************************************** */

#define ZXW_RADIOBUTTON_CLASS \
  ZXW_BOX_CLASS;\
  ZXW_ENABLE;\
  ZXW_PRESS

typedef struct{
  ZXW_RADIOBUTTON_CLASS;
} zxwRadioButton;

#define ZXW_RADIOBUTTON_R 8

#define zxwRadioButtonCreate(b,x,y) do{\
  zxwBoxSetRegion( b, x, y, ZXW_RADIOBUTTON_R*2, ZXW_RADIOBUTTON_R*2 );\
  zxwEnable( b );\
  zxwRelease( b );\
} while(0)

#define zxwRadioButtonDraw(win,b) do{\
  int __x, __y;\
  __x = (b)->reg.x + ZXW_RADIOBUTTON_R;\
  __y = (b)->reg.y + ZXW_RADIOBUTTON_R;\
  zxwBoxDraw( win, b, zxw_front_color );\
  zxWindowSetColor( win, zxw_shade_color );\
  zxDrawFillArc( win, (b)->reg.x, (b)->reg.y, (b)->reg.width, (b)->reg.height, 45, 180 ); \
  zxWindowSetColor( win, zxw_hilit_color );\
  zxDrawFillArc( win, (b)->reg.x, (b)->reg.y, (b)->reg.width, (b)->reg.height, 225, 180 ); \
  zxWindowSetColor( win, (b)->enable ? zxw_hilit_color:zxw_back_color );\
  zxDrawFillCircle( win, __x, __y, ZXW_RADIOBUTTON_R-2 );\
  if( (b)->pressed ){\
    zxWindowSetColor( win, (b)->enable ? zxw_radio_color:zxw_shade_color );\
    zxDrawFillCircle( win, __x, __y, ZXW_RADIOBUTTON_R-4 );\
  }\
} while(0)

/* ********************************************************** */
/* CLASS: zxwCheckBox
 * check box widget
 * ********************************************************** */

#define ZXW_CHECKBOX_CLASS ZXW_RADIOBUTTON_CLASS

typedef struct{
  ZXW_CHECKBOX_CLASS;
} zxwCheckBox;

#define ZXW_CHECKBOX_W 16

#define zxwCheckBoxCreate(b,x,y) do{\
  zxwBoxSetRegion( b, x, y, ZXW_CHECKBOX_W, ZXW_CHECKBOX_W );\
  zxwEnable( b );\
  zxwRelease( b );\
} while(0)

#define zxwCheckBoxDraw(win,b) do{\
  zxwBoxDrawLower(win,&(b)->reg,(b)->enable ? zxw_hilit_color:zxw_back_color,2);\
  if( (b)->pressed ){\
    zxWindowSetColor( win, (b)->enable ? zxw_radio_color:zxw_shade_color );\
    zxDrawLine(win,(b)->reg.x+3,(b)->reg.y+8,(b)->reg.x+6,(b)->reg.y+11);\
    zxDrawLine(win,(b)->reg.x+3,(b)->reg.y+9,(b)->reg.x+6,(b)->reg.y+13);\
    zxDrawLine(win,(b)->reg.x+6,(b)->reg.y+11,(b)->reg.x+13,(b)->reg.y+4);\
    zxDrawLine(win,(b)->reg.x+6,(b)->reg.y+12,(b)->reg.x+13,(b)->reg.y+4);\
    zxDrawLine(win,(b)->reg.x+6,(b)->reg.y+13,(b)->reg.x+13,(b)->reg.y+4);\
  }\
} while(0)

/* ********************************************************** */
/* CLASS: zxwProgressBar
 * progress bar widget
 * ********************************************************** */

typedef struct{
  ZXW_BOX_CLASS;
  zxRegion fillreg;
  double deg;
} zxwProgressBar;

#define zxwProgressBarSetDegree(b,d) do{\
  (b)->deg = zLimit( (d), 0, 100 );\
  (b)->fillreg.width = ( (b)->reg.width-2 ) * (b)->deg * 0.01;\
} while(0)

#define zxwProgressBarInit(b,x,y,w) do{\
  zxwBoxSetRegion( b, x, y, w, 16 );\
  zxRegionSet( &(b)->fillreg, x+1, y+1, 0, 14 );\
  zxwProgressBarSetDegree( b, 0 );\
} while(0)

#define zxwProgressBarDraw(win,b) do{\
  zxwBoxDrawLower( win, &(b)->reg, zxw_front_color, 1 );\
  zxWindowSetColor( (win), zxGetColor(win,"darkblue") );\
  zxDrawRegionFillRect( (win), &(b)->fillreg );\
} while(0)

/* ********************************************************** */
/* CLASS: zxwGauge
 * gauge widget
 * ********************************************************** */

typedef struct{
  ZXW_BOX_CLASS;
  ZXW_EMBOSS_CLASS;
  zxRegion guide;
  ZXW_KNOB_CLASS;
  short x_range;
  short div;
  double min, max; /* mapped minimum/maximum value */
  ZXW_ENABLE;
} zxwGauge;

#define ZXWGAUGE_TOP_OFFSET    4
#define ZXWGAUGE_GUIDE_HEIGHT  8
#define ZXWGAUGE_KNOB_WIDTH    8
#define ZXWGAUGE_KNOB_HEIGHT  16

#define ZXWGAUGE_MAX 100

#define zxwGaugeCreate(g,x,y,l,d,mn,mx) do{\
  zxwBoxSetRegion( g, x, y, l, ZXWGAUGE_KNOB_HEIGHT );\
  zxRegionSet( &(g)->guide, x, y+ZXWGAUGE_TOP_OFFSET, l, ZXWGAUGE_GUIDE_HEIGHT );\
  zxwKnobSet( g, x, y, ZXWGAUGE_KNOB_WIDTH, ZXWGAUGE_KNOB_HEIGHT );\
  (g)->x_range = (g)->guide.width - zxwKnobWidth(g);\
  (g)->div = (d);\
  (g)->min = (mn);\
  (g)->max = (mx);\
  zxwEmbossSetDefault( g );\
  zxwUngrab( g );\
  zxwEnable( g );\
} while(0)

#define zxwGaugeDraw(win,g) do{\
  zxwBoxDraw( win, g, zxw_back_color );\
  if( (g)->div != 0 ){ /* draw discrete gauge */\
    register short __zxw_gauge_i, __zxw_gauge_x;\
    zxWindowSetColor( (win), zxw_gauge_g_color );\
    for( __zxw_gauge_i=0; __zxw_gauge_i<=(g)->div; __zxw_gauge_i++ ){\
      __zxw_gauge_x = zxwGaugeXMin(g) + (g)->knob.width/2 + (g)->x_range * __zxw_gauge_i / (g)->div;\
      zxDrawLine( win, __zxw_gauge_x, (g)->reg.y, __zxw_gauge_x, (g)->guide.y - 1 );\
    }\
  }\
  zxwBoxDrawLower( win, &(g)->guide, (g)->enable ? zxw_gauge_e_color : zxw_gauge_d_color, (g)->emboss );\
  zxwBoxDrawRaise( win, zxwKnob(g), zxw_knob_color, (g)->emboss );\
} while(0)

#define zxwGaugeValue(g) \
  ( ( (g)->max - (g)->min ) * ( zxwKnobX(g) - (g)->guide.x ) / (g)->x_range + (g)->min )

void zxwGaugeSetRate(zxwGauge *gauge, double rate);
void zxwGaugeSetValue(zxwGauge *gauge, double val);

void zxwGaugeMove(zxwGauge *gauge, int x, int y);

#define zxwGaugeXMax(g) ( (g)->guide.x+(g)->x_range )
#define zxwGaugeXMin(g) ( (g)->guide.x )

#define zxwGaugeSetX(g,v) do{\
  short __zxw_gauge_x_tmp;\
  __zxw_gauge_x_tmp = zLimit( v, zxwGaugeXMin(g), zxwGaugeXMax(g) );\
  zxwKnobSetX( g, ( (g)->div == 0 ) ? __zxw_gauge_x_tmp : zxwGaugeXMin(g) + ( (g)->div * ( __zxw_gauge_x_tmp - zxwGaugeXMin(g) ) / (g)->x_range ) * (g)->x_range / (g)->div );\
} while(0)

/* ********************************************************** */
/* CLASS: zxwScrollBar
 * scroll bar widget
 * ********************************************************** */

typedef struct{
  zxRegion bar;
  ZXW_KNOB_CLASS;
  ZXW_EMBOSS_CLASS;
  zxwPixButton inc, dec;
  ZXW_ENABLE;
  short grabx, graby;
} zxwScrollBar;

#define ZXW_SCROLLBAR_W     16
#define ZXW_SCROLLKNOB_WMIN  3

#define zxwScrollBarResizeHoriz(s,x,y,l) do{\
  zxRegionSet( &(s)->bar, (x)+ZXW_SCROLLBAR_W, (y), (l)-ZXW_SCROLLBAR_W*2, ZXW_SCROLLBAR_W );\
  zxwKnobSet( s, (s)->bar.x, (s)->bar.y, 0, ZXW_SCROLLBAR_W );\
} while(0)
#define zxwScrollBarResizeVert(s,x,y,l) do{\
  zxRegionSet( &(s)->bar, (x), (y)+ZXW_SCROLLBAR_W, ZXW_SCROLLBAR_W, (l)-ZXW_SCROLLBAR_W*2 );\
  zxwKnobSet( s, (s)->bar.x, (s)->bar.y, ZXW_SCROLLBAR_W, 0 );\
} while(0)

void zxwScrollBarCreateHoriz(zxWindow *win, zxwScrollBar *sb, short x, short y, short length);
void zxwScrollBarCreateVert(zxWindow *win, zxwScrollBar *sb, short x, short y, short length);

#define zxwScrollBarDestroy(s) do{\
  zxwPixButtonDestroy( &(s)->inc );\
  zxwPixButtonDestroy( &(s)->dec );\
} while(0)

#define zxwScrollBarSetKnobHoriz(b,w,wf) \
  zxwKnobSetWidth( b, zMax( ( (b)->bar.width + ZXW_SCROLLBAR_W*2 ) * (w)/(wf), ZXW_SCROLLKNOB_WMIN ) )
#define zxwScrollBarSetKnobVert(b,h,hf) \
  zxwKnobSetHeight( b, zMax( ( (b)->bar.height + ZXW_SCROLLBAR_W*2 ) * (h)/(hf), ZXW_SCROLLKNOB_WMIN ) )

#define zxwScrollBarDrawBar(win,s) \
  zxwBoxDrawLower( win, &(s)->bar, zxw_bar_color, (s)->emboss )
#define zxwScrollBarDrawKnob(win,s) \
  zxwBoxDrawRaise( win, zxwKnob(s), zxw_knob_color, (s)->emboss )

#define zxwScrollBarDraw(win,s) do{\
  zxwScrollBarDrawBar( win, s );\
  zxwScrollBarDrawKnob( win, s );\
  zxwPixButtonDrawRaise( win, &(s)->inc );\
  zxwPixButtonDrawRaise( win, &(s)->dec );\
} while(0)

#define zxwScrollBarDX(s) ( zxwKnobX(s) - (s)->bar.x )
#define zxwScrollBarDY(s) ( zxwKnobY(s) - (s)->bar.y )

#define zxwScrollBarXMax(s) ( (s)->bar.x+(s)->bar.width-(s)->knob.width )
#define zxwScrollBarYMax(s) ( (s)->bar.y+(s)->bar.height-(s)->knob.height )

#define zxwScrollBarSetX(s,v) do{\
  if( (v) > zxwScrollBarXMax(s) ) zxwKnobSetX( s, zxwScrollBarXMax(s) ); else\
  if( (v) < (s)->bar.x ) zxwKnobSetX( s, (s)->bar.x ); else\
  zxwKnobSetX( s, v );\
} while(0)
#define zxwScrollBarSetY(s,v) do{\
  if( (v) > zxwScrollBarYMax(s) ) zxwKnobSetY( s, zxwScrollBarYMax(s) ); else\
  if( (v) < (s)->bar.y ) zxwKnobSetY( s, (s)->bar.y ); else\
  zxwKnobSetY( s, v );\
} while(0)

bool zxwScrollBarPress(zxWindow *win, zxwScrollBar *sb, int x, int y);
bool zxwScrollBarReleaseX(zxWindow *win, zxwScrollBar *sb);
bool zxwScrollBarReleaseY(zxWindow *win, zxwScrollBar *sb);

#define zxwScrollBarTryGrab(s,x,y) do{\
  zxwKnobTryGrab( s, x, y );\
  (s)->grabx = (x);\
  (s)->graby = (y);\
} while(0)

/* ********************************************************** */
/* CLASS: zxwScrollRegion
 * scrollable region widget
 * ********************************************************** */

typedef struct _zxwScrollRegion{
  ZXW_BOX_CLASS;
  zxRegion fullreg;
  zxwScrollBar *vbar, *hbar;
  void (*draw)(zxWindow *win, struct _zxwScrollRegion *sr, void *util);
} zxwScrollRegion;

bool zxwScrollRegionCreate(zxWindow *win, zxwScrollRegion *sr, short x, short y, short width, short height, int fullwidth, int fullheight, void (*draw)(zxWindow*,zxwScrollRegion*,void*));
bool zxwScrollRegionCreateHoriz(zxWindow *win, zxwScrollRegion *sr, short x, short y, short width, short height, int fullwidth, void (*draw)(zxWindow*,zxwScrollRegion*,void*));
bool zxwScrollRegionCreateVert(zxWindow *win, zxwScrollRegion *sr, short x, short y, short width, short height, int fullheight, void (*draw)(zxWindow*,zxwScrollRegion*,void*));

#define zxwScrollRegionDestroy(s) do{\
  if( (s)->hbar ){ zxwScrollBarDestroy( (s)->hbar ); zFree( (s)->hbar ); }\
  if( (s)->vbar ){ zxwScrollBarDestroy( (s)->vbar ); zFree( (s)->vbar ); }\
} while(0)

#define zxwScrollRegionResize(s,w,h,ws,hs) do{\
  zxwBoxSetRegion( s, (s)->reg.x, (s)->reg.y, (w)-ZXW_SCROLLBAR_W, (h)-ZXW_SCROLLBAR_W );\
  if( (s)->vbar ){\
    zxwScrollBarResizeVert( (s)->vbar, (s)->reg.x+(s)->reg.width, (s)->reg.y, (s)->reg.height );\
    zxwScrollBarSetKnobVert( (s)->vbar, (s)->reg.height, hs );\
  }\
  if( (s)->hbar ){\
    zxwScrollBarResizeHoriz( (s)->hbar, (s)->reg.x, (s)->reg.y+(s)->reg.height, (s)->reg.width );\
    zxwScrollBarSetKnobHoriz( (s)->hbar, (s)->reg.width, ws );\
  }\
} while(0)

#define zxwScrollRegionDraw(win,s,util) do{\
  if( (s)->vbar ) zxwScrollBarDraw( win, (s)->vbar );\
  if( (s)->hbar ) zxwScrollBarDraw( win, (s)->hbar );\
  if( (s)->draw )\
    (s)->draw( win, s, util );\
} while(0)

#define zxwScrollRegionDrawHoriz(win,s,util) do{\
  if( (s)->hbar ) zxwScrollBarDraw( win, (s)->hbar );\
  if( (s)->draw )\
    (s)->draw( win, s, util );\
} while(0)

#define zxwScrollRegionDrawVert(win,s,util) do{\
  if( (s)->vbar ) zxwScrollBarDraw( win, (s)->vbar );\
  if( (s)->draw )\
    (s)->draw( win, s, util );\
} while(0)

#define zxwScrollRegionX(s) \
  (int)( (double)zxwScrollBarDX((s)->hbar) * ( (s)->fullreg.width - (s)->reg.width ) / ( (s)->hbar->bar.width - zxwKnobWidth((s)->hbar) ) )
#define zxwScrollRegionY(s) \
  (int)( (double)zxwScrollBarDY((s)->vbar) * ( (s)->fullreg.height - (s)->reg.height ) / ( (s)->vbar->bar.height - zxwKnobHeight((s)->vbar) ) )
#define zxwScrollRegionW(s) ( (s)->reg.width )
#define zxwScrollRegionH(s) ( (s)->reg.height )

#define zxwScrollRegionPress(w,s,x,y) do{\
  if( (s)->hbar ){\
    if( !zxwScrollBarPress( w, (s)->hbar, x, y ) )\
      zxwScrollBarTryGrab( (s)->hbar, x, y );\
  }\
  if( (s)->vbar ){\
    if( !zxwScrollBarPress( w, (s)->vbar, x, y ) )\
      zxwScrollBarTryGrab( (s)->vbar, x, y );\
  }\
} while(0)
#define zxwScrollRegionRelease(w,s,u) do{\
  if( (s)->hbar ){\
    if( zxwScrollBarReleaseX( w, (s)->hbar ) )\
      zxwScrollRegionDrawHoriz( w, s, u );\
    else\
      zxwUngrab( (s)->hbar );\
  }\
  if( (s)->vbar ){\
    if( zxwScrollBarReleaseY( w, (s)->vbar ) )\
      zxwScrollRegionDrawVert( w, s, u );\
    else\
      zxwUngrab( (s)->vbar );\
  }\
} while(0)

#define zxwScrollRegionDrag(w,s,x,y,u) do{\
  if( (s)->hbar && zxwIsGrabbed((s)->hbar) ){\
    zxwScrollBarSetX( (s)->hbar, zxwKnobX((s)->hbar) + (x) - (s)->hbar->grabx );\
    (s)->hbar->grabx = (x);\
    zxwScrollRegionDrawHoriz( w, s, u );\
  }\
  if( (s)->vbar && zxwIsGrabbed((s)->vbar) ){\
    zxwScrollBarSetY( (s)->vbar, zxwKnobY((s)->vbar) + (y) - (s)->vbar->graby );\
    (s)->vbar->graby = y;\
    zxwScrollRegionDrawVert( w, s, u );\
  }\
} while(0)

/* ********************************************************** */
/* item group class
 * ********************************************************** */

#define ZXW_ITEM_CLASS(type) \
  type *item;           /* array of items */\
  int item_num;         /* number of items */\
  int item_active;      /* identifier of active item */\
  int item_prev_active; /* identifier of previously active item */\
  int item_selected     /* identifier of selected item */

#define ZXW_ITEM_NONE (-1)

#define zxwIsActive(w)     ( (w)->item_active != ZXW_ITEM_NONE )
#define zxwPrevIsActive(w) ( (w)->item_prev_active != ZXW_ITEM_NONE )
#define zxwUnactivate(w)   ( (w)->item_active = ZXW_ITEM_NONE )
#define zxwBackupActive(w) ( (w)->item_prev_active = (w)->item_active )

#define zxwSelect(w)       ( (w)->item_selected = (w)->item_active )
#define zxwUnselect(w)     ( (w)->item_selected = ZXW_ITEM_NONE )

#define zxwItemInit(w) do{\
  (w)->item = NULL;\
  (w)->item_num = 0;\
  zxwUnactivate( w );\
  zxwBackupActive( w );\
  zxwUnselect( w );\
} while(0)
#define zxwItemDestroy(w) do{\
  zFree( (w)->item );\
  zxwItemInit( w );\
} while(0)


#define zxwItem(w,i)           ( &(w)->item[(i)] )
#define zxwItemNum(w)          ( (w)->item_num )
#define zxwItemActivate(w,i)   ( (w)->item_active = (i) )
#define zxwItemActive(w)       zxwItem(w,(w)->item_active)
#define zxwItemPrevActive(w)   zxwItem(w,(w)->item_prev_active)
#define zxwItemActivateNext(w) ( (w)->item_active += (w)->item_active < (w)->item_num ? 1 : 0 )
#define zxwItemSelect(w,i)     ( (w)->item_selected = (i) )
#define zxwItemSelected(w)     zxwItem(w,(w)->item_selected)

#define zxwItemIsDisable(w,i) ( (i) != ZXW_ITEM_NONE && !zxwItem(w,i)->enable )
#define zxwItemActiveIsDisable(w) zxwItemIsDisable( w, (w)->item_active )
#define zxwItemActiveIsChanged(w) ( (w)->item_active != (w)->item_prev_active )

#define zxwItemNumInc(w) ( (w)->item_num++ )
#define zxwItemNumDec(w) ( (w)->item_num-- )
#define zxwItemNext(w) do{\
  zxwBackupActive( w );\
  do{\
    if( ++(w)->item_active == (w)->item_num ) zxwUnactivate( w );\
  } while( zxwItemActiveIsDisable( w ) );\
} while(0)
#define zxwItemPrev(w) do{\
  do{\
    zxwBackupActive( w );\
    if( (w)->item_active == ZXW_ITEM_NONE ) (w)->item_active = (w)->item_num - 1;\
    else (w)->item_active--;\
  } while( zxwItemActiveIsDisable( w ) );\
} while(0)

#define zxwItemPoint(w,x,y) do{\
  register int __i;\
  zxwBackupActive(w);\
  zxwUnactivate(w);\
  if( zxRegionIsIn( &(w)->reg, (x), (y) ) )\
    for( __i=0; __i<(w)->item_num; __i++ )\
      if( zxRegionIsIn( &zxwItem(w,__i)->reg, (x), (y) ) )\
        (w)->item_active = __i;\
} while(0)

#define zxwItemMousePoint(win,w) zxwItemPoint(w,zxMouseX,zxMouseY)

#define zxwItemMouseOP(win,w,b,op) do{\
  zxwItemMousePoint( win, w );\
  *(b) = zxwIsActive( w );\
  op;\
} while(0)

/* ********************************************************** */
/* CLASS: zxwPopup
 * pop-up menu widget
 * ********************************************************** */

typedef struct{
  ZXW_BUTTON_CLASS;
  ZXW_HOTKEY;
  ZXW_CALLBACK;
} zxwPopupItem;

typedef struct{
  ZXW_LABEL_CLASS;
  ZXW_HOTKEY;
  ZXW_CALLBACK;
} zxwPopupItemEntry;

#define zxwLabelIsSep(l)     ( (l)[0] == '-' )
#define zxwPopupItemIsSep(i) zxwLabelIsSep( (i)->label )

#define ZXWPOPUPITEM_W 120
#define ZXWPOPUPITEM_H 24

typedef struct{
  zxWindow w;
  ZXW_BOX_CLASS; /* dummy */
  ZXW_ITEM_CLASS( zxwPopupItem );
} zxwPopup;

#define zxwPopupRegSync(p) ( (p)->reg = (p)->w.reg )

#define zxwPopupCreate(p) do{\
  zxWindowCreate( &(p)->w, 0, 0, ZXWPOPUPITEM_W+ZXW_EMBOSS_H*2, ZXWPOPUPITEM_H+ZXW_EMBOSS_H*2 );\
  zxwPopupRegSync( p );\
  zxWindowSetBorderWidth( &(p)->w, 0 );\
  zxWindowOverrideRedirectEnable( &(p)->w );\
  zxWindowSetColor( &(p)->w, zxw_front_color );\
  zxWindowSetBGColor( &(p)->w, zxw_back_color );\
  zxwItemInit( p );\
} while(0)

void zxwPopupAddItem(zxwPopup *popup, char *label, uint hotkey, void *(* callback)(void));
void zxwPopupAddItemList(zxwPopup *popup, zxwPopupItemEntry *entry);
void zxwPopupDraw(zxwPopup *popup);

#define zxwPopupDrawPress(p) \
  if( zxwIsActive(p) && !zxwItemActive(p)->pressed ){\
    zxwButtonDrawLower( &(p)->w, zxwItemActive(p) );\
    zxwPress( zxwItemActive(p) );\
    zxFlush();\
  } while(0)
#define zxwPopupDrawRelease(p) \
  if( zxwIsActive(p) && zxwItemActive(p)->pressed ){\
    zxwButtonDrawFlat( &(p)->w, zxwItemActive(p) );\
    zxwRelease( zxwItemActive(p) );\
    zxFlush();\
    zxwCallback( zxwItemActive(p) );\
  } while(0)
#define zxwPopupDrawMove(p) \
  if( zxwItemActiveIsChanged( p ) ){\
    if( zxwPrevIsActive(p) ){\
      if( zxwItemPrevActive(p)->pressed )\
        zxwRelease( zxwItemPrevActive(p) );\
      zxwButtonDrawFlat( &(p)->w, zxwItemPrevActive(p) );\
    }\
    if( zxwIsActive(p) )\
      zxwButtonDrawRev( &(p)->w, zxwItemActive(p) );\
    zxFlush();\
  } while(0)

#define zxwPopupPress(p,b) zxwItemMouseOP( &(p)->w, p, b, zxwPopupDrawPress( p ) )
#define zxwPopupRelease(p,b) zxwItemMouseOP( &(p)->w, p, b, zxwPopupDrawRelease( p ) )
#define zxwPopupMove(p,b) zxwItemMouseOP( &(p)->w, p, b, zxwPopupDrawMove( p ) )

void zxwPopupOpen(zxwPopup *popup, short x, short y);
#define zxwPopupClose(p) do{\
  zxWindowClose( &(p)->w );\
  zxFlush();\
} while(0)
#define zxwPopupDestroy(p) do{\
  zxwItemDestroy( p );\
  zxWindowDestroy( &(p)->w );\
} while(0)

/* ********************************************************** */
/* CLASS: zxwMenu
 * menu bar widget
 * ********************************************************** */

typedef struct{
  ZXW_BUTTON_CLASS;
  ZXW_HOTKEY;
  zxwPopup *popup;
} zxwMenuItem;

#define ZXWMENUITEM_W 48
#define ZXWMENUITEM_H 24

void zxwMenuItemCreate(zxwMenuItem *item, int n, char *label, uint hotkey, zxwPopup *popup);

typedef struct{
  ZXW_LABEL_CLASS;
  ZXW_HOTKEY;
  zxwPopup *popup;
} zxwMenuItemEntry;

#define ZXWMENU_H  ZXWMENUITEM_H

typedef struct{
  ZXW_BOX_CLASS;
  ZXW_ITEM_CLASS( zxwMenuItem );
} zxwMenu;

#define zxwMenuCreate(win,m) do{\
  zxwBoxSetRegion( m, 0, 0, zxWindowWidth(win), ZXWMENU_H );\
  zxwItemInit( m );\
} while(0)
#define zxwMenuDestroy(m) zxwItemDestroy(m)
void zxwMenuAddItem(zxwMenu *menu, char *label, uint hotkey, zxwPopup *popup);
void zxwMenuAddItemList(zxwMenu *menu, zxwMenuItemEntry *entry);
void zxwMenuDraw(zxWindow *win, zxwMenu *menu);

#define zxwMenuDrawPress(win,m) \
  if( zxwIsActive(m) && !zxwItemActive(m)->pressed ){\
    zxwButtonDrawLower( win, zxwItemActive(m) );\
    zxwPress( zxwItemActive(m) );\
    zxFlush();\
  } while(0)
#define zxwMenuDrawRelease(win,m) \
  if( zxwIsActive(m) && zxwItemActive(m)->pressed ){\
    zxwButtonDrawRaise( win, zxwItemActive(m) );\
    zxwRelease( zxwItemActive(m) );\
    zxFlush();\
  } while(0)
#define zxwMenuDrawMove(win,m) \
  if( zxwItemActiveIsChanged( m ) ){\
    if( zxwPrevIsActive(m) ){\
      if( zxwItemPrevActive(m)->pressed )\
        zxwRelease( zxwItemPrevActive(m) );\
      zxwButtonDrawFlat( win, zxwItemPrevActive(m) );\
    }\
    if( zxwIsActive(m) )\
      zxwButtonDrawRaise( win, zxwItemActive(m) );\
    zxFlush();\
  } while(0)

#define zxwMenuPress(win,m,b)   zxwItemMouseOP( win, m, b, zxwMenuDrawPress(win,m) )
#define zxwMenuRelease(win,m,b) zxwItemMouseOP( win, m, b, zxwMenuDrawRelease(win,m) )
#define zxwMenuMove(win,m,b)    zxwItemMouseOP( win, m, b, zxwMenuDrawMove(win,m) )

/* ********************************************************** */
/* CLASS: zxwGroup
 * groupe mode
 * ********************************************************** */

#define ZXW_GROUP ubyte group_mode

#define ZXW_GROUP_NONE       0x0
#define ZXW_GROUP_TOGGLE     0x1
#define ZXW_GROUP_EXCLUSIVE  0x2

#define zxwGroupResetMode(g)    ( (g)->group_mode  = ZXW_GROUP_NONE )
#define zxwGroupSetToggle(g)    ( (g)->group_mode |= ZXW_GROUP_TOGGLE )
#define zxwGroupSetExclusive(g) ( (g)->group_mode |= ZXW_GROUP_EXCLUSIVE )

#define zxwGroupIsToggle(g)     ( (g)->group_mode & ZXW_GROUP_TOGGLE )
#define zxwGroupIsExclusive(g)  ( (g)->group_mode & ZXW_GROUP_EXCLUSIVE )

/* ********************************************************** */
/* CLASS: zxwPixButtonGroup
 * grouped pixmap button widget
 * ********************************************************** */

typedef struct{
  ZXW_BOX_CLASS;
  ZXW_GROUP;
  short btn_w, btn_h; /* size of each button */
  ZXW_ITEM_CLASS( zxwPixButton );
} zxwPixButtonGroup;

#define zxwPixButtonGroupInit(p,x,y,w,h) do{\
  zxwBoxSetRegion( p, x, y, 0, 0 );\
  (p)->btn_w = w;\
  (p)->btn_h = h;\
  zxwGroupResetMode(p);\
  zxwItemInit( p );\
} while(0)
void zxwPixButtonGroupDestroy(zxwPixButtonGroup *pbg);
int zxwPixButtonGroupAdd(zxwPixButtonGroup *pbg, int row, int col);

#define zxwPixButtonGroupAddFile(win,p,r,c,n,f) do{\
  if( zxwPixButtonGroupAdd( p, r, c ) == 0 )\
    zxwPixButtonReadFile( win, zxwItem(p,(p)->item_num-1), (p)->reg.x+(p)->btn_w*(c), (p)->reg.y+(p)->btn_h*(r), (p)->btn_w, (p)->btn_h, (n), (f) );\
} while(0)
#define zxwPixButtonGroupAddData(win,p,r,c,d,f) do{\
  if( zxwPixButtonGroupAdd( p, r, c ) == 0 )\
    zxwPixButtonReadData( win, zxwItem(p,(p)->item_num-1), (p)->reg.x+(p)->btn_w*(c), (p)->reg.y+(p)->btn_h*(r), (p)->btn_w, (p)->btn_h, (d), (f) );\
} while(0)

void zxwPixButtonGroupDraw(zxWindow *win, zxwPixButtonGroup *pbg);

#define zxwPixButtonGroupDrawPress(win,p) \
  if( zxwIsActive(p) && !zxwItemActive(p)->pressed ){\
    zxwPixButtonDrawLower( win, zxwItemActive(p) );\
    zxwPress( zxwItemActive(p) );\
    zxFlush();\
  }
#define zxwPixButtonGroupDrawRelease(win,p) \
  if( zxwIsActive(p) && zxwItemActive(p)->pressed ){\
    zxwPixButtonDrawRaise( win, zxwItemActive(p) );\
    zxwRelease( zxwItemActive(p) );\
    zxFlush();\
    zxwCallback( zxwItemActive(p) );\
  }
#define zxwPixButtonGroupDrawTogglePress(win,p) \
  if( zxwIsActive(p) ){\
    if( zxwGroupIsExclusive(p) && (p)->item_selected != ZXW_ITEM_NONE &&\
        (p)->item_selected != (p)->item_active ){\
      zxwPixButtonDrawRaise( win, zxwItemSelected(p) );\
      zxwToggleOff( zxwItemSelected(p) );\
      zxwRelease( zxwItemSelected(p) );\
      zxwUnselect( p );\
    }\
    if( !zxwItemActive(p)->pressed ){\
      zxwPixButtonDrawLower( win, zxwItemActive(p) );\
      zxwPress( zxwItemActive(p) );\
      zxFlush();\
    }\
  }
#define zxwPixButtonGroupDrawToggleRelease(win,p) \
  if( zxwIsActive(p) && zxwItemActive(p)->pressed ){\
    if( !zxwToggleIsOn( zxwItemActive(p) ) ){\
      zxwPixButtonDrawLower( win, zxwItemActive(p) );\
      zxwToggleOn( zxwItemActive(p) );\
      zxwSelect( p );\
      zxFlush();\
    } else{\
      zxwPixButtonDrawRaise( win, zxwItemActive(p) );\
      zxwToggleOff( zxwItemActive(p) );\
      zxwUnselect( p );\
      zxFlush();\
    }\
    zxwCallback( zxwItemActive(p) );\
    zxwRelease( zxwItemActive(p) );\
  }
#define zxwPixButtonGroupDrawSelective(win,p) \
  if( zxwIsActive(p) ){\
    if( (p)->item_selected != ZXW_ITEM_NONE ){\
      zxwRelease( zxwItemSelected(p) );\
      zxwPixButtonDrawRaise( win, zxwItemSelected(p) );\
    }\
    zxwSelect( p );\
    zxwPress( zxwItemActive(p) );\
    zxwPixButtonDrawLower( win, zxwItemActive(p) );\
    zxFlush();\
    zxwCallback( zxwItemActive(p) );\
    zxwRelease( zxwItemActive(p) );\
  }
#define zxwPixButtonGroupDrawMove(win,p) \
  if( (p)->item_prev_active != (p)->item_active &&\
      zxwPrevIsActive(p) && zxwItemPrevActive(p)->pressed ){\
    zxwPixButtonDrawRaise( win, zxwItemPrevActive(p) );\
    zxwRelease( zxwItemPrevActive(p) );\
    zxFlush();\
  }

#define zxwPixButtonGroupPress(win,p,b) do{\
  if( zxwGroupIsToggle(p) )\
    zxwItemMouseOP(win,p,b,zxwPixButtonGroupDrawTogglePress(win,p));\
  else\
    zxwItemMouseOP(win,p,b,zxwPixButtonGroupDrawPress(win,p));\
} while(0)
#define zxwPixButtonGroupRelease(win,p,b) do{\
  if( zxwGroupIsToggle(p) )\
    zxwItemMouseOP(win,p,b,zxwPixButtonGroupDrawToggleRelease(win,p));\
  else\
    zxwItemMouseOP(win,p,b,zxwPixButtonGroupDrawRelease(win,p));\
} while(0)
#define zxwPixButtonGroupSelective(win,p,b) \
  zxwItemMouseOP(win,p,b,zxwPixButtonGroupDrawSelective(win,p))
#define zxwPixButtonGroupMove(win,p,b) \
  zxwItemMouseOP(win,p,b,zxwPixButtonGroupDrawMove(win,p))

/* ********************************************************** */
/* CLASS: zxwRadioButtonGroup
 * grouped radio button widget (exclusive mode assigned)
 * ********************************************************** */

typedef struct{
  ZXW_BOX_CLASS;
  ZXW_GROUP;
  ZXW_ITEM_CLASS( zxwRadioButton );
} zxwRadioButtonGroup;

#define zxwRadioButtonGroupInit(r,x,y,w,h) do{\
  zxwBoxSetRegion( r, x, y, w, h );\
  zxwGroupResetMode( r );\
  zxwItemInit( r );\
} while(0)

#define zxwRadioButtonGroupDestroy(r) zxwItemDestroy( r )

int zxwRadioButtonGroupAdd(zxwRadioButtonGroup *rbg, int x, int y);

#define zxwRadioButtonGroupActivate(r,i) do{\
  (r)->item_active = i;\
  zxwSelect( r );\
  if( zxwPrevIsActive(r) )\
    zxwRelease( zxwItemPrevActive(r) );\
  zxwPress( zxwItem(r,i) );\
} while(0)

void zxwRadioButtonGroupDraw(zxWindow *win, zxwRadioButtonGroup *rbg);

#define zxwRadioButtonGroupDrawPress(win,r) do{\
  if( zxwIsActive(r) ){\
    if( !zxwGroupIsToggle(r) && (r)->item_active == (r)->item_selected ){\
      zxwUnselect( r );\
      zxwRelease( zxwItemActive(r) );\
    } else{\
      if( (r)->item_selected != ZXW_ITEM_NONE ){\
        zxwRelease( zxwItemSelected(r) );\
        zxwRadioButtonDraw( win, zxwItemSelected(r) );\
      }\
      zxwSelect( r );\
      zxwPress( zxwItemActive(r) );\
    }\
    zxwRadioButtonDraw( win, zxwItemActive(r) );\
    zxFlush();\
  }\
} while(0)

#define zxwRadioButtonGroupPress(win,r,b) \
  zxwItemMouseOP(win,r,b,zxwRadioButtonGroupDrawPress(win,r))

/* ********************************************************** */
/* CLASS: zxwTabGroup
 * grouped pixmap button widget
 * ********************************************************** */

typedef struct{
  ZXW_BUTTON_CLASS;
  ZXW_CALLBACK;
} zxwTab;

#define ZXW_TAB_DW  2
#define ZXW_TAB_H  16

#define zxwTabDraw(w,t) do{\
  zxWindowSetColor( w, zxw_back_color );\
  zxDrawRegionFillRect( w, &(t)->reg );\
  zxWindowSetColor( w, zxw_hilit_color );\
  zxDrawLine( w, (t)->reg.x, (t)->reg.y+(t)->reg.height-1, (t)->reg.x+ZXW_TAB_DW, (t)->reg.y );\
  zxDrawLine( w, (t)->reg.x+ZXW_TAB_DW, (t)->reg.y, (t)->reg.x+(t)->reg.width-ZXW_TAB_DW, (t)->reg.y );\
  zxWindowSetColor( w, zxw_shade_color );\
  zxDrawLine( w, (t)->reg.x+(t)->reg.width-ZXW_TAB_DW, (t)->reg.y, (t)->reg.x+(t)->reg.width, (t)->reg.y+(t)->reg.height-1 );\
  zxwButtonDrawLabel(w,t);\
} while(0)

#define zxwTabDrawUnselected(w,t) do{\
  zxwDisable( t );\
  zxwTabDraw( w, t );\
} while(0)
#define zxwTabDrawSelected(w,t) do{\
  zxwEnable( t );\
  zxwTabDraw( w, t );\
} while(0)

typedef struct{
  ZXW_BOX_CLASS;
  ZXW_GROUP;
  ZXW_ITEM_CLASS( zxwTab );
} zxwTabGroup;

#define zxwTabGroupInit(t,x,y,w) do{\
  zxwBoxSetRegion( t, x, y, w, ZXW_TAB_H );\
  zxwGroupResetMode(t);\
  zxwItemInit( t );\
  (t)->item_active = 0;\
} while(0)
int zxwTabGroupAdd(zxwTabGroup *tg, char *label, void *(* f)(void));
void zxwTabGroupDestroy(zxwTabGroup *tg);

void zxwTabGroupDraw(zxWindow *win, zxwTabGroup *tg);

#define zxwTabGroupDrawSelected(w,t) do{\
  if( zxwItemActive(t) ){\
    zxwSelect( t );\
    zxwTabDrawSelected( w, zxwItemSelected(t) );\
    zxWindowSetColor( w, zxw_hilit_color );\
    zxDrawLine( w, (t)->reg.x, (t)->reg.y+(t)->reg.height-1, zxwItemSelected(t)->reg.x, (t)->reg.y+(t)->reg.height-1 );\
    zxDrawLine( w, zxwItemSelected(t)->reg.x+zxwItemSelected(t)->reg.width, (t)->reg.y+(t)->reg.height-1, (t)->reg.x+(t)->reg.width, (t)->reg.y+(t)->reg.height-1 );\
  }\
} while(0)

#define zxwTabGroupDrawPress(win,t) \
  if( zxwIsActive(t) ){\
    if( (t)->item_selected != ZXW_ITEM_NONE )\
      zxwTabDrawUnselected( win, zxwItemSelected(t) );\
    zxwSelect( t );\
    zxwPress( zxwItemActive(t) );\
    zxwTabGroupDrawSelected( win, t );\
    zxFlush();\
    zxwCallback( zxwItemActive(t) );\
  }
#define zxwTabGroupPress(win,t,b) \
  zxwItemMouseOP(win,t,b,zxwTabGroupDrawPress(win,t))

__END_DECLS

#endif /* __ZX_WIDGET_H__ */

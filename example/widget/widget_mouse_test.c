#include <unistd.h>
#include <zx11/zxwidget.h>
#include "../pixmaps/mini.x.xpm"
#include "../pixmaps/mini.cut.xpm"
#include "../pixmaps/mini.cross.xpm"
#include "../pixmaps/mini.line.xpm"
#include "../pixmaps/mini.zoom.xpm"

/* tab group */
zxwTabGroup tabgroup;

/* button & pixmap button group */

zxwPixButtonGroup pixbuttongroup[3];

void *x(void){     return NULL; }
void *cut(void){   return NULL; }
void *cross(void){ return NULL; }
void *line(void){  return NULL; }
void *zoom(void){  return NULL; }

void init_pixbutton(zxWindow *win)
{
  /* pixbutton group */
  zxwPixButtonGroupInit( &pixbuttongroup[0], 50, 60, 20, 20 );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[0], 0, 0, mini_x_xpm, x );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[0], 0, 1, cut_xpm, cut );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[0], 0, 2, mini_cross_xpm, cross );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[0], 0, 3, line_xpm, line );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[0], 0, 4, zoom_xpm, zoom );

  zxwPixButtonGroupInit( &pixbuttongroup[1], 50, 90, 20, 20 );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[1], 0, 0, mini_x_xpm, x );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[1], 0, 1, cut_xpm, cut );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[1], 0, 2, mini_cross_xpm, cross );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[1], 0, 3, line_xpm, line );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[1], 0, 4, zoom_xpm, zoom );
  zxwGroupSetToggle( &pixbuttongroup[1] );
  zxwGroupSetExclusive( &pixbuttongroup[1] );

  zxwPixButtonGroupInit( &pixbuttongroup[2], 50, 120, 20, 20 );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[2], 0, 0, mini_x_xpm, x );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[2], 0, 1, cut_xpm, cut );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[2], 0, 2, mini_cross_xpm, cross );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[2], 0, 3, line_xpm, line );
  zxwPixButtonGroupAddData( win, &pixbuttongroup[2], 0, 4, zoom_xpm, zoom );
  zxwItemSelect( &pixbuttongroup[2], 0 );
}

void destroy_pixbutton(void)
{
  zxwPixButtonGroupDestroy( &pixbuttongroup[0] );
  zxwPixButtonGroupDestroy( &pixbuttongroup[1] );
  zxwPixButtonGroupDestroy( &pixbuttongroup[2] );
}

void draw_tab_pixbutton(zxWindow *win)
{
  zxRegion reg;

  zxRegionSet( &reg, 20, 40, 600, 420 );
  zxwBoxDrawRelief( win, &reg, zxw_theme->color_front, 2 );
  zxwPixButtonGroupDraw( win, &pixbuttongroup[0] );
  zxwPixButtonGroupDraw( win, &pixbuttongroup[1] );
  zxwPixButtonGroupDraw( win, &pixbuttongroup[2] );
  zxFlush();
}

void button_press_tab_pixbutton(zxWindow *win)
{
  bool judge;

  zxwPixButtonGroupSelective( win, &pixbuttongroup[2], &judge );
  if( judge ) return;
  zxwPixButtonGroupPress( win, &pixbuttongroup[1], &judge );
  if( judge ) return;
  zxwPixButtonGroupPress( win, &pixbuttongroup[0], &judge );
  if( judge ) return;
}

void button_release_tab_pixbutton(zxWindow *win)
{
  bool judge;

  zxwPixButtonGroupRelease( win, &pixbuttongroup[1], &judge );
  if( judge ) return;
  zxwPixButtonGroupRelease( win, &pixbuttongroup[0], &judge );
  if( judge ) return;
}

void motion_notify_tab_pixbutton(zxWindow *win)
{
  bool judge;

  zxwPixButtonGroupMove( win, &pixbuttongroup[1], &judge );
  if( judge ) return;
  zxwPixButtonGroupMove( win, &pixbuttongroup[0], &judge );
  if( judge ) return;
}

/* radio button group */
zxwRadioButtonGroup radiobuttongroup1;
zxwRadioButtonGroup radiobuttongroup2;
zxwCheckBox checkbox[4];

void add_radiobutton(zxwRadioButtonGroup *radiobuttongroup)
{
  zxwRadioButtonGroupAdd( radiobuttongroup,  16, 16 );
  zxwRadioButtonGroupAdd( radiobuttongroup,  64, 16 );
  zxwRadioButtonGroupAdd( radiobuttongroup, 112, 16 );
  zxwRadioButtonGroupAdd( radiobuttongroup, 160, 16 );
}

void init_radiobutton(zxWindow *win)
{
  /* radio button group */
  zxwRadioButtonGroupInit( &radiobuttongroup1, 50, 60, 200, 48 );
  add_radiobutton( &radiobuttongroup1 );
  zxwRadioButtonGroupActivate( &radiobuttongroup1, 0 );

  zxwRadioButtonGroupInit( &radiobuttongroup2, 50, 120, 200, 48 );
  add_radiobutton( &radiobuttongroup2 );
  zxwRadioButtonGroupActivate( &radiobuttongroup2, 0 );
  zxwGroupSetToggle( &radiobuttongroup2 );
}

void destroy_radiobutton(void)
{
  zxwRadioButtonGroupDestroy( &radiobuttongroup1 );
  zxwRadioButtonGroupDestroy( &radiobuttongroup2 );
}

void draw_tab_radiobutton(zxWindow *win)
{
  zxRegion reg;

  zxRegionSet( &reg, 20, 40, 600, 420 );
  zxwBoxDrawRelief( win, &reg, zxw_theme->color_front, 2 );
  zxwRadioButtonGroupDraw( win, &radiobuttongroup1 );
  zxwRadioButtonGroupDraw( win, &radiobuttongroup2 );
  zxFlush();
}

void button_press_tab_radiobutton(zxWindow *win)
{
  bool judge;

  zxwRadioButtonGroupPress( win, &radiobuttongroup1, &judge );
  zxwRadioButtonGroupPress( win, &radiobuttongroup2, &judge );
}

void button_release_tab_radiobutton(zxWindow *win){}
void motion_notify_tab_radiobutton(zxWindow *win){}

/* gauge */
zxwGauge gauge[2];
zxwEditBox editbox[2];

void init_gauge(zxWindow *win)
{
  /* gauge */
  zxwGaugeCreate( &gauge[0], 40, 60, 200,  0, -2, 2 );
  zxwGaugeCreate( &gauge[1], 40, 90, 200, 10, -2, 2 );

  zxwEditBoxCreate( &editbox[0], 0, 280, 60, 80, 0 );
  zxwAlignRight( &editbox[0] );
  zxwAlignFlowRight( &editbox[0] );

  zxwEditBoxCreate( &editbox[1], 0, 280, 90, 80, 0 );
  zxwAlignRight( &editbox[1] );
  zxwAlignFlowRight( &editbox[1] );
}

void destroy_gauge(void)
{
  zxwEditBoxDestroy( &editbox[0] );
  zxwEditBoxDestroy( &editbox[1] );
}

void draw_gauge(zxWindow *win, zxwGauge *gauge, zxwEditBox *eb)
{
  char buf[BUFSIZ];

  zxwGaugeDraw( win, gauge );
  sprintf( buf, "%f", zxwGaugeValue( gauge ) );
  zxwEditBoxSetString( eb, buf );
  zxwEditBoxDraw( win, eb );
}

void draw_tab_gauge(zxWindow *win)
{
  zxRegion reg;

  zxRegionSet( &reg, 20, 40, 600, 420 );
  zxwBoxDrawRelief( win, &reg, zxw_theme->color_front, 2 );
  draw_gauge( win, &gauge[0], &editbox[0] );
  draw_gauge( win, &gauge[1], &editbox[1] );
  zxFlush();
}

void button_press_tab_gauge(zxWindow *win)
{
  zxwKnobTryGrab( &gauge[0], zxMouseX, zxMouseY );
  zxwKnobTryGrab( &gauge[1], zxMouseX, zxMouseY );
}

void button_release_tab_gauge(zxWindow *win)
{
  zxwUngrab( &gauge[0] );
  zxwUngrab( &gauge[1] );
}

void motion_notify_tab_gauge(zxWindow *win)
{
  if( zxwIsGrabbed( &gauge[0] ) ){
    zxwGaugeSetX( &gauge[0], zxMouseX );
    draw_gauge( win, &gauge[0], &editbox[0] );
  }
  if( zxwIsGrabbed( &gauge[1] ) ){
    zxwGaugeSetX( &gauge[1], zxMouseX );
    draw_gauge( win, &gauge[1], &editbox[1] );
  }
}

/* scroll regions */
zxwScrollRegion sr1, sr2, sr3;

void draw_scrollregion(zxWindow *win, zxwScrollRegion *sr, void *dummy)
{
  char str[BUFSIZ];

  zxWindowSetColor( win, zxw_theme->color_back );
  zxDrawFillRect( win, sr->reg.x+1, sr->reg.y+1, sr->reg.width-2, sr->reg.height-2 );
  zxWindowSetColorByName( win, "blue" );
  zxDrawRegionRect( win, &sr->reg );

  zxWindowSetColor( win, zxw_theme->color_text_e );
  sprintf( str, "(%d,%d): (%d,%d)-(%d,%d)", zxwKnobX(sr->hbar), zxwKnobY(sr->vbar), zxwScrollBarDX(sr->hbar), zxwScrollBarDY(sr->vbar), zxwScrollBarDX(sr->hbar)+zxwKnobWidth(sr->hbar), zxwScrollBarDY(sr->vbar)+zxwKnobHeight(sr->vbar) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+24, str );
  sprintf( str, "(%d,%d)-(%d,%d)", zxwScrollRegionX(sr), zxwScrollRegionY(sr), zxwScrollRegionX(sr)+zxwScrollRegionW(sr), zxwScrollRegionY(sr)+zxwScrollRegionH(sr) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+48, str );
  zxFlush();
}

void draw_scrollregion_horiz(zxWindow *win, zxwScrollRegion *sr, void *dummy)
{
  char str[BUFSIZ];

  zxWindowSetColor( win, zxw_theme->color_back );
  zxDrawFillRect( win, sr->reg.x+1, sr->reg.y+1, sr->reg.width-2, sr->reg.height-2 );
  zxWindowSetColorByName( win, "blue" );
  zxDrawRegionRect( win, &sr->reg );

  zxWindowSetColor( win, zxw_theme->color_text_e );
  sprintf( str, "%d-%d", zxwScrollBarDX(sr->hbar), zxwScrollBarDX(sr->hbar)+zxwKnobWidth(sr->hbar) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+24, str );
  sprintf( str, "%d-%d", zxwScrollRegionX(sr), zxwScrollRegionX(sr)+zxwScrollRegionW(sr) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+48, str );
  zxFlush();
}

void draw_scrollregion_vert(zxWindow *win, zxwScrollRegion *sr, void *dummy)
{
  char str[BUFSIZ];

  zxWindowSetColor( win, zxw_theme->color_back );
  zxDrawFillRect( win, sr->reg.x+1, sr->reg.y+1, sr->reg.width-2, sr->reg.height-2 );
  zxWindowSetColorByName( win, "blue" );
  zxDrawRegionRect( win, &sr->reg );

  zxWindowSetColor( win, zxw_theme->color_text_e );
  sprintf( str, "%d-%d", zxwScrollBarDY(sr->vbar), zxwScrollBarDY(sr->vbar)+zxwKnobHeight(sr->vbar) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+24, str );
  sprintf( str, "%d-%d", zxwScrollRegionY(sr), zxwScrollRegionY(sr)+zxwScrollRegionH(sr) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+48, str );
  zxFlush();
}

void draw_tab_scrollregion(zxWindow *win)
{
  zxRegion reg;

  zxRegionSet( &reg, 20, 40, 600, 420 );
  zxwBoxDrawRelief( win, &reg, zxGetColor( win, "lightyellow" ), 2 );
  zxwScrollRegionDraw( win, &sr1, NULL );
  zxwScrollRegionDraw( win, &sr2, NULL );
  zxwScrollRegionDraw( win, &sr3, NULL );
  zxFlush();
}

void button_press_tab_scrollregion(zxWindow *win)
{
  zxwScrollRegionPress( win, &sr1, zxMouseX, zxMouseY );
  zxwScrollRegionPress( win, &sr2, zxMouseX, zxMouseY );
  zxwScrollRegionPress( win, &sr3, zxMouseX, zxMouseY );
}

void button_release_tab_scrollregion(zxWindow *win)
{
  zxwScrollRegionRelease( win, &sr1, NULL );
  zxwScrollRegionRelease( win, &sr2, NULL );
  zxwScrollRegionRelease( win, &sr3, NULL );
}

void motion_notify_tab_scrollregion(zxWindow *win)
{
  zxwScrollRegionDrag( win, &sr1, zxMouseX, zxMouseY, NULL );
  zxwScrollRegionDrag( win, &sr2, zxMouseX, zxMouseY, NULL );
  zxwScrollRegionDrag( win, &sr3, zxMouseX, zxMouseY, NULL );
}

void init_scrollregion(zxWindow *win)
{
  zxwScrollRegionCreate(      win, &sr1,  50,  50, 240, 240, 1000, 1000, draw_scrollregion );
  zxwScrollRegionCreateHoriz( win, &sr2,  50, 310, 224, 120, 1000, draw_scrollregion_horiz );
  zxwScrollRegionCreateVert(  win, &sr3, 310,  50, 120, 224, 1000, draw_scrollregion_vert );
}

void destroy_scrollregion(void)
{
  zxwScrollRegionDestroy( &sr1 );
  zxwScrollRegionDestroy( &sr2 );
  zxwScrollRegionDestroy( &sr3 );
}

/* tab group */

void (* draw_tab)(zxWindow *win) = draw_tab_pixbutton;
void (* button_press_tab)(zxWindow *win) = button_press_tab_pixbutton;
void (* button_release_tab)(zxWindow *win) = button_release_tab_pixbutton;
void (* motion_notify_tab)(zxWindow *win) = motion_notify_tab_pixbutton;

void *select_tab_pixbutton(void)
{
  draw_tab = draw_tab_pixbutton;
  button_press_tab = button_press_tab_pixbutton;
  button_release_tab = button_release_tab_pixbutton;
  motion_notify_tab = motion_notify_tab_pixbutton;
  return NULL;
}

void *select_tab_radiobutton(void)
{
  draw_tab = draw_tab_radiobutton;
  button_press_tab = button_press_tab_radiobutton;
  button_release_tab = button_release_tab_radiobutton;
  motion_notify_tab = motion_notify_tab_radiobutton;
  return NULL;
}

void *select_tab_gauge(void)
{
  draw_tab = draw_tab_gauge;
  button_press_tab = button_press_tab_gauge;
  button_release_tab = button_release_tab_gauge;
  motion_notify_tab = motion_notify_tab_gauge;
  return NULL;
}

void *select_tab_scrollregion(void)
{
  draw_tab = draw_tab_scrollregion;
  button_press_tab = button_press_tab_scrollregion;
  button_release_tab = button_release_tab_scrollregion;
  motion_notify_tab = motion_notify_tab_scrollregion;
  return NULL;
}

int main(void)
{
  zxWindow win;
  bool judge;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 640, 480 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWidgetInit( &win );

  init_pixbutton( &win );
  init_radiobutton( &win );
  init_gauge( &win );
  init_scrollregion( &win );

  /* tab group */
  zxwTabGroupInit( &tabgroup, 20, 20, 280 );
  zxwTabGroupAdd( &tabgroup, "Button", select_tab_pixbutton );
  zxwTabGroupAdd( &tabgroup, "Radio", select_tab_radiobutton );
  zxwTabGroupAdd( &tabgroup, "Gauge", select_tab_gauge );
  zxwTabGroupAdd( &tabgroup, "Scroll", select_tab_scrollregion );
  zxwTabGroupDraw( &win, &tabgroup );
  draw_tab( &win );
  zxFlush();
  while( 1 ){
    switch( zxGetEvent() ){
    case KeyPress:      if( zxKeySymbol() == XK_q ) goto END; break;
    case ButtonPress:   zxwTabGroupPress( &win, &tabgroup, &judge );
                        if( judge ) draw_tab( &win );
                        button_press_tab( &win );   break;
    case ButtonRelease: button_release_tab( &win ); break;
    case MotionNotify:  motion_notify_tab( &win );  break;
    default: ;
    }
  }

 END:
  zxwTabGroupDestroy( &tabgroup );
  destroy_pixbutton();
  destroy_radiobutton();
  destroy_gauge();
  destroy_scrollregion();
  zxExit();
  return 0;
}

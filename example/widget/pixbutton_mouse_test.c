#include <zx11/zxwidget.h>
#include "../pixmaps/mini.x.xpm"
#include  "../pixmaps/mini.cut.xpm"
#include  "../pixmaps/mini.cross.xpm"
#include  "../pixmaps/mini.line.xpm"
#include  "../pixmaps/mini.zoom.xpm"

zxWindow win;
zxwMenu menu;
zxwPixButtonGroup pbg[3];

void *dummy(void){ return NULL; }

zxwMenuItemEntry entry[] = {
  { 4, "File", XK_f, NULL },
  { 4, "Edit", XK_e, NULL },
  { 4, "Hoge", XK_VoidSymbol, NULL },
  { 4, "Help", XK_h, NULL },
  { 0, NULL, 0, NULL },
};

void *x(void){ ZECHO( "X" ); return NULL; }
void *cut(void){ ZECHO( "cut" ); return NULL; }
void *cross(void){ ZECHO( "cross" ); return NULL; }
void *line(void){ ZECHO( "line" ); return NULL; }
void *zoom(void){ ZECHO( "zoom" ); return NULL; }

void draw(void)
{
  zxwMenuDraw( &win, &menu );
  zxwPixButtonGroupDraw( &win, &pbg[0] );
  zxwPixButtonGroupDraw( &win, &pbg[1] );
  zxwPixButtonGroupDraw( &win, &pbg[2] );
  zxFlush();
}

int main(void)
{
  bool judge;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 300, 160 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWidgetInit( &win );

  zxwMenuCreate( &win, &menu );
  zxwMenuAddItemList( &menu, entry );

  zxwPixButtonGroupInit( &pbg[0], 20, 40, 20, 20 );
  zxwPixButtonGroupAddData( &win, &pbg[0], 0, 0, mini_x_xpm, x );
  zxwPixButtonGroupAddData( &win, &pbg[0], 0, 1, cut_xpm, cut );
  zxwPixButtonGroupAddData( &win, &pbg[0], 0, 2, mini_cross_xpm, cross );
  zxwPixButtonGroupAddData( &win, &pbg[0], 0, 3, line_xpm, line );
  zxwPixButtonGroupAddData( &win, &pbg[0], 0, 4, zoom_xpm, zoom );

  zxwPixButtonGroupInit( &pbg[1], 20, 70, 20, 20 );
  zxwPixButtonGroupAddData( &win, &pbg[1], 0, 0, mini_x_xpm, x );
  zxwPixButtonGroupAddData( &win, &pbg[1], 0, 1, cut_xpm, cut );
  zxwPixButtonGroupAddData( &win, &pbg[1], 0, 2, mini_cross_xpm, cross );
  zxwPixButtonGroupAddData( &win, &pbg[1], 0, 3, line_xpm, line );
  zxwPixButtonGroupAddData( &win, &pbg[1], 0, 4, zoom_xpm, zoom );
  zxwGroupSetToggle( &pbg[1] );
  zxwGroupSetExclusive( &pbg[1] );

  zxwPixButtonGroupInit( &pbg[2], 20, 100, 20, 20 );
  zxwPixButtonGroupAddData( &win, &pbg[2], 0, 0, mini_x_xpm, x );
  zxwPixButtonGroupAddData( &win, &pbg[2], 0, 1, cut_xpm, cut );
  zxwPixButtonGroupAddData( &win, &pbg[2], 0, 2, mini_cross_xpm, cross );
  zxwPixButtonGroupAddData( &win, &pbg[2], 0, 3, line_xpm, line );
  zxwPixButtonGroupAddData( &win, &pbg[2], 0, 4, zoom_xpm, zoom );
  zxwItemSelect( &pbg[2], 0 );

  draw();
  while( 1 ){
    switch( zxGetEvent() ){
    case Expose:
      draw();
      break;
    case KeyPress:
      if( zxKeySymbol() == XK_q ) goto END;
    case ButtonPress:
      zxwMenuPress( &win, &menu, &judge );
      if( judge ) break;
      zxwPixButtonGroupSelective( &win, &pbg[2], &judge );
      if( judge ) break;
      zxwPixButtonGroupPress( &win, &pbg[1], &judge );
      if( judge ) break;
      zxwPixButtonGroupPress( &win, &pbg[0], &judge );
      if( judge ) break;
      break;
    case ButtonRelease:
      zxwMenuRelease( &win, &menu, &judge );
      if( judge ) break;
      zxwPixButtonGroupRelease( &win, &pbg[1], &judge );
      if( judge ) break;
      zxwPixButtonGroupRelease( &win, &pbg[0], &judge );
      if( judge ) break;
      break;
    case MotionNotify:
      zxwMenuMove( &win, &menu, &judge );
      if( judge ) break;
      zxwPixButtonGroupMove( &win, &pbg[1], &judge );
      if( judge ) break;
      zxwPixButtonGroupMove( &win, &pbg[0], &judge );
      if( judge ) break;
      break;
    default: ;
    }
  }

 END:
  zxwPixButtonGroupDestroy( &pbg[0] );
  zxwPixButtonGroupDestroy( &pbg[1] );
  zxwPixButtonGroupDestroy( &pbg[2] );
  zxwMenuDestroy( &menu );
  zxExit();
  return 0;
}

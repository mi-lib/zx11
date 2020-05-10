#include <zx11/zxwidget.h>

static char fontname[] =
"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso8859-1";

void draw1(zxWindow *win, zxwScrollRegion *sr, void *dummy)
{
  char str[BUFSIZ];

  zxWindowSetColor( win, zxw_back_color );
  zxDrawFillRect( win, sr->reg.x+1, sr->reg.y+1, sr->reg.width-2, sr->reg.height-2 );
  zxWindowSetColorByName( win, "blue" );
  zxDrawRegionRect( win, &sr->reg );

  zxWindowSetColor( win, zxw_text_e_color );
  sprintf( str, "(%d,%d): (%d,%d)-(%d,%d)", zxwNobX(sr->hbar), zxwNobY(sr->vbar), zxwScrollBarDX(sr->hbar), zxwScrollBarDY(sr->vbar), zxwScrollBarDX(sr->hbar)+zxwNobWidth(sr->hbar), zxwScrollBarDY(sr->vbar)+zxwNobHeight(sr->vbar) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+24, str );
  sprintf( str, "(%d,%d)-(%d,%d)", zxwScrollRegionX(sr), zxwScrollRegionY(sr), zxwScrollRegionX(sr)+zxwScrollRegionW(sr), zxwScrollRegionY(sr)+zxwScrollRegionH(sr) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+48, str );
  zxFlush();
}

void draw2(zxWindow *win, zxwScrollRegion *sr, void *dummy)
{
  char str[BUFSIZ];

  zxWindowSetColor( win, zxw_back_color );
  zxDrawFillRect( win, sr->reg.x+1, sr->reg.y+1, sr->reg.width-2, sr->reg.height-2 );
  zxWindowSetColorByName( win, "blue" );
  zxDrawRegionRect( win, &sr->reg );

  zxWindowSetColor( win, zxw_text_e_color );
  sprintf( str, "%d-%d", zxwScrollBarDX(sr->hbar), zxwScrollBarDX(sr->hbar)+zxwNobWidth(sr->hbar) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+24, str );
  sprintf( str, "%d-%d", zxwScrollRegionX(sr), zxwScrollRegionX(sr)+zxwScrollRegionW(sr) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+48, str );
  zxFlush();
}

void draw3(zxWindow *win, zxwScrollRegion *sr, void *dummy)
{
  char str[BUFSIZ];

  zxWindowSetColor( win, zxw_back_color );
  zxDrawFillRect( win, sr->reg.x+1, sr->reg.y+1, sr->reg.width-2, sr->reg.height-2 );
  zxWindowSetColorByName( win, "blue" );
  zxDrawRegionRect( win, &sr->reg );

  zxWindowSetColor( win, zxw_text_e_color );
  sprintf( str, "%d-%d", zxwScrollBarDY(sr->vbar), zxwScrollBarDY(sr->vbar)+zxwNobHeight(sr->vbar) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+24, str );
  sprintf( str, "%d-%d", zxwScrollRegionY(sr), zxwScrollRegionY(sr)+zxwScrollRegionH(sr) );
  zxDrawString( win, sr->reg.x+24, sr->reg.y+48, str );
  zxFlush();
}

void draw(zxWindow *win, zxwScrollRegion *sr1, zxwScrollRegion *sr2, zxwScrollRegion *sr3)
{
  zxwScrollRegionDraw( win, sr1, NULL );
  zxwScrollRegionDraw( win, sr2, NULL );
  zxwScrollRegionDraw( win, sr3, NULL );
  zxFlush();
}

int main(void)
{
  zxWindow win;
  zxwScrollRegion sr1, sr2, sr3;
  bool quit_flag = false;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 500, 500 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWidgetInit( &win );
  zxWindowSetFont( &win, fontname );

  zxwScrollRegionCreate( &win, &sr1, 20, 20, 240, 240, 1000, 1000, draw1 );
  zxwScrollRegionCreateHoriz( &win, &sr2, 20, 280, 240, 120, 1000, draw2 );
  zxwScrollRegionCreateVert( &win, &sr3, 280, 20, 120, 240, 1000, draw3 );

  draw( &win, &sr1, &sr2, &sr3 );
  while( !quit_flag ){
    switch( zxGetEvent() ){
    case Expose:
      draw( &win, &sr1, &sr2, &sr3 );
      break;
    case KeyPress:
      if( zxKeySymbol() == XK_q ) quit_flag = true;
      break;
    case ButtonPress:
      zxwScrollRegionPress( &win, &sr1, zxMouseX, zxMouseY );
      zxwScrollRegionPress( &win, &sr2, zxMouseX, zxMouseY );
      zxwScrollRegionPress( &win, &sr3, zxMouseX, zxMouseY );
      break;
    case ButtonRelease:
      zxwScrollRegionRelease( &win, &sr1, NULL );
      zxwScrollRegionRelease( &win, &sr2, NULL );
      zxwScrollRegionRelease( &win, &sr3, NULL );
      break;
    case MotionNotify:
      zxwScrollRegionDrag( &win, &sr1, zxMouseX, zxMouseY, NULL );
      zxwScrollRegionDrag( &win, &sr2, zxMouseX, zxMouseY, NULL );
      zxwScrollRegionDrag( &win, &sr3, zxMouseX, zxMouseY, NULL );
      break;
    default: ;
    }
  }

  zxwScrollRegionDestroy( &sr1 );
  zxwScrollRegionDestroy( &sr2 );
  zxwScrollRegionDestroy( &sr3 );
  zxExit();
  return 0;
}

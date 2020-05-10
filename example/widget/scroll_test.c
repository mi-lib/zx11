#include <unistd.h>
#include <zx11/zxwidget.h>

static char fontname[] =
"-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso8859-1";

void draw(zxWindow *win, zxwScrollRegion *sr, void *dummy)
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

int main(void)
{
  zxWindow win;
  zxwScrollRegion sr;
  int l;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 400, 400 );
  zxWidgetInit( &win );
  zxWindowSetFont( &win, fontname );

  zxwScrollRegionCreate( &win, &sr, 20, 20, 300, 300, 2000, 2000, draw );
  zxwScrollRegionDraw( &win, &sr, NULL );
  getchar();

  l = sr.vbar->bar.y + sr.vbar->bar.height - zxwNobHeight(sr.vbar);
  for( ; zxwNobY(sr.vbar)<l; zxwNobY(sr.vbar)++ ){
    zxwScrollRegionDrawVert( &win, &sr, NULL );
    usleep( 10000 );
  }
  l = sr.hbar->bar.x + sr.hbar->bar.width - zxwNobWidth(sr.hbar);
  for( ; zxwNobX(sr.hbar)<l; zxwNobX(sr.hbar)++ ){
    zxwScrollRegionDrawHoriz( &win, &sr, NULL );
    usleep( 10000 );
  }
  zxwScrollRegionDraw( &win, &sr, NULL );
  getchar();

  zxExit();
  return 0;
}

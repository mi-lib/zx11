#include <unistd.h>
#include <zx11/zxwidget.h>

void separator(zxWindow *win, int x, int y)
{
  zxwSepHoriz(     win, x   , y   , 580 );
  zxwSepVert(      win, x   , y+10, 80 );
  zxwSepBoxRelief( win, x+10, y+10, 580, 80 );
  zxwSepBoxLower(  win, x+20, y+20, 560, 60 );
  zxwSepBoxRaise(  win, x+30, y+30, 540, 40 );
}

typedef struct{
  ZXW_BOX_CLASS;
} box_t;
void box(zxWindow *win, int x, int y)
{
  box_t b1, b2, b3, b4;

  zxwBoxSetRegion( &b1, x+  0, y, 120, 64 );
  zxwBoxSetRegion( &b2, x+136, y, 120, 64 );
  zxwBoxSetRegion( &b3, x+272, y, 120, 64 );
  zxwBoxSetRegion( &b4, x+408, y, 120, 64 );

  zxwBoxDraw( win, &b1, zxGetColor(win,"darkblue") );
  zxwBoxDrawRaise( win, &b2.reg, zxw_theme->color_front, 2 );
  zxwBoxDrawLower( win, &b3.reg, zxw_theme->color_front, 2 );
  zxwBoxDrawRelief( win, &b4.reg, zxw_theme->color_front, 2 );
}

void label(zxWindow *win, int x, int y)
{
  zxwLabel label;
  int w, h;

  w = 96;
  h = zxwLabelHeight( NULL, NULL );

  zxwLabelInit( &label );
  zxwLabelSet( &label, "hello" );
  zxwBoxSetRegion( &label, 0, 0, w, h );

  zxwBoxSetPos( &label, x, y );
  zxwAlignTop( &label ); zxwAlignLeft( &label );
  zxwLabelDrawColor( win, &label, "black" );

  zxwBoxSetPos( &label, x, y+h );
  zxwAlignTop( &label ); zxwAlignCenter( &label );
  zxwLabelDrawColor( win, &label, "red" );

  zxwBoxSetPos( &label, x, y+h*2 );
  zxwAlignTop( &label ); zxwAlignRight( &label );
  zxwLabelDrawColor( win, &label, "blue" );

  zxwBoxSetPos( &label, x+w, y );
  zxwAlignMiddle( &label ); zxwAlignLeft( &label );
  zxwLabelDrawColor( win, &label, "green" );

  zxwBoxSetPos( &label, x+w, y+h );
  zxwAlignMiddle( &label ); zxwAlignCenter( &label );
  zxwLabelDrawColor( win, &label, "yellow" );

  zxwBoxSetPos( &label, x+w, y+h*2 );
  zxwAlignMiddle( &label ); zxwAlignRight( &label );
  zxwLabelDrawColor( win, &label, "white" );

  zxwBoxSetPos( &label, x+w*2, y );
  zxwAlignBottom( &label ); zxwAlignLeft( &label );
  zxwLabelDrawColor( win, &label, "darkgreen" );

  zxwBoxSetPos( &label, x+w*2, y+h );
  zxwAlignBottom( &label ); zxwAlignCenter( &label );
  zxwLabelDrawColor( win, &label, "magenta" );

  zxwBoxSetPos( &label, x+w*2, y+h*2 );
  zxwAlignBottom( &label ); zxwAlignRight( &label );
  zxwLabelDrawColor( win, &label, "cyan" );

  zxwLabelDestroy( &label );
}

void radiobutton(zxWindow *win, int x, int y)
{
  zxwRadioButton b1, b2, b3, b4;

  zxwRadioButtonCreate( &b1, x    , y );
  zxwRadioButtonCreate( &b2, x+ 50, y );
  zxwRadioButtonCreate( &b3, x+100, y );
  zxwRadioButtonCreate( &b4, x+150, y );

  zxwPress( &b1 );
  zxwDisable( &b3 );
  zxwPress( &b4 );
  zxwDisable( &b4 );

  zxwRadioButtonDraw( win, &b1 );
  zxwRadioButtonDraw( win, &b2 );
  zxwRadioButtonDraw( win, &b3 );
  zxwRadioButtonDraw( win, &b4 );
}

void checkbox(zxWindow *win, int x, int y)
{
  zxwCheckBox b1, b2, b3, b4;

  zxwCheckBoxCreate( &b1, x    , y );
  zxwCheckBoxCreate( &b2, x+ 50, y );
  zxwCheckBoxCreate( &b3, x+100, y );
  zxwCheckBoxCreate( &b4, x+150, y );

  zxwPress( &b1 );
  zxwDisable( &b3 );
  zxwPress( &b4 );
  zxwDisable( &b4 );

  zxwCheckBoxDraw( win, &b1 );
  zxwCheckBoxDraw( win, &b2 );
  zxwCheckBoxDraw( win, &b3 );
  zxwCheckBoxDraw( win, &b4 );
}

void button(zxWindow *win, int x, int y)
{
  zxwButton btn[3];

  zxwButtonCreate( &btn[0], x,     y, 80, 24, "hello" );
  zxwButtonCreate( &btn[1], x+100, y, 80, 24, "this button has a too long label to be completely shown." );
  zxwButtonCreate( &btn[2], x+200, y, 80, 24, "good-bye" );
  zxwDisable( &btn[2] );

  zxwButtonDrawRaise( win, &btn[0] );
  zxwButtonDrawRaise( win, &btn[1] );
  zxwButtonDrawRaise( win, &btn[2] );
  zxFlush();
  getchar();
  zxwButtonDrawLower( win, &btn[0] );
  zxFlush();
  getchar();

  zxwDisable( &btn[0] );
  zxwEnable( &btn[2] );
  zxwButtonDrawRaise( win, &btn[0] );
  zxwButtonDrawRaise( win, &btn[2] );
  zxFlush();
  getchar();
  zxwButtonDrawLower( win, &btn[2] );
  zxFlush();
  getchar();

  zxwButtonDestroy( &btn[0] );
  zxwButtonDestroy( &btn[1] );
  zxwButtonDestroy( &btn[2] );
}

void textbox(zxWindow *win, int x, int y)
{
#define STR1 "Hello world!"
#define STR2 "This is a sentense to test drawing a long string."
  zxwEditBox tb;
  int n;

  zxwEditBoxCreate( &tb, BUFSIZ, x, y, 180, 0 );
  strcpy( tb.label, STR1 );

  zxwEditBoxDraw( win, &tb );
  zxFlush();
  getchar();
  zxwDisable( &tb );
  zxwEditBoxDraw( win, &tb );
  zxwEditBoxDestroy( &tb );

  zxwEditBoxCreate( &tb, BUFSIZ, x, y+32, 180, 0 );
  for( n=0; n<=strlen(STR2); n++ ){
    tb.label[n] = STR2[n];
    tb.cursor_str_cur = n;
    zxwEditBoxDraw( win, &tb );
    zxFlush();
    usleep( 50000 );
  }
  getchar();

/*
  zxwEditBoxFit( win, &tb );
  zxwCursorInit( &tb );
  zxwEditBoxDraw( win, &tb );
*/
  zxwEditBoxDestroy( &tb );
}

void draw_gauge(zxWindow *win, zxwGauge *gauge, zxwEditBox *eb)
{
  static char buf[BUFSIZ];

  zxwGaugeDraw( win, gauge );
  sprintf( buf, "%f", zxwGaugeValue( gauge ) );
  zxwEditBoxSetString( eb, buf );
  zxwEditBoxDraw( win, eb );
  zxFlush();
  usleep( 10000 );
}

void gauge(zxWindow *win, int x, int y)
{
  zxwGauge gauge[2];
  zxwEditBox eb[2];
  double val;

  zxwGaugeCreate( &gauge[0],   x,     y,    200, 0, -2, 2 );
  zxwGaugeCreate( &gauge[1],   x,     y+32, 200, 0, -2, 2 );
  zxwEditBoxCreate( &eb[0], 0, x+216, y,     80, 0 );
  zxwEditBoxCreate( &eb[1], 0, x+216, y+32,  80, 0 );
  zxwAlignRight( &eb[0] );
  zxwAlignFlowRight( &eb[0] );
  zxwAlignRight( &eb[1] );
  zxwAlignFlowRight( &eb[1] );
  for( val=-2.2; val<=2.2; val+=0.04 ){
    zxwGaugeSetValue( &gauge[0], val );
    draw_gauge( win, &gauge[0], &eb[0] );
  }
  for( ; val>=-2.2; val-=0.04 ){
    zxwGaugeSetValue( &gauge[0], val );
    draw_gauge( win, &gauge[0], &eb[0] );
  }
  for( ; zxwKnobX(&gauge[1])<=gauge[1].guide.x+gauge[1].x_range-1; zxwKnobX(&gauge[1])+=2 )
    draw_gauge( win, &gauge[1], &eb[1] );
  for( ; zxwKnobX(&gauge[1])>=gauge[1].guide.x; zxwKnobX(&gauge[1])-=2 )
    draw_gauge( win, &gauge[1], &eb[1] );
  zxwEditBoxDestroy( &eb[0] );
  zxwEditBoxDestroy( &eb[1] );
}

#include "../pixmaps/mini.x.xpm"
#include  "../pixmaps/mini.cut.xpm"
#include  "../pixmaps/mini.cross.xpm"
#include  "../pixmaps/mini.line.xpm"
#include  "../pixmaps/mini.zoom.xpm"

void pixbutton(zxWindow *win, int x, int y)
{
  zxwPixButtonGroup pbg;
  int i;

  zxwPixButtonGroupInit( &pbg, x, y, 20, 20 );
  zxwPixButtonGroupAddData( win, &pbg, 0, 0, mini_x_xpm, NULL );
  zxwPixButtonGroupAddData( win, &pbg, 0, 1, cut_xpm, NULL );
  zxwPixButtonGroupAddData( win, &pbg, 0, 2, mini_cross_xpm, NULL );
  zxwPixButtonGroupAddData( win, &pbg, 1, 1, line_xpm, NULL );
  zxwPixButtonGroupAddData( win, &pbg, 1, 2, zoom_xpm, NULL );

  zxwPixButtonGroupDraw( win, &pbg );
  zxFlush();
  getchar();
  for( i=0; i<zxwItemNum(&pbg); i++ ){
    zxwItemActivate( &pbg, i );
    zxwPixButtonGroupDrawPress( win, &pbg );
    zxFlush();
    getchar();
    zxwPixButtonGroupDrawRelease( win, &pbg );
  }
  zxwPixButtonGroupDestroy( &pbg );
}

void progressbar(zxWindow *win, int x, int y)
{
  zxwProgressBar pb;
  int i;

  zxwProgressBarInit( &pb, x, y, 200 );
  for( i=-10; i<=110; i++ ){
    zxwProgressBarSetDegree( &pb, i );
    zxwProgressBarDraw( win, &pb );
    zxFlush();
    usleep( 10000 * zRandI(1,10) );
  }
}

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

void scrollregion(zxWindow *win, int x, int y)
{
  zxwScrollRegion sr;
  int l;

  zxwScrollRegionCreate( win, &sr, x, y, 300, 200, 1000, 1000, draw_scrollregion );
  zxwScrollRegionDraw( win, &sr, NULL );
  getchar();

  l = sr.vbar->bar.y + sr.vbar->bar.height - zxwKnobHeight(sr.vbar);
  for( ; zxwKnobY(sr.vbar)<l; zxwKnobY(sr.vbar)++ ){
    zxwScrollRegionDrawVert( win, &sr, NULL );
    usleep( 10000 );
  }
  l = sr.hbar->bar.x + sr.hbar->bar.width - zxwKnobWidth(sr.hbar);
  for( ; zxwKnobX(sr.hbar)<l; zxwKnobX(sr.hbar)++ ){
    zxwScrollRegionDrawHoriz( win, &sr, NULL );
    usleep( 10000 );
  }
  zxwScrollRegionDraw( win, &sr, NULL );
}

int main(void)
{
  zxWindow win;

  zRandInit();
  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 640, 480 );
  zxWidgetInit( &win );

  box( &win, 24, 24 );
  separator( &win, 24, 96 );
  label( &win, 24, 196 );
  radiobutton( &win, 336, 200 );
  checkbox( &win, 336, 220 );
  button( &win, 24, 264 );
  textbox( &win, 24, 296 );
  gauge( &win, 24, 360 );
  pixbutton( &win, 24, 424 );
  progressbar( &win, 96, 424 );
  scrollregion( &win, 336, 260 );

  zxFlush();
  getchar();
  zxWindowClose( &win );
  zxExit();
  return 0;
}

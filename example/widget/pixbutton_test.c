#include <zx11/zxwidget.h>
#include "../pixmaps/mini.x.xpm"
#include  "../pixmaps/mini.cut.xpm"
#include  "../pixmaps/mini.cross.xpm"
#include  "../pixmaps/mini.line.xpm"
#include  "../pixmaps/mini.zoom.xpm"

int main(void)
{
  zxWindow win;
  zxwPixButtonGroup pbg;
  register int i;

  zxInit();
  zxWindowCreateAndOpen( &win, 1000, 50, 300, 80 );
  zxWidgetInit( &win );

  zxwPixButtonGroupInit( &pbg, 20, 20, 20, 20 );
  zxwPixButtonGroupAddData( &win, &pbg, 0, 0, mini_x_xpm, NULL );
  zxwPixButtonGroupAddData( &win, &pbg, 0, 1, cut_xpm, NULL );
  zxwPixButtonGroupAddData( &win, &pbg, 0, 2, mini_cross_xpm, NULL );
  zxwPixButtonGroupAddData( &win, &pbg, 1, 1, line_xpm, NULL );
  zxwPixButtonGroupAddData( &win, &pbg, 1, 2, zoom_xpm, NULL );

  zxwPixButtonGroupDraw( &win, &pbg );
  zxFlush();
  getchar();

  for( i=0; i<zxwItemNum(&pbg); i++ ){
    zxwItemActivate( &pbg, i );
    zxwPixButtonGroupDrawPress( &win, &pbg );
    zxFlush();
    getchar();
    zxwPixButtonGroupDrawRelease( &win, &pbg );
  }
  zxwPixButtonGroupDestroy( &pbg );
  zxExit();
  return 0;
}

#include <zx11/zxutil.h>

#if 0
void zxWindowCopyFromParent(zxWindow *win)
{
  win->attr.background_pixmap = ParentRelative;
  XChangeWindowAttributes( zxdisplay,
    zxWindowBody(win), CWBackingPixel, &win->attr );
  XSetWindowBackgroundPixmap( zxdisplay, zxWindowBody(win), CopyFromParent );
  XClearWindow( zxdisplay, zxWindowBody(win) );
  zxFlush();
}
#else
void zxWindowCreateCopyFromParent(zxWindow *win, int x, int y, int w, int h)
{
  zxWindowCreate( win, x, y, w, h );
  win->attr.background_pixmap = ParentRelative;
  XChangeWindowAttributes( zxdisplay,
    zxWindowBody(win), CWBackingPixel, &win->attr );
  XSetWindowBackgroundPixmap( zxdisplay, zxWindowBody(win), CopyFromParent );
  zxWindowOpen( win );
  XClearWindow( zxdisplay, zxWindowBody(win) );
  zxFlush();
}
#endif

int main(void)
{
  zxWindow win;

  zxInit();
#if 0
  zxWindowCreateAndOpen( &win, 0, 0, 300, 300 );
  zxWindowCopyFromParent( &win );
#else
  zxWindowCreateCopyFromParent( &win, 0, 0, 300, 300 );
#endif
  getchar();
  zxClose();
  return 0;
}

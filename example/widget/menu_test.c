#include <zx11/zxwidget.h>

zxWindow win;
zxwMenu menu;
zxwPopup popup;

void *dummy(void){ return NULL; }

zxwPopupItemEntry popup_entry[] = {
  { 0, "Open",   XK_o, dummy },
  { 0, "Save",   XK_s, NULL },
  { 0, "SaveAs", XK_w, NULL },
  { 0, "Close",  XK_c, dummy },
  { 0, "-",      XK_VoidSymbol, NULL },
  { 0, "Quit",   XK_q, NULL },
  { 0, NULL,        0, NULL },
};

zxwMenuItemEntry entry[] = {
  { 0, "File", XK_f, &popup },
  { 0, "Edit", XK_e, NULL },
  { 0, "Hoge", XK_VoidSymbol, NULL },
  { 0, "Help", XK_h, NULL },
  { 0, NULL, 0, NULL },
};

void popup_op(zxwPopup *pp)
{
  zxwPopupOpen( pp, zxWindowOX(&win), zxWindowOY(&win)+24 );
  do{
    zxwPopupDrawMove( pp );
    getchar();
    zxwBackupActive( pp );
    zxwItemNext( pp );
  } while( pp->active != ZXW_ITEM_NONE );
  zxwPopupClose( pp );
  zxFlush();
}

int main(void)
{
  zxwMenuItem *mi;
  int i;

  zxInit();
  zxWindowCreateAndOpen( &win, 50, 50, 300, 80 );

  zxWidgetInit( &win );
  zxWindowSetBG( &win, "lightgray" );
  zxwMenuCreate( &win, &menu );
  zxwMenuAddItemList( &menu, entry );

  zxwPopupCreate( &popup );
  zxwPopupAddItemList( &popup, popup_entry );
  zxwMenuDraw( &win, &menu );
  zxFlush();
  getchar();

  for( i=0; i<3; i++ ){
    while( ++menu.active < menu.num ){
      zxwMenuDrawMove( &win, &menu );
      getchar();
      zxwMenuDrawPress( &win, &menu );
      if( ( mi = zxwItemActive( &menu ) )->popup )
        popup_op( mi->popup );
      getchar();
      zxwBackupActive( &menu );
    }
    menu.active = ZXW_ITEM_NONE;
  }

  zxwPopupDestroy( &popup );
  zxwMenuDestroy( &menu );
  zxClose();
  return 0;
}

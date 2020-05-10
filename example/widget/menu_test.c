#include <zx11/zxwidget.h>

zxWindow win;
zxwMenu menu;
zxwPopup popup1, popup2;

void *dummy(void){ return NULL; }

zxwPopupItemEntry popup_entry1[] = {
  { 0, "Open",   XK_o, dummy },
  { 0, "Save",   XK_s, NULL },
  { 0, "SaveAs", XK_w, NULL },
  { 0, "Close",  XK_c, dummy },
  { 0, "-",      XK_VoidSymbol, NULL },
  { 0, "Quit",   XK_q, NULL },
  { 0, NULL,        0, NULL },
};

zxwPopupItemEntry popup_entry2[] = {
  { 0, "Select", XK_s, dummy },
  { 0, "Copy",   XK_c, dummy },
  { 0, "Cut",    XK_x, dummy },
  { 0, "Paste",  XK_v, dummy },
  { 0, NULL,        0, NULL },
};

zxwMenuItemEntry entry[] = {
  { 0, "File", XK_f, &popup1 },
  { 0, "Edit", XK_e, &popup2 },
  { 0, "Hoge", XK_VoidSymbol, NULL },
  { 0, "Help", XK_h, NULL },
  { 0, NULL, 0, NULL },
};

void popup_op(zxwMenuItem *mi, zxwPopup *pp)
{
  zxRegion reg;

  zxGetGeometry(zxWindowBody(&win),&reg);
  zxwPopupOpen( pp, reg.x+mi->reg.x, reg.y+mi->reg.y+ZXWMENU_H );
  do{
    zxwPopupDrawMove( pp );
    getchar();
    zxwBackupActive( pp );
    zxwItemNext( pp );
  } while( zxwIsActive( pp ) );
  zxwPopupClose( pp );
  zxFlush();
}

int main(void)
{
  zxwMenuItem *mi;
  int i, j;

  zxInit();
  zxWindowCreateAndOpen( &win, 1000, 50, 300, 80 );
  zxWidgetInit( &win );
  zxwMenuCreate( &win, &menu );
  zxwMenuAddItemList( &menu, entry );

  zxwPopupCreate( &popup1 );
  zxwPopupAddItemList( &popup1, popup_entry1 );
  zxwPopupCreate( &popup2 );
  zxwPopupAddItemList( &popup2, popup_entry2 );
  zxwMenuDraw( &win, &menu );
  zxFlush();
  getchar();

  for( i=0; i<3; i++ ){
    for( j=0; j<zxwItemNum(&menu); j++ ){
      zxwItemActivateNext( &menu );
      zxwMenuDrawMove( &win, &menu );
      getchar();
      zxwMenuDrawPress( &win, &menu );
      if( ( mi = zxwItemActive( &menu ) )->popup )
        popup_op( mi, mi->popup );
      getchar();
      zxwBackupActive( &menu );
    }
    zxwUnactivate( &menu );
  }

  zxwPopupDestroy( &popup1 );
  zxwPopupDestroy( &popup2 );
  zxwMenuDestroy( &menu );
  zxExit();
  return 0;
}

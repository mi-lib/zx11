#include <unistd.h>
#include <pthread.h>
#include <zx11/zxsprite.h>

zxWindow win;
zxsLayer layer;
zxSprite spr;
zxVec d = { 1, 0 };

#define WAITCOUNT 60

void *draw(void *dummy)
{
  int j=0, id=0;

  while( 1 ){
    zxsPatternSet( zListHead(&layer.data.plist), 0, id );
    layer.data.bg_reg.x += d.x;
    layer.data.bg_reg.y += d.y;
    if( layer.data.bg_reg.x > 350 ){ layer.data.bg_reg.x = 350; d.x = 0; d.y = 1; }
    if( layer.data.bg_reg.y > 250 ){ layer.data.bg_reg.y = 250; d.x =-1; d.y = 0; }
    if( layer.data.bg_reg.x <   0 ){ layer.data.bg_reg.x =   0; d.x = 0; d.y =-1; }
    if( layer.data.bg_reg.y <   0 ){ layer.data.bg_reg.y =   0; d.x = 1; d.y = 0; }
    if( ++j % 10 == 0 ){
      if( ++id >= zListTail(&layer.data.plist)->data.col ) id = 0;
      j = 0;
    }
    zxSpriteDrawAll( &win, &spr );
    zxSpriteAppearAll( &win, &spr );
    zxFlush();
    usleep( 10000 );
  }
  pthread_exit( NULL );
}

int main(int argc, char *argv[])
{
  pthread_t thread;

  zxInit();

  zxWindowCreate( &win, 0, 0, 400, 300 );
  zxWindowSetBGColorByName( &win, "dark blue" );

  zxSpriteCreate( &win, &spr, 25, 25, 350, 250 );
  zxsLayerInit( &layer );
  zxsLayerCreatePixArray( &layer, 1 );
  zxsLayerReadBGPixFile( &win, &layer, "../pixmaps/quinta-2.xpm" );
  zxsLayerBGSetRegion( &layer, 0, 0, layer.data.bg.width, layer.data.bg.height );
  zxsLayerReadPixFile( &win, &layer, 0, "../pixmaps/peng-movie.xpm" );
  zxsLayerPatternListAddHead( &layer, 0, 1, 12, 140, 100 );

  zxWindowOpen( &win );
  zxSpriteAddLayer( &spr, &layer );
  zxSpriteDrawAll( &win, &spr );
  zxSpriteAppearAll( &win, &spr );

  pthread_create( &thread, NULL, draw, NULL );
  getchar();
  pthread_cancel( thread );
  pthread_join( thread, NULL );
  zxExit();
  return 0;
}

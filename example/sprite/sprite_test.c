#include <unistd.h>
#include <pthread.h>
#include <zx11/zxsprite.h>

zxWindow win;
zxsPattern *pat;
zxsLayer layer;
zxSprite spr;
zxVec d[] = { { 4, 4 }, { -8, 8 }, { 8, -4 }, { -4, -8 } };
int id[] = { 0, 5, 2, 8 };

void *draw(void *dummy)
{
  uint j;

  while( 1 ){
    for( pat=zListTail(&layer.data.plist), j=0;
         pat!=zListRoot(&layer.data.plist);
         pat=zListCellNext(pat), j++ ){
      id[j]++;
      if( id[j] >= pat->data.col ) id[j] = 0;
      zxsPatternSet( pat, 0, id[j] );
      zxsPatternAddPos( pat, d[j].x, d[j].y );
      if( pat->data.cur.x < 0 || pat->data.cur.x + pat->data.dx > spr.reg.width )
        d[j].x = -d[j].x;
      if( pat->data.cur.y < 0 || pat->data.cur.y + pat->data.dy > spr.reg.height )
        d[j].y = -d[j].y;
    }
    zxSpriteUpdate( &spr );
    zxSpriteDraw( &win, &spr );
    zxSpriteAppear( &win, &spr );
    zxFlush();
    usleep( 50000 );
  }
  pthread_exit( NULL );
}

int main(int argc, char *argv[])
{
  pthread_t thread;

  zxInit();
  zxsLayerInit( &layer );
  zxsLayerCreatePixArray( &layer, 1 );
  zxWindowCreate( &win, 0, 0, 700, 520 );
  zxWindowSetBGColorByName( &win, "blue" );
  zxSpriteCreate( &win, &spr, 80, 10, 600, 500 );

  zxsLayerReadBGPixFile( &win, &layer, "../pixmaps/quinta-2.xpm" );
  zxsLayerBGSetRegion( &layer, 0, 0, layer.data.bg.width, layer.data.bg.height );
  zxsLayerReadPixFile( &win, &layer, 0, "../pixmaps/peng-movie.xpm" );
  zxsLayerPatternListAddHead( &layer, 0, 1, 12, 0, 0 );
  zxsLayerPatternListAddHead( &layer, 0, 1, 12, spr.reg.width-100, 0 );
  zxsLayerPatternListAddHead( &layer, 0, 1, 12, 0, spr.reg.height-100 );
  zxsLayerPatternListAddHead( &layer, 0, 1, 12, spr.reg.width-100, spr.reg.height-100 );

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

/* zxpdtv - PDT file viewer
 *
 * PDT is an image format for AVG32, a visual novel engine produced
 * by verzammeln (an online distribution brand of VisualArt's).
 * This software is permitted to be published with favor of them.
 *
 * This software is conforming to PDT10 and PDT11, while compatibility
 * to the latter might be incomplete.
 *
 * Special thanks to JAGARL (Dr. Kazunori Ueno) for his contribution
 * to the implementation of PDT decoder.
 */

#include <zx11/zxpm.h>
#include <zx11/zximage_supported.h>
#include <zx11/zxinput.h>

enum{
  ZXPDTV_OPTION_GEOMETRY = 0,
  ZXPDTV_OPTION_SUPERIMPOSE,
  ZXPDTV_OPTION_PNG,
  ZXPDTV_OPTION_HELP,
  ZXPDTV_OPTION_INVALID,
};
zOption opt[] = {
  { "geometry", "geometry", "<window geometry>", "window geometry", NULL, false },
  { "si", "superimpose", "<overlay image>", "superimpose specified image to other", NULL, false },
  { "png", "png", NULL, "convert image to a PNG file", NULL, false },
  { "help", "help", NULL, "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void zxpdtv_usage(char *arg)
{
  eprintf( "Usage: %s [options] <MAG files> ...\n", arg );
  eprintf( "[options]\n" );
  zOptionHelp( opt );
  eprintf( "\n[operations]\n" );
  eprintf( " Space   show next image\n" );
  eprintf( " p       show previous image\n" );
  eprintf( " m       show/hide mask image if existing\n" );
  eprintf( " q       quit\n" );
  exit( EXIT_SUCCESS );
}

bool zxpdtv_parse_option(int argc, char *argv[], zStrAddrList *filelist)
{
  bool ret;

  if( argc <= 1 ) zxpdtv_usage( argv[0] );
  ret = zOptionRead( opt, argv+1, filelist );
  if( opt[ZXPDTV_OPTION_HELP].flag ) zxpdtv_usage( argv[0] );
  return ret;
}

void zxpdtv_draw(zxWindow *win, zxImage *img, zxImage *siimg, bool mask_draw)
{
  zxWindowClear( win );
  if( opt[ZXPDTV_OPTION_SUPERIMPOSE].flag )
    zxImagePutSuperimpose( img, siimg, 0, 0 );
  mask_draw ? zxImageDrawMaskAll( win, img, 0, 0 ) : zxImageDrawAll( win, img, 0, 0 );
  zxWindowDoubleBufferAppear( win );
  zxFlush();
}

enum{
  ZXPDTV_VIEW_NEXT,
  ZXPDTV_VIEW_PREV,
  ZXPDTV_VIEW_QUIT,
};
int zxpdtv_eventloop(zxWindow *win, zxImage *img, zxImage *siimg)
{
  bool quit_flag = false;
  bool mask_draw = false;

  zxpdtv_draw( win, img, siimg, mask_draw );
  while( !quit_flag ){
    switch( zxGetEvent() ){
    case KeyPress:
      switch( zxKeySymbol() ){
      case XK_m:     mask_draw = 1 - mask_draw; break;
      case XK_space: return ZXPDTV_VIEW_NEXT;
      case XK_p:     return ZXPDTV_VIEW_PREV;
      case XK_q:     return ZXPDTV_VIEW_QUIT;
      default: ;
      }
    case Expose:
      zxpdtv_draw( win, img, siimg, mask_draw );
      break;
    default: ;
    }
  }
  return ZXPDTV_VIEW_QUIT;
}

int zxpdtv_view(char *filename, zxRegion *reg)
{
  zxWindow win;
  zxImage img, siimg;
  int ret = ZXPDTV_VIEW_NEXT;

  if( !zxImageReadPDTFile( &img, filename ) ) return ret;
  if( !opt[ZXPDTV_OPTION_GEOMETRY].flag )
    zxRegionSet( reg, 0, 0, img.width, img.height );
  zxImageInit( &siimg );
  if( opt[ZXPDTV_OPTION_SUPERIMPOSE].flag )
    if( !zxImageReadPDTFile( &siimg, opt[ZXPDTV_OPTION_SUPERIMPOSE].arg ) ) goto TERMINATE;
  zxWindowCreateAndOpen( &win, reg->x, reg->y, reg->width, reg->height );
  zxWindowKeyEnable( &win );
  zxWindowDoubleBufferEnable( &win );
  ret = zxpdtv_eventloop( &win, &img, &siimg );
  zxWindowDestroy( &win );
  if( opt[ZXPDTV_OPTION_SUPERIMPOSE].flag )
    zxImageDestroy( &siimg );
 TERMINATE:
  zxImageDestroy( &img );
  return ret;
}

bool zxpdtv_view_list(zStrAddrList *filelist)
{
  zStrAddrListCell *cp;
  zxRegion reg;
  int ret;

  if( opt[ZXPDTV_OPTION_GEOMETRY].flag )
    zxParseGeometry( opt[ZXPDTV_OPTION_GEOMETRY].arg, &reg );
  cp = zListTail( filelist );
  while( cp != zListRoot(filelist) ){
    switch( ( ret = zxpdtv_view( cp->data, &reg ) ) ){
    case ZXPDTV_VIEW_NEXT: cp = zListCellNext(cp); break;
    case ZXPDTV_VIEW_PREV: cp = zListCellPrev(cp); break;
    case ZXPDTV_VIEW_QUIT: return true;
    }
  }
  return true;
}

void zxpdtv_to_png(zStrAddrList *filelist)
{
  zStrAddrListCell *cp;
  zxImage dat;
  char basename[BUFSIZ];
  char outputname[BUFSIZ];
  int ret;

  zListForEach( filelist, cp ){
    if( !zxImageReadPDTFile( &dat, cp->data ) ) continue;
    zGetBasename( cp->data, basename, BUFSIZ );
    zReplaceSuffix( basename, "png", outputname, BUFSIZ );
    eprintf( "open %s and convert to %s ... ", cp->data, outputname );
    ret = zxImageWritePNGFile( &dat, outputname );
    eprintf( "%s.\n", ret ? "success" : "failure" );
    zxImageDestroy( &dat );
  }
}

int main(int argc, char *argv[])
{
  zStrAddrList filelist;

  if( !zxpdtv_parse_option( argc, argv, &filelist ) )
    return EXIT_FAILURE;
  zxInit();
  if( opt[ZXPDTV_OPTION_PNG].flag )
    zxpdtv_to_png( &filelist );
  else
    zxpdtv_view_list( &filelist );
  zStrAddrListDestroy( &filelist );
  zxExit();
  return EXIT_SUCCESS;
}

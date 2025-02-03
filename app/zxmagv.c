#include <zx11/zxpm.h>
#include <zx11/zximage_supported.h>
#include <zx11/zxinput.h>

enum{
  ZXMAGV_OPTION_GEOMETRY = 0,
  ZXMAGV_OPTION_INFO,
  ZXMAGV_OPTION_GRAYSCALE,
  ZXMAGV_OPTION_BRIGHTNESS,
  ZXMAGV_OPTION_FG,
  ZXMAGV_OPTION_BG,
  ZXMAGV_OPTION_SILENT,
  ZXMAGV_OPTION_VERBOSE,
  ZXMAGV_OPTION_DUMP,
  ZXMAGV_OPTION_PPM,
  ZXMAGV_OPTION_PNG,
  ZXMAGV_OPTION_HELP,
  ZXMAGV_OPTION_INVALID,
};
zOption opt[] = {
  { "geometry", "geometry", "<window geometry>", "window geometry", NULL, false },
  { "info", "info", NULL, "output information of MAG file(s) to the standard output", NULL, false },
  { "gray", "gray", NULL, "grayscalize images", NULL, false },
  { "brightness", "brightness", "<value (0-2)>", "set brightness of images", "1.0", false },
  { "fg", "fg", "<color>", "foreground color", "white", false },
  { "bg", "bg", "<color>", "background color", "black", false },
  { "silent", "silent", NULL, "silent mode", NULL, true },
  { "verbose", "verbose", NULL, "verbose mode", NULL, false },
  { "dump", "dump", NULL, "dump images to the standard output", NULL, false },
  { "ppm", "ppm", NULL, "convert image to a PPM file (binary)", NULL, false },
  { "png", "png", NULL, "convert image to a PNG file", NULL, false },
  { "help", "help", NULL, "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void zxmagv_usage(char *arg)
{
  eprintf( "Usage: %s [options] <MAG files> ...\n", arg );
  eprintf( "[options]\n" );
  zOptionHelp( opt );
  eprintf( "\n[operations]\n" );
  eprintf( " Space   show next image\n" );
  eprintf( " p       show previous image\n" );
  eprintf( " h       scroll to left\n" );
  eprintf( " j       scroll up\n" );
  eprintf( " k       scroll down\n" );
  eprintf( " l       scroll to right\n" );
  eprintf( " q       quit\n" );
  exit( EXIT_SUCCESS );
}

bool zxmagv_parse_option(int argc, char *argv[], zStrAddrList *filelist)
{
  bool ret;

  if( argc <= 1 ) zxmagv_usage( argv[0] );
  ret = zOptionRead( opt, argv+1, filelist );
  if( opt[ZXMAGV_OPTION_HELP].flag ) zxmagv_usage( argv[0] );
  return ret;
}

void zxmagv_draw(zxWindow *win, zxPM *p, char *filename, int x, int y, int w, int h)
{
  short a, d;

  zxWindowClear( win );
  zxPMDraw( win, p, x, y, w, h, 0, 0 );
  zxWindowSetColorByName( win, opt[ZXMAGV_OPTION_FG].arg );
  zxDrawString( win, -x + ( p->width - zxTextWidth(filename) ) / 2, -y + p->height + zxTextHeight(filename,&a,&d), filename );
  zxWindowDoubleBufferAppear( win );
  zxFlush();
}

enum{
  ZXMAGV_VIEW_NEXT,
  ZXMAGV_VIEW_PREV,
  ZXMAGV_VIEW_QUIT,
};
#define ZXMAGV_D 10
int zxmagv_eventloop(zxWindow *win, zxPM *p, char *filename, zxRegion *reg)
{
  uint x, y;
  bool quit_flag = false;

  x = y = 0;
  zxmagv_draw( win, p, filename, x, y, reg->width, reg->height );
  while( !quit_flag ){
    switch( zxGetEvent() ){
    case KeyPress:
      switch( zxKeySymbol() ){
      case XK_h:     x -= ZXMAGV_D; break;
      case XK_j:     y += ZXMAGV_D; break;
      case XK_k:     y -= ZXMAGV_D; break;
      case XK_l:     x += ZXMAGV_D; break;
      case XK_space: return ZXMAGV_VIEW_NEXT;
      case XK_p:     return ZXMAGV_VIEW_PREV;
      case XK_q:     return ZXMAGV_VIEW_QUIT;
      default: ;
      }
    case Expose:
      zxmagv_draw( win, p, filename, x, y, reg->width, reg->height );
      break;
    default: ;
    }
  }
  return ZXMAGV_VIEW_QUIT;
}

#define ZXMAGV_FONT "-misc-fixed-bold-r-normal--14-130-75-75-c-70-iso8859-1"
int zxmagv_view(char *filename, zxRegion *reg)
{
  zxWindow win;
  zxImage orgdat, dat;
  zxPM p;
  int ret = ZXMAGV_VIEW_NEXT;

  if( opt[ZXMAGV_OPTION_VERBOSE].flag )
    eprintf( "viewing %s.\n", filename );
  if( opt[ZXMAGV_OPTION_INFO].flag )
    zxMAGDispCommentFile( filename );
  if( !zxImageReadMAGFile( &orgdat, filename ) ) return ret;
  if( !zxImageAllocDefault( &dat, orgdat.width, orgdat.height ) ) goto TERMINATE2;
  if( opt[ZXMAGV_OPTION_GRAYSCALE].flag ){
    if( !zxImageGrayscalize( &orgdat, &dat ) ) goto TERMINATE1;
  } else if( opt[ZXMAGV_OPTION_BRIGHTNESS].flag ){
    if( !zxImageBrighten( &orgdat, &dat, atof(opt[ZXMAGV_OPTION_BRIGHTNESS].arg) ) ) goto TERMINATE1;
  } else{
    if( !zxImageClone( &orgdat, &dat ) ) goto TERMINATE1;
  }

  if( !opt[ZXMAGV_OPTION_GEOMETRY].flag )
    zxRegionSet( reg, 0, 0, dat.width, dat.height );
  zxWindowCreateAndOpen( &win, reg->x, reg->y, reg->width, reg->height );
  zxWindowSetBGColorByName( &win, opt[ZXMAGV_OPTION_BG].arg );
  zxWindowKeyEnable( &win );
  zxWindowDoubleBufferEnable( &win );
  zxWindowSetFont( &win, ZXMAGV_FONT );
  zxImageToPM( &win, &dat, &p );
  ret = zxmagv_eventloop( &win, &p, filename, reg );

  zxWindowDestroy( &win );
  zxPMDestroy( &p );
 TERMINATE1:
  zxImageDestroy( &dat );
 TERMINATE2:
  zxImageDestroy( &orgdat );
  return ret;
}

bool zxmagv_view_list(zStrAddrList *filelist)
{
  zStrAddrListCell *cp;
  zxRegion reg;
  int ret;

  if( opt[ZXMAGV_OPTION_GEOMETRY].flag )
    zxParseGeometry( opt[ZXMAGV_OPTION_GEOMETRY].arg, &reg );
  cp = zListTail( filelist );
  while( cp != zListRoot(filelist) ){
    switch( ( ret = zxmagv_view( cp->data, &reg ) ) ){
    case ZXMAGV_VIEW_NEXT: cp = zListCellNext(cp); break;
    case ZXMAGV_VIEW_PREV: cp = zListCellPrev(cp); break;
    case ZXMAGV_VIEW_QUIT: return true;
    }
  }
  return true;
}

bool zxmagv_dump(zStrAddrList *filelist)
{
  zStrAddrListCell *cp;
  zxImage dat;
  register int i, j;

  zListForEach( filelist, cp ){
    if( !zxImageReadMAGFile( &dat, cp->data ) ) return false;
    if( opt[ZXMAGV_OPTION_VERBOSE].flag )
      eprintf( "dump %s to the standard output.\n", cp->data );
    for( i=0; i<dat.height; i++ ){
      for( j=0; j<dat.width; j++ )
        printf( "%X", zxImageCellPixel(&dat,j,i) );
      printf( "\n" );
    }
    zxImageDestroy( &dat );
  }
  return true;
}

void zxmagv_convert(zStrAddrList *filelist, int (* image_write)(zxImage*,const char*), char *suffix)
{
  zStrAddrListCell *cp;
  zxImage dat;
  char basename[BUFSIZ];
  char outputname[BUFSIZ];
  char format[BUFSIZ];
  int ret;

  if( opt[ZXMAGV_OPTION_VERBOSE].flag )
    eprintf( "convert MAG files to %s files.\n", zStrToUpper(suffix,BUFSIZ,format) );
  zListForEach( filelist, cp ){
    if( !zxImageReadMAGFile( &dat, cp->data ) ) continue;
    zGetBasename( cp->data, basename, BUFSIZ );
    zReplaceSuffix( basename, suffix, outputname, BUFSIZ );
    if( opt[ZXMAGV_OPTION_VERBOSE].flag )
      eprintf( "open %s and convert to %s ... ", cp->data, outputname );
    ret = image_write( &dat, outputname );
    if( opt[ZXMAGV_OPTION_VERBOSE].flag )
      eprintf( "%s.\n", ret ? "success" : "failure" );
    zxImageDestroy( &dat );
  }
}

void zxmagv_to_ppm(zStrAddrList *filelist)
{
  zxmagv_convert( filelist, zxImageWritePPMFile, "ppm" );
}

void zxmagv_to_png(zStrAddrList *filelist)
{
  zxmagv_convert( filelist, zxImageWritePNGFile, "png" );
}

int main(int argc, char *argv[])
{
  zStrAddrList filelist;

  if( !zxmagv_parse_option( argc, argv, &filelist ) )
    return EXIT_FAILURE;
  if( opt[ZXMAGV_OPTION_VERBOSE].flag )
    eprintf( "zxmagv programmed by Zhidao, May 12, 2020.\n" );
  zxInit();
  if( opt[ZXMAGV_OPTION_DUMP].flag )
    zxmagv_dump( &filelist );
  else if( opt[ZXMAGV_OPTION_PPM].flag )
    zxmagv_to_ppm( &filelist );
  else if( opt[ZXMAGV_OPTION_PNG].flag )
    zxmagv_to_png( &filelist );
  else
    zxmagv_view_list( &filelist );
  zStrAddrListDestroy( &filelist );
  zxExit();
  return EXIT_SUCCESS;
}

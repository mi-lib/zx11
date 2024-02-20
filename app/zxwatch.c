/* zxwatch - digital watch on X11

% zxwatch [options]
[options]
 -image    <image file> put an <image file> on the background of the watch
 -geometry WIDTHxHEIGHT+X+Y
                        set geometry of the watch
 -clip     +X+Y         set the clip coordinate of the background image
 -color    <color>      set the color of digit segments
                        refer /etc/X11/rgb.txt for <color>
 -h                     show how to use this

 * 2002. 4.18. Created.
 * 2024. 2.20. Last updated.
 */

#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <zx11/zximage.h>
#include <zx11/zxinput.h>

#define TITLE "zxwatch ver4.0"

zxWindow win;
Pixmap img;

#define DEFAULT_WIDTH  260
#define DEFAULT_HEIGHT  80

zxRegion winreg;
zxRegion imgreg;
int watch_x, watch_y;
int needlecolor;

enum{
  ZW_YEAR1=0, ZW_YEAR2, ZW_YEAR3, ZW_YEAR4,
  ZW_MONTH1, ZW_MONTH2,
  ZW_DAY1, ZW_DAY2,
  ZW_HOUR1, ZW_HOUR2,
  ZW_MIN1, ZW_MIN2,
  ZW_SEC1, ZW_SEC2,
  ZW_USEC1, ZW_USEC2,
  ZW_LAST,
};

#define ZW_BUFSIZ ZW_LAST

#define ZW_INTERVAL_MS 1000

void zwGetTime(char str[])
{
  struct timespec ts;
  struct tm tmp;
  time_t now;

  clock_gettime( CLOCK_REALTIME, &ts );
  now = ts.tv_sec;
  strftime( str, ZW_BUFSIZ, "%Y%m%d%H%M%S%U", localtime_r( &now, &tmp ) );
}

/* digital display */

void _zwPointOffset(XPoint p[], int x, int y, int n)
{
  register int i;
  for( i=0; i<n; i++ ){
    p[i].x += x; p[i].y += y;
  }
}

#define zwPointOffset(p,x,y) _zwPointOffset( p, x, y, sizeof(p)/sizeof(XPoint) )

void zwSegmentH(int x, int y)
{
  XPoint p[] = { { 1, 0 }, { 2, 3 }, { 20, 3 }, { 23, 0 }, { 22, -2 }, { 4, -2 } };
  zwPointOffset( p, x, y );
  zxDrawFillPolygon( &win, p, sizeof(p)/sizeof(XPoint) );
}

void zwSegmentV(int x, int y)
{
  XPoint p[] = { { 0, 1 }, { -3, 3 }, { -6, 21 }, { -4, 23 }, { -1, 21 }, { 2, 3 } };
  zwPointOffset( p, x, y );
  zxDrawFillPolygon( &win, p, sizeof(p)/sizeof(XPoint) );
}

void zwSegments(int x, int y, unsigned char sw)
{
  XPoint p[] = { { 4, -24 }, { 4, -24 }, { 0, 0 }, { -4, 24 }, { 24, 0 }, { 28, -24 }, { 0, 0 } };
  void (* segment[])(int,int) = { zwSegmentH, zwSegmentV, zwSegmentV, zwSegmentH, zwSegmentV, zwSegmentV, zwSegmentH };
  register int i;
  zwPointOffset( p, x, y );
  for( i=0; i<7; i++ )
    if( sw & ( 0x01 << i ) ) segment[i]( p[i].x, p[i].y );
}

void zwDigit(int x, int y, int val)
{
  static unsigned char sw[] = {
    0x3f, 0x30, 0x6d, 0x79, 0x72, 0x5b, 0x5f, 0x31, 0x7f, 0x7b,
  };
  zwSegments( x, y, sw[val] );
}

void zwSpltr(int x, int y)
{
  XPoint p[] = { { 2, -10 }, { -1, 10 } };
  zwPointOffset( p, x, y );
  zxDrawFillCircle( &win, p[0].x, p[0].y, 3 );
  zxDrawFillCircle( &win, p[1].x, p[1].y, 3 );
}

void zwDisplay(char str[])
{
  if( img != None )
    zxPixmapDraw( &win, img,
      imgreg.x, imgreg.y, winreg.width, winreg.height, 0, 0 );
  else
    zxWindowClear( &win );

  zxWindowSetColor( &win, needlecolor );
  if( str[ZW_HOUR1] != '0' )
    zwDigit( watch_x+0, watch_y, str[ZW_HOUR1]-'0' );
  zwDigit( watch_x+ 32, watch_y, str[ZW_HOUR2]-'0' );
  zwSpltr( watch_x+ 64, watch_y );
  zwDigit( watch_x+ 72, watch_y, str[ZW_MIN1]-'0' );
  zwDigit( watch_x+104, watch_y, str[ZW_MIN2]-'0' );
  zwSpltr( watch_x+136, watch_y );
  zwDigit( watch_x+144, watch_y, str[ZW_SEC1]-'0' );
  zwDigit( watch_x+178, watch_y, str[ZW_SEC2]-'0' );
  zxWindowDoubleBufferAppear( &win );
  zxFlush();
}

void zwAlign(int w, int h)
{
  if( w > 0 && h > 0 ){
    if( imgreg.x > w ){
      fprintf( stderr, "invalid offset for X, reduced to 0.\n" );
      imgreg.x = 0;
    }
    if( imgreg.y > h ){
      fprintf( stderr, "invalid offset for Y, reduced to 0.\n" );
      imgreg.y = 0;
    }
    if( winreg.width == 0 || w - imgreg.x < winreg.width )
      winreg.width = w - imgreg.x;
    if( winreg.height == 0 || h - imgreg.y < winreg.height )
      winreg.height = h - imgreg.y;
  } else{
    if( winreg.width == 0 )
      winreg.width = DEFAULT_WIDTH;
    if( winreg.height == 0 )
      winreg.height = DEFAULT_HEIGHT;
  }
  watch_x = ( winreg.width - 210 ) / 2;
  watch_y = winreg.height / 2;
}

/* main */

enum{
  ZW_HELP = 0,
  ZW_IMAGE,
  ZW_GEOMETRY,
  ZW_CLIP,
  ZW_COLOR,
  ZW_INVALID,
};
zOption option[] = {
  { "h",        "help",     NULL,               "show how to use this",    NULL, false },
  { "image",    "image",    "<image file>",     "background image file",   NULL, false },
  { "geometry", "geometry", "WIDTHxHEIGHT+X+Y", "geometry of the main window", "260x80+0+0", false },
  { "clip",     "clip",     "+X+Y",             "clipping geometry of the background image", "0x0+0+0", false },
  { "color",    "color",    "<color>",          "color of digit segments (refer /etc/X11/rgb.txt for <color>)", "white", false },
  { NULL, NULL, NULL, NULL, false },
};

void zwUsage(char *arg)
{
  eprintf( "Usage: %s [options]\n", arg );
  eprintf( "Options:\n" );
  zOptionHelp( option );
  eprintf( "JPEG, PNG, BMP, PDT, MAG and PNM are available for the background image.\n" );
  exit( 1 );
}

void zwInit(int argc, char **argv)
{
  zStrAddrList arglist;
  zxImage image;

  zxInit();
  zxImageInit( &image );
  img = None;

  zListInit( &arglist );
  zOptionRead( option, argv+1, &arglist );
  if( option[ZW_HELP].flag ) zwUsage( argv[0] );
  zxParseGeometry( option[ZW_GEOMETRY].arg, &winreg );
  zxParseGeometry( option[ZW_CLIP].arg, &imgreg );
  if( option[ZW_IMAGE].flag )
    zxImageReadFile( &image, option[ZW_IMAGE].arg );
  zwAlign( image.width, image.height );

  zxWindowCreateAndOpen( &win,
    winreg.x, winreg.y, winreg.width, winreg.height );
  zxWindowSetTitle( &win, TITLE );
  zxWindowDoubleBufferEnable( &win );
  zxWindowKeyEnable( &win );
  needlecolor = zxGetColor( &win, option[ZW_COLOR].arg );
  if( option[ZW_IMAGE].flag ){
    img = zxImageCreatePixmap( &win, &image );
    zxImageDestroy( &image );
  }
}

int main(int argc, char *argv[])
{
  char str[ZW_BUFSIZ+1];
  struct timespec ts = { 0, ZW_INTERVAL_MS * 1000 * 1000 };
  bool quit_flag = false;

  zwInit( argc, argv );
  while( !quit_flag ){
    zwGetTime( str );
    zwDisplay( str );
    nanosleep( &ts, NULL );
    if( zxCheckEvent( KeyPress ) ){
      switch( zxKeySymbol() ){
      case XK_q: case XK_Q: case XK_Escape: quit_flag = true; break;
      default: ;
      }
    }
  }
  zxWindowDestroy( &win );
  zxPixmapDestroy( img );
  zxExit();
  return 0;
}

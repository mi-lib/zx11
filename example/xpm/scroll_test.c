#include <unistd.h>
#include <zx11/zxinput.h>
#include <zx11/zxpm.h>
#include <zx11/zximage.h>

void draw(zxWindow *win, zxPM *p, int x, int y, int w, int h)
{
  zxWindowClear( win );
  zxPMDraw( win, p, x, y, w, h, 0, 0 );
  zxWindowDoubleBufferAppear( win );
  zxFlush();
}

int main(int argc, char *argv[])
{
  uint x, y, w, h;
  zxWindow win;
  zxImage dat;
  zxPM p;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  w = dat.width / 2;
  h = dat.height / 2;
  zxWindowCreateAndOpen( &win, 0, 0, w, h );
  zxWindowKeyEnable( &win );
  zxWindowDoubleBufferEnable( &win );
  zxWindowSetBGColorByName( &win, "lemmon chiffon" );
  zxImageToPM( &win, &dat, &p );

  x = y = 0;
  draw( &win, &p, x, y, w, h );
  while( 1 ){
    switch( zxGetEvent() ){
    case Expose:
      draw( &win, &p, x, y, w, h );
      break;
    case KeyPress:
      switch( zxKeySymbol() ){
      case XK_h:
        x -= (x>0) ? 10 : 0; break;
      case XK_j:
        y += (y<dat.height-h) ? 10 : 0; break;
      case XK_k:
        y -= (y>0) ? 10 : 0; break;
      case XK_l:
        x += (x<dat.width-w) ? 10 : 0; break;
      case XK_q:
        goto TERMINATE;
      }
      draw( &win, &p, x, y, w, h );
      break;
    default: ;
    }
  }

 TERMINATE:
  zxImageDestroy( &dat );
  zxWindowClose( &win );
  zxPMDestroy( &p );
  zxExit();
  return 0;
} 

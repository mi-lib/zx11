#include <unistd.h>
#include <zx11/zxinput.h>
#include <zx11/zxpm.h>
#include <zx11/zximage.h>

void draw(zxWindow *win, zxPM *p, int x, int y, int w, int h)
{
  zxClear( win );
  zxPMDraw( win, p, x, y, w, h, 0, 0 );
  zxDoubleBufferAppear( win );
  zxFlush();
}

int main(int argc, char *argv[])
{
  uint x, y, w, h;
  zxWindow win;
  zxImage dat;
  zxPM p;

  w = 300; h = 300;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, w, h );
  zxKeyEnable( &win );
  zxDoubleBufferEnable( &win );
  zxSetBGColor( &win, zxGetColor( &win, "lemmon chiffon" ) );
  zxImageToPM( &win, &dat, &p );

  x = y = 0;
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
  zxClose();
  return 0;
} 

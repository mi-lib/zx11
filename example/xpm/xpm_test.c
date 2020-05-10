#ifndef __ZX11_USE_XPM
#define __ZX11_USE_XPM
#endif
#include <zx11/zxpm.h>

#include "../pixmaps/peng-movie.xpm"

int main(int argc, char *argv[])
{
  zxWindow win;
  zxPM p;

  zxInit();
  zxWindowCreateAndOpen( &win, 0, 0, 1000, 400 );
  zxWindowSetBGColorByName( &win, "gray" );
  zxWindowDoubleBufferEnable( &win );
  zxWindowClear( &win );

  zxPMInit( &p );
  zxPMReadData( &win, &p, peng_movie );
  zxPMDrawAll( &win, &p, 0, 200 );
  zxPMDestroy( &p );

  zxPMReadFile( &win, &p, "../pixmaps/peng-movie.xpm" );
  zxPMDrawAll( &win, &p, 20, 100 );
  zxPMDestroy( &p );

  zxClipUnsetMask( &win );
  zxWindowSetColorByName( &win, "red" );
  zxSetLineWidth( &win, 5 );
  zxDrawLine( &win, 0, 0, 1000, 400 );

  zxWindowDoubleBufferAppear( &win );
  zxFlush();
  getchar();

  zxExit();
  return 0;
}

#ifndef __ZX11_USE_XPM
#define __ZX11_USE_XPM
#endif

#ifndef __ZX11_USE_XEXT
#define __ZX11_USE_XEXT
#endif

#include <zx11/zxpm.h>
#include "../pixmaps/peng-movie.xpm"

int main()
{
  zxWindow win;
  zxPM p;

  zxInit();
  zxWindowCreate( &win, 0, 200, 1000, 100 );
  zxWindowOverrideRedirectEnable( &win );
  zxWindowOpen( &win );
  zxPMInit( &p );
  zxPMReadData( &win, &p, peng_movie );
  zxPMClipShape( &win, &p );
  zxPMDrawAll( &win, &p, 0, 0 );
  zxFlush();
  getchar();
  zxExit();
  return 0;
}

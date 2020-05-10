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
  zxWindowCreateAndOpen( &win, 0, 0, 1000, 300 );
  zxWindowSetBGColorByName( &win, "gray" );

  zxPMReadData( &win, &p, peng_movie );
  zxPMDrawAll( &win, &p, 0, 100 );
  zxPMWriteFile( &p, "daa.xpm" );

  zxPMReadFile( &win, &p, "../pixmaps/peng-movie.xpm" );
  zxPMDrawAll( &win, &p, 0, 0 );
  zxPMWriteFile( &p, "dee.xpm" );
  getchar();

  zxExit();
  return 0;
}

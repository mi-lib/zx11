#include <zx11/zximage_mag.h>

int main(int argc, char *argv[])
{
  zxImage dat;
  zxWindow win;
  char *filename = argc > 1 ? argv[1] : "fig/moon.mag";

  zxInit();
  zxMAGDispCommentFile( filename );
  zxImageReadMAGFile( &dat, filename );

  zxWindowCreateAndOpen( &win, 0, 0, dat.width, dat.height );
  while( zxGetEvent() != Expose );
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxClose();
  return 0;
}

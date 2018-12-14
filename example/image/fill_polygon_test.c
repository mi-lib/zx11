#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage dat;
  Pixmap bg;
  XPoint points[] = {
    { 320,  40 },
    { 140, 440 },
    { 600, 160 },
    {  40, 160 },
    { 500, 440 },
  };

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "fig/lena_mini.jpg" ) == 0 )
    exit( 1 );

  zxWindowCreateAndOpen( &win, 0, 0, 640, 480 );
  while( zxGetEvent() != Expose );
  bg = zxImageCreatePixmap( &win, &dat );
  zxImageDestroy( &dat );

  /* EvenOddRule or WindingRule */
  zxDrawPixmapPolygon( &win, points, 5, bg );
  zxFlush();
  getchar();
  zxClose();
  return 0;
} 

#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win1, win2;
  zxImage dat;
  Pixmap bg;
  XPoint points[] = {
    { 320,   0 },
    { 140, 480 },
    { 600, 160 },
    {  40, 160 },
    { 500, 480 },
  };

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win1, 0, 0, 640, 480 );
  bg = zxImageCreatePixmap( &win1, &dat );
  /* EvenOddRule or WindingRule */
  zxDrawPixmapPolygon( &win1, points, 5, bg );

  zxWindowCreateAndOpen( &win2, 640, 0, dat.width*2.5, dat.height*2.5 );
  bg = zxImageCreatePixmap( &win2, &dat );
  zxWindowFill( &win2, bg );

  zxFlush();
  getchar();
  zxImageDestroy( &dat );
  zxExit();
  return 0;
} 

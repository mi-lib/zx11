/*
 * Sierpinski's gasket
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000. 3.18. Created.
 * 2000. 6.25. Applied zxutil.o.
 */

#include <stdio.h>
#include <stdlib.h>
#include <zx11/zxutil.h>

void Sierpinski(zxWindow *c, double x1, double y1, double x2, double y2, double x3, double y3, int dim)
{
  double x4, y4, x5, y5, x6, y6;

  if( dim == 0 ) return;

  zxDrawLine( c, x1, y1, x2, y2 );
  zxDrawLine( c, x2, y2, x3, y3 );
  zxDrawLine( c, x3, y3, x1, y1 );
  x4 = ( x1 + x2 ) / 2;   y4 = ( y1 + y2 ) / 2;
  x5 = ( x2 + x3 ) / 2;   y5 = ( y2 + y3 ) / 2;
  x6 = ( x3 + x1 ) / 2;   y6 = ( y3 + y1 ) / 2;
  Sierpinski( c, x1, y1, x4, y4, x6, y6, dim-1 );
  Sierpinski( c, x4, y4, x2, y2, x5, y5, dim-1 );
  Sierpinski( c, x6, y6, x5, y5, x3, y3, dim-1 );
  zxFlush();
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 5;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 400, 400 );
  zxWindowSetTitle( &c, "Sierpinski's gasket" );
  zxWindowClear( &c );
  zxSetColor( &c, "white" );
  Sierpinski( &c, 0, 350, 400, 350, 200, 30, dim );
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

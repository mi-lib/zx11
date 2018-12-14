/*
 * C curve
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000. 4.16. Created.
 * 2000. 6.25. Applied zxutil.o.
 */

#include <stdio.h>
#include <stdlib.h>
#include <zx11/zxutil.h>

void CCurve(zxWindow *c, double x1, double y1, double x2, double y2, int dim)
{
  double x3, y3;

  if( dim <= 0 ){
    zxDrawLine( c, x1, y1, x2, y2 );
    return;
  }
  x3 = 0.5 * ( x1 + x2 - y1 + y2 );
  y3 = 0.5 * ( x1 - x2 + y1 + y2 );
  CCurve( c, x1, y1, x3, y3, dim-1 );
  CCurve( c, x3, y3, x2, y2, dim-1 );
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 10;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 500, 500 );
  zxWindowSetTitle( &c, "C curve" );
  zxWindowClear( &c );

  zxSetColor( &c, "green" );
  CCurve( &c, 150, 300, 350, 300, dim );
  zxFlush();
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

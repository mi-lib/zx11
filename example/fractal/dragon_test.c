/*
 * dragon curve
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000. 4.16. Created.
 * 2000. 6.25. Applied zxutil.o.
 */

#include <stdio.h>
#include <stdlib.h>
#include <zx11/zxutil.h>

void DragonCurve(zxWindow *c, double x1, double y1, double x2, double y2, int dim)
{
  double x3, y3, x4, y4, x5, y5;

  if( dim <= 0 ){
    zxDrawLine( c, x1, y1, x2, y2 );
    return;
  }
  x3 = 0.5 * ( x1 + x2 );
  y3 = 0.5 * ( y1 + y2 );
  x4 = 0.5 * ( x1 + x3 - y1 + y3 );
  y4 = 0.5 * ( x1 - x3 + y1 + y3 );
  x5 = 0.5 * ( x2 + x3 - y2 + y3 );
  y5 = 0.5 * ( x2 - x3 + y2 + y3 );

  DragonCurve( c, x1, y1, x4, y4, dim-1 );
  DragonCurve( c, x4, y4, x3, y3, dim-1 );
  DragonCurve( c, x3, y3, x5, y5, dim-1 );
  DragonCurve( c, x5, y5, x2, y2, dim-1 );
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 10;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 500, 500 );
  zxWindowSetTitle( &c, "Dragon curve" );
  zxWindowClear( &c );
  zxSetColor( &c, "green" );
  DragonCurve( &c, 100, 250, 400, 250, dim );
  zxFlush();
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

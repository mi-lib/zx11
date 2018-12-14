/*
 * Koch curve
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000. 3.16. Created.
 * 2000. 6.25. Applied zxutil.o.
 * 2001. 7.21. Treated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <zx11/zxutil.h>

#define PI 3.1415926535

double sn, cs;

void KochCurve(zxWindow *c, double x1, double y1, double x2, double y2, int dim)
{
  double x3, y3, x4, y4, x5, y5;

  if( dim <= 0 ){
    zxDrawLine( c, x1, y1, x2, y2 );
    return;
  }
  x3 = ( 2*x1 +   x2 ) / 3;
  y3 = ( 2*y1 +   y2 ) / 3;
  x5 = (   x1 + 2*x2 ) / 3;
  y5 = (   y1 + 2*y2 ) / 3;
  x4 = x3 + (x5-x3)*cs + (y5-y3)*sn;
  y4 = y3 - (x5-x3)*sn + (y5-y3)*cs;
  KochCurve( c, x1, y1, x3, y3, dim-1 );
  KochCurve( c, x3, y3, x4, y4, dim-1 );
  KochCurve( c, x4, y4, x5, y5, dim-1 );
  KochCurve( c, x5, y5, x2, y2, dim-1 );
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 5;
  sn = sin(PI/3); cs = cos(PI/3);

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 500, 300 );
  zxWindowSetTitle( &c, "Koch curve" );
  zxWindowClear( &c );

  zxSetColor( &c, "green" );
  KochCurve( &c, 0, 200, 500, 200, dim );
  zxFlush();
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

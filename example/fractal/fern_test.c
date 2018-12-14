/*
 * Fern
 * (C)Copyright, Zhidao since 2000.
 *
 * 2001. 7.21. Created.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <zx11/zxutil.h>

#define PI 3.1415926535

void Fern(zxWindow *c, double x1, double y1, double x2, double y2, double x3, double y3, int dim)
{
  double x4, y4, x5, y5;

  if( dim == 0 ){
    zxDrawLine( c, x1, y1, x2, y2 );
    zxDrawLine( c, x2, y2, x3, y3 );
    zxDrawLine( c, x3, y3, x1, y1 );
    return;
  }

  x4 = ( 2*x1        +   x3 ) / 3.0;
  y4 = ( 2*y1        +   y3 ) / 3.0;
  x5 = ( 4*x1 - 3*x2 + 5*x3 ) / 6.0;
  y5 = ( 4*y1 - 3*y2 + 5*y3 ) / 6.0;
  Fern( c, x1, y1, x4, y4, x2, y2, dim-1 );
  Fern( c, x4, y4, x5, y5, x3, y3, dim-1 );
  zxFlush();
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 8;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 500, 500 );
  zxWindowSetTitle( &c, "Fern" );
  zxWindowClear( &c );
  zxSetColor( &c, "lightgreen" );
  Fern( &c, 50, 300, 250, 100, 450, 300, dim );
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

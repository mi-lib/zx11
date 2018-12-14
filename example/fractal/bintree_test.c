/*
 * Binary tree
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000. 3.18. Created.
 * 2000. 6.25. Applied zxutil.o.
 * 2001. 7.21. Treated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <zx11/zxutil.h>

#define PI 3.1415926535

void BinaryTree(zxWindow *c, double x1, double y1, double x2, double y2, double angle, int dim)
{
  double sn, cs, dx, dy, x3, y3, x4, y4;

  zxDrawLine( c, x1, y1, x2, y2 );

  if( dim == 0 ) return;
  sn = sin(angle); cs = cos(angle);
  dx = 0.5*(x2-x1); dy = 0.5*(y2-y1);
  x3 = x2 + dx*cs - dy*sn;
  y3 = y2 + dx*sn + dy*cs;
  x4 = x2 + dx*cs + dy*sn;
  y4 = y2 - dx*sn + dy*cs;
  BinaryTree( c, x2, y2, x3, y3, angle, dim-1 );
  BinaryTree( c, x2, y2, x4, y4, angle, dim-1 );
  zxFlush();
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 10;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 400, 400 );
  zxWindowSetTitle( &c, "Binary tree" );
  zxWindowClear( &c );

  zxSetColor( &c, "lightgreen" );
  BinaryTree( &c, 200, 400, 200, 200, PI/6, dim );
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

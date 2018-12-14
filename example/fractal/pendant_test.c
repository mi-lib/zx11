/*
 * Fractal Pendant
 * (C)Copyright, Zhidao since 2000.
 *
 * 2000.11.10. Created.
 * 2001. 7.21. Treated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <zx11/zxutil.h>

static char *color[] = {
  "yellow", "cyan", "red", "green", "magenta"
};

#define PI 3.1415926535

typedef struct {
  double x, y;
} vector;

vector *Rotate(vector *v1, double theta, vector *v2)
{
  double cs, sn;

  sn = sin( theta );
  cs = cos( theta );
  v2->x = cs*v1->x - sn*v1->y;
  v2->y = sn*v1->x + cs*v1->y;
  return v2;
}

static double ratio;
static XPoint points[10];

void Pendant(zxWindow *c, double ox, double oy, double dx, double dy, int dim, int flag)
{
  register int i;
  double r1, r2;
  vector d[5], e[5];

  if( dim <= 0 ) return;
  r1 = sqrt( dx*dx + dy*dy );
  r2 = ratio * r1;
  points[0].x = (short)( ox + ( d[0].x = dx ) );
  points[0].y = (short)( oy + ( d[0].y = dy ) );
  e[1].x = dx*r2/r1;
  e[1].y = dy*r2/r1;
  Rotate( &e[1], 0.2*PI, &e[0] );
  points[1].x = (short)( ox + e[0].x );
  points[1].y = (short)( oy + e[0].y );
  for( i=1; i<5; i++ ){
    Rotate( &d[0], 0.4*i*PI, &d[i] );
    points[i*2  ].x = (short)( ox + d[i].x );
    points[i*2  ].y = (short)( oy + d[i].y );
    Rotate( &e[0], 0.4*i*PI, &e[i] );
    points[i*2+1].x = (short)( ox + e[i].x );
    points[i*2+1].y = (short)( oy + e[i].y );
  }
  zxSetColor( c, color[dim%5] );
  zxDrawFillPolygonNonConvex( c, points, 10 );

  if( flag )
    Pendant( c, ox+(1+ratio)*d[0].x, oy+(1+ratio)*d[0].y,-ratio*d[0].x,-ratio*d[0].y, dim-1, 0 );
  for( i=1; i<5; i++ )
    Pendant( c, ox+(1+ratio)*d[i].x, oy+(1+ratio)*d[i].y,-ratio*d[i].x,-ratio*d[i].y, dim-1, 0 );
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 5;
  ratio = 1.0/(2*cos(0.2*PI)+1);

  zxInit();
  zxWindowCreateAndOpen( &c, 0, 0, 500, 500 );
  zxWindowSetTitle( &c, "Pendant" );
  zxWindowClear( &c );
  zxSetColor( &c, "white" );
  Pendant( &c, 250, 250, 0, -80, dim, 1 );
  zxFlush();
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

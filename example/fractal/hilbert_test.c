/*
 * Hilbert curve
 *
 * 2000. 4.15. Created. : Zhidao
 * 2000.11. 8. Debugged. : Zhidao
 * 2001. 7.21. Treated. : Zhidao
 */

#include <stdio.h>
#include <stdlib.h>
#include <zx11/zxutil.h>

static char *color[] = {
  "yellow", "red", "green", "blue"
};

typedef struct {
  double x, y;
} vector;

static struct {
  vector p, d;
  double len;
} t;

#define RotateRight(d) ( ( (d)+1 ) % 4 )
#define RotateLeft(d)  ( ( (d)+3 ) % 4 )
#define Rotate(d,c)    ( (c) ? RotateRight(d) : RotateLeft(d) )
#define Reverse(d)     ( ( (d)+2 ) % 4 )

vector *Direction(int dir, vector *v)
{
  switch( dir ){
  case 0:  v->x = 0; v->y =-1; break;
  case 1:  v->x = 1; v->y = 0; break;
  case 2:  v->x = 0; v->y = 1; break;
  case 3:  v->x =-1; v->y = 0; break;
  default: v->x = 0; v->y = 0; break;
  }
  return v;
}

void HilbertCurve(zxWindow *c, double ox, double oy, double w, int dir, int cw, int dim)
{
  vector v1, v2;

  if( dim == 0 ){
    t.p.x = ox; t.p.y = oy; t.len = w*2; return;
  }

  Direction( dir, &v1 );
  Direction( Rotate( dir, cw ), &v2 );
  w *= 0.5;
  HilbertCurve( c, ox + w * (-v1.x+v2.x), oy + w * (-v1.y+v2.y),
    w, Rotate(dir,1-cw), 1-cw, dim-1 );
  zxSetColor( c, color[dir] );
  zxDrawLine( c, t.p.x, t.p.y, t.p.x+v1.x*t.len, t.p.y+v1.y*t.len );
  HilbertCurve( c, ox + w * ( v1.x+v2.x), oy + w * ( v1.y+v2.y),
    w, dir, cw, dim-1 );
  zxSetColor( c, color[Rotate(dir,1-cw)] );
  zxDrawLine( c, t.p.x, t.p.y, t.p.x-v2.x*t.len, t.p.y-v2.y*t.len );
  HilbertCurve( c, ox + w * ( v1.x-v2.x), oy + w * ( v1.y-v2.y),
    w, dir, cw, dim-1 );
  zxSetColor( c, color[Reverse(dir)] );
  zxDrawLine( c, t.p.x, t.p.y, t.p.x-v1.x*t.len, t.p.y-v1.y*t.len );
  HilbertCurve( c, ox + w * (-v1.x-v2.x), oy + w * (-v1.y-v2.y),
    w, Rotate(dir,cw), 1-cw, dim-1 );
}

int main(int argc, char *argv[])
{
  zxWindow c;
  register int dim;

  dim = argc > 1 ? atoi(argv[1]) : 5;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, 500, 500 );
  zxWindowSetTitle( &c, "Hilbert curve" );
  zxWindowClear( &c );
  zxSetColor( &c, "white" );
  HilbertCurve( &c, 250, 250, 250, 0, 0, dim );
  zxFlush();
  printf( "dimention = %d", dim );
  getchar();

  zxClose();
  return 1;
}

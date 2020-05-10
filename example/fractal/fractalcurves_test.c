#include <zx11/zxutil.h>
#include <math.h>

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
}

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
}

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
}

typedef struct {
  double x, y;
} vector;

static struct {
  vector p, d;
  double len;
} t;

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

#define IndexRotateRight(d) ( ( (d)+1 ) % 4 )
#define IndexRotateLeft(d)  ( ( (d)+3 ) % 4 )
#define IndexRotate(d,c)    ( (c) ? IndexRotateRight(d) : IndexRotateLeft(d) )
#define IndexReverse(d)     ( ( (d)+2 ) % 4 )
void HilbertCurve(zxWindow *c, double ox, double oy, double w, int dir, int cw, int dim)
{
  vector v1, v2;
  static char *color[] = {
    "yellow", "red", "green", "blue"
  };

  if( dim == 0 ){
    t.p.x = ox; t.p.y = oy; t.len = w*2; return;
  }

  Direction( dir, &v1 );
  Direction( IndexRotate( dir, cw ), &v2 );
  w *= 0.5;
  HilbertCurve( c, ox + w * (-v1.x+v2.x), oy + w * (-v1.y+v2.y),
    w, IndexRotate(dir,1-cw), 1-cw, dim-1 );
  zxWindowSetColorByName( c, color[dir] );
  zxDrawLine( c, t.p.x, t.p.y, t.p.x+v1.x*t.len, t.p.y+v1.y*t.len );
  HilbertCurve( c, ox + w * ( v1.x+v2.x), oy + w * ( v1.y+v2.y),
    w, dir, cw, dim-1 );
  zxWindowSetColorByName( c, color[IndexRotate(dir,1-cw)] );
  zxDrawLine( c, t.p.x, t.p.y, t.p.x-v2.x*t.len, t.p.y-v2.y*t.len );
  HilbertCurve( c, ox + w * ( v1.x-v2.x), oy + w * ( v1.y-v2.y),
    w, dir, cw, dim-1 );
  zxWindowSetColorByName( c, color[IndexReverse(dir)] );
  zxDrawLine( c, t.p.x, t.p.y, t.p.x-v1.x*t.len, t.p.y-v1.y*t.len );
  HilbertCurve( c, ox + w * (-v1.x-v2.x), oy + w * (-v1.y-v2.y),
    w, IndexRotate(dir,cw), 1-cw, dim-1 );
}

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
  static char *color[] = {
    "yellow", "cyan", "red", "green", "magenta"
  };

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
  zxWindowSetColorByName( c, color[dim%5] );
  zxDrawFillPolygonNonConvex( c, points, 10 );

  if( flag )
    Pendant( c, ox+(1+ratio)*d[0].x, oy+(1+ratio)*d[0].y,-ratio*d[0].x,-ratio*d[0].y, dim-1, 0 );
  for( i=1; i<5; i++ )
    Pendant( c, ox+(1+ratio)*d[i].x, oy+(1+ratio)*d[i].y,-ratio*d[i].x,-ratio*d[i].y, dim-1, 0 );
}




#define DIM_BINTREE 10
#define DIM_CCURVE  10
#define DIM_DRAGON  10
#define DIM_FERN     8
#define DIM_KOCH     5
#define DIM_GASKET   5
#define DIM_HILBERT  5
#define DIM_PENDANT  5

int main(int argc, char *argv[])
{
  zxWindow w, wbintree, wccurve, wdragon, wfern, wkoch, wsierpinski, whilbert, wpendant;

  zxInit();
  zxWindowCreateAndOpen( &w, 0, 0, 1600, 800 );
  /* binary tree */
  zxWindowCreateAndOpenChild( &wbintree, &w, 0, 0, 400, 400 );
  zxWindowSetTitle( &wbintree, "Binary tree" );
  zxWindowSetColorByName( &wbintree, "lightgreen" );
  BinaryTree( &wbintree, 200, 400, 200, 200, PI/6, DIM_BINTREE );
  /* C curve */
  zxWindowCreateAndOpenChild( &wccurve, &w, 400, 0, 400, 400 );
  zxWindowSetTitle( &wccurve, "C curve" );
  zxWindowSetColorByName( &wccurve, "green" );
  CCurve( &wccurve, 100, 300, 300, 300, DIM_CCURVE );
  /* dragon curve */
  zxWindowCreateAndOpenChild( &wdragon, &w, 800, 0, 400, 400 );
  zxWindowSetTitle( &wdragon, "Dragon curve" );
  zxWindowSetColorByName( &wdragon, "orange" );
  DragonCurve( &wdragon, 50, 200, 350, 200, DIM_DRAGON );
  /* fern */
  zxWindowCreateAndOpenChild( &wfern, &w, 1200, 0, 400, 400 );
  zxWindowSetTitle( &wfern, "Fern" );
  zxWindowSetColorByName( &wfern, "lightgreen" );
  Fern( &wfern, 0, 250, 200, 50, 400, 250, DIM_FERN );
  /* Koch curve */
  sn = sin(PI/3); cs = cos(PI/3);
  zxWindowCreateAndOpenChild( &wkoch, &w, 0, 400, 400, 400 );
  zxWindowSetTitle( &wkoch, "Koch curve" );
  zxWindowSetColorByName( &wkoch, "green" );
  KochCurve( &wkoch, 0, 200, 400, 200, DIM_KOCH );
  /* Sierpinski's gasket */
  zxWindowCreateAndOpenChild( &wsierpinski, &w, 400, 400, 400, 400 );
  zxWindowSetTitle( &wsierpinski, "Sierpinski's gasket" );
  zxWindowSetColorByName( &wsierpinski, "white" );
  Sierpinski( &wsierpinski, 0, 350, 400, 350, 200, 30, DIM_GASKET );
  /* Hilbert curve */
  zxWindowCreateAndOpenChild( &whilbert, &w, 800, 400, 400, 400 );
  zxWindowSetTitle( &whilbert, "Hilbert curve" );
  zxWindowSetColorByName( &whilbert, "white" );
  HilbertCurve( &whilbert, 200, 200, 200, 0, 0, DIM_HILBERT );
  /* fractal pendant */
  ratio = 1.0/(2*cos(0.2*PI)+1);
  zxWindowCreateAndOpenChild( &wpendant, &w, 1200, 400, 400, 400 );
  zxWindowSetTitle( &wpendant, "Pendant" );
  zxWindowSetColorByName( &wpendant, "white" );
  Pendant( &wpendant, 200, 200, 0, -80, DIM_PENDANT, 1 );

  zxFlush();
  eprintf( "Hit enter key." );
  getchar();
  zxExit();
  return 1;
}

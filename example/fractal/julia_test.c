/*
 * Julia Series
 * (C)Copyright, Zhidao since 2000.
 *
 * 2001. 7.21. Created.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <zx11/zxutil.h>

#define WIDTH_X 100
#define WIDTH_Y 100

double ratio_x=1.0/WIDTH_X, ratio_y=1.0/WIDTH_Y;
int ox=WIDTH_X*1.5, oy=WIDTH_Y*1.5;
int wx=WIDTH_X*3, wy=WIDTH_Y*3;
double cx, cy;

zxWindow c;

char *color[] = {
  "green", "red", "magenta", "yellow", "blue",
  "cyan", "green", "blue", "yellow", "red", "magenta", "black"
};

#define COLOR_NUM 10

int JuliaPixel(int x, int y)
{
  double sx, sy, zx, zy;
  register int i;

  zx = ( x - ox ) * ratio_x;
  zy = ( y - oy ) * ratio_y;
  for( i=0; i<=COLOR_NUM; i++ ){
    sx = zx*zx;
    sy = zy*zy;
    if( ( sx + sy ) >= 4 ){
      zxSetColor( &c, color[i] );
      zxDrawPoint( &c, x, y );
      break;
    }
    zy = 2*zx*zy + cy;
    zx = sx - sy + cx;
  }
  return i;
}

void Julia(void)
{
  register int i, j;

  for( i=0; i<=wx; i++ )
    for( j=0; j<=wy; j++ )
      JuliaPixel( i, j );
}

int main(int argc, char *argv[])
{
  cx = argc > 2 ? atof(argv[1]) :-0.06;
  cy = argc > 3 ? atof(argv[2]) : 0.8;

  zxInit();
  zxWindowCreateAndOpen( &c, 100, 100, wx, wy );
  zxWindowSetTitle( &c, "Julia Series" );
  zxWindowClear( &c );

  Julia();
  zxFlush();
  getchar();

  zxClose();
  return 1;
}

#include <zx11/zximage.h>
#include <math.h>

#define WIDTH_X 100
#define WIDTH_Y 100

#define PI 3.14159265359

double ratio_x=1.0/WIDTH_X, ratio_y=1.0/WIDTH_Y;
int wx=WIDTH_X*3, wy=WIDTH_Y*3;

int mandelbrot_ox=WIDTH_X*2, mandelbrot_oy=WIDTH_Y*1.5;
int julia_ox=WIDTH_X*1.5, julia_oy=WIDTH_Y*1.5;
double julia_cx, julia_cy;

#define C_NUM 50
static zxPixel c_pix[C_NUM];

int mandelbrot_pixel(zxImage *img, int x, int y)
{
  double cx, cy, sx, sy, zx=0, zy=0;
  int i;

  cx = ( x - mandelbrot_ox ) * ratio_x;
  cy = ( y - mandelbrot_oy ) * ratio_y;
  for( i=0; i<C_NUM; i++ ){
    sx = zx*zx;
    sy = zy*zy;
    if( ( sx + sy ) >= 4 ) break;
    zy = 2*zx*zy + cy;
    zx = sx - sy + cx;
  }
  zxImageCellFromPixel( img, x, y, c_pix[i] );
  return i;
}

void mandelbrot_img(zxImage *img)
{
  int i, j;

  for( i=0; i<=wx; i++ )
    for( j=0; j<=wy; j++ )
      mandelbrot_pixel( img, i, j );
}

void mandelbrot_color_init(void)
{
  zxPixel r, g, b;
  zxPixelManip *pm;
  int i;
  double phase_base;

  pm = zxPixelManipDefault();
  for( i=0; i<C_NUM; i++ ){
    phase_base = (double)i/C_NUM;
    r = 0xff * exp(-phase_base*10);
    g = 0xff * 0.5*( 1 - cos( 2*PI*pow(phase_base,0.7) ) );
    b = 0xff * zMin(1.2*phase_base,1);
    c_pix[i] = pm->PixelFromRGB( r, g, b );
  }
}

void mandelbrot_draw(zxWindow *parent, int x, int y)
{
  zxWindow w;
  zxImage img;

  mandelbrot_color_init();
  zxImageAllocDefault( &img, wx+1, wy+1 );
  mandelbrot_img( &img );
  zxWindowCreateAndOpenChild( &w, parent, x, y, wx, wy );
  zxImageDrawAll( &w, &img, 0, 0 );
}

int julia_pixel(zxImage *img, int x, int y)
{
  double sx, sy, zx, zy;
  int i;

  zx = ( x - julia_ox ) * ratio_x;
  zy = ( y - julia_oy ) * ratio_y;
  for( i=0; i<C_NUM; i++ ){
    sx = zx*zx;
    sy = zy*zy;
    if( ( sx + sy ) >= 4 ){
      zxImageCellFromPixel( img, x, y, c_pix[i] );
      break;
    }
    zy = 2*zx*zy + julia_cy;
    zx = sx - sy + julia_cx;
  }
  return i;
}

void julia_img(zxImage *img)
{
  int i, j;

  for( i=0; i<=wx; i++ )
    for( j=0; j<=wy; j++ )
      julia_pixel( img, i, j );
}

void julia_color_init(double phase_offset)
{
  zxPixel r, g, b;
  zxPixelManip *pm;
  int i;
  double phase_base;

  pm = zxPixelManipDefault();
  for( i=0; i<C_NUM; i++ ){
    phase_base = 2*PI*pow((double)i/C_NUM,0.5) + phase_offset;
    r = 0xff * 0.5*( cos( phase_base          ) + 1 );
    g = 0xff * 0.5*( cos( phase_base + PI  /4 ) + 1 );
    b = 0xff * 0.5*( cos( phase_base + PI*3/4 ) + 1 );
    c_pix[i] = pm->PixelFromRGB( r, g, b );
  }
}

void julia_draw(zxWindow *parent, int x, int y, double cx, double cy)
{
  zxWindow w;
  zxImage img;

  julia_cx = cx;
  julia_cy = cy;
  julia_color_init( atan2( julia_cy, julia_cx ) );
  zxImageAllocDefault( &img, wx+1, wy+1 );
  julia_img( &img );
  zxWindowCreateAndOpenChild( &w, parent, x, y, wx, wy );
  zxImageDrawAll( &w, &img, 0, 0 );
}

int main(int argc, char *argv[])
{
  zxWindow w;

  zxInit();
  zxWindowCreateAndOpen( &w, 0, 0, wx*3, wy*3 );
  julia_draw( &w,   0,    0, -0.06, 0.8 );
  julia_draw( &w, wx  ,    0,     0, 0.8 );
  julia_draw( &w, wx*2,    0,  0.06, 0.8 );
  julia_draw( &w,    0, wy  , -0.06, 0.5 );
  mandelbrot_draw( &w, wx, wy );
  julia_draw( &w, wx*2, wy  ,  0.06,-0.5 );
  julia_draw( &w,    0, wy*2, -0.06,-0.8 );
  julia_draw( &w, wx  , wy*2,     0,-0.8 );
  julia_draw( &w, wx*2, wy*2,  0.06,-0.8 );
  zxFlush();
  getchar();
  zxExit();
  return 0;
}

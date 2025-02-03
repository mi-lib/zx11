#include <zx11/zximage.h>
#include <math.h>

void test_disk_rasterize(zxImage *img, uint x, uint y, uint r, uint g)
{
  int jmin, jmax, j, xj;
  int imin, imax, i, yi;
  ubyte gp;

  imin =-r;
  imax = r;
  for( i=imin; i<=imax; i++ ){
    jmax = sqrt( r*r - i*i );
    jmin =-jmax;
    for( j=jmin; j<=jmax; j++ ){
      if( !zxImagePosIsValid( img, ( xj = x + j ), ( yi = y + i ) ) ) continue;
      gp = zxImageCellGS( img, xj, yi );
      zxImageCellFromGS( img, xj, yi, zMax(gp,2*g+100) );
    }
  }
}

void test_bump_one(zxImage *img, uint x, uint y, uint r)
{
  int z;

  for( z=0; z<=r; z++ ){
    test_disk_rasterize( img, x, y, sqrt( r*r - z*z ), z );
  }
}

void test_bump_random(zxImage *img, int n)
{
  int i;

  for( i=0; i<n; i++ ){
    test_bump_one( img, zRandI(0,img->width-1), zRandI(0,img->height), zRandI(10,50) );
  }
}

#define WIDTH  256
#define HEIGHT 256

#define DEPTH  1.0

#define N 20

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, nvm;

  zRandInit();
  zxInit();
  zxWindowCreateAndOpen( &win, 0, 0, WIDTH * 2, HEIGHT );
  zxImageAllocDefault( &src, WIDTH, HEIGHT );
  test_bump_random( &src, N );
  zxImageAllocDefault( &nvm, src.width, src.height );
  zxImageNormalMap( &src, argc > 1 ? atof( argv[1] ) : DEPTH, &nvm );

  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageDrawAll( &win, &nvm, src.width, 0 );
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &nvm );
  zxExit();
  return 0;
} 

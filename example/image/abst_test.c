#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  register int i, j;
  zxWindow win;
  zxImage dat, rdat, gdat, bdat;
  ubyte r, g, b;
  zxPixelManip pm;

  zxInit();
  if( zxImageReadFile( &dat, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &rdat, dat.width, dat.height );
  zxImageAllocDefault( &gdat, dat.width, dat.height );
  zxImageAllocDefault( &bdat, dat.width, dat.height );

  /* original image */
  zxWindowCreateAndOpen( &win, 0, 0, dat.width*2, dat.height*2 );
  /* abstract of RGB */
  zxPixelManipSet( &pm, zxdepth );
  for( i=0; i<dat.height; i++ )
    for( j=0; j<dat.width; j++ ){
      zxImageCellRGB( &dat, &pm, j, i, &r, &g, &b );
      zxImageCellFromRGB( &rdat, &pm, j, i, r, 0, 0 );
      zxImageCellFromRGB( &gdat, &pm, j, i, 0, g, 0 );
      zxImageCellFromRGB( &bdat, &pm, j, i, 0, 0, b );
    }
  zxImageDrawAll( &win, &dat, 0, 0 );
  zxImageDrawAll( &win, &rdat, dat.width, 0 );
  zxImageDrawAll( &win, &gdat, dat.width, dat.height );
  zxImageDrawAll( &win, &bdat, 0, dat.height );
  zxFlush();
  getchar();

  zxImageDestroy( &dat );
  zxImageDestroy( &rdat );
  zxImageDestroy( &gdat );
  zxImageDestroy( &bdat );
  zxExit();
  return 0;
} 

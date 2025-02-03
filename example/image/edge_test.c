#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage src, gsc, dst;

  zxInit();
  if( zxImageReadFile( &src, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxWindowCreateAndOpen( &win, 0, 0, src.width*3, src.height*4 );
  zxImageAllocDefault( &gsc, src.width, src.height );
  zxImageAllocDefault( &dst, src.width, src.height );

  zxImageDrawAll( &win, &src, 0, 0 );
  zxImageGrayscalize( &src, &gsc );
  zxImageDrawAll( &win, &gsc, src.width, 0 );

  zxImageEdgePrewittV( &gsc, &dst );  zxImageDrawAll( &win, &dst, 0, src.height );
  zxImageEdgePrewittH( &gsc, &dst );  zxImageDrawAll( &win, &dst, src.width, src.height );
  zxImageEdgePrewitt( &gsc, &dst );   zxImageDrawAll( &win, &dst, src.width*2, src.height );

  zxImageEdgeSobelV( &gsc, &dst );    zxImageDrawAll( &win, &dst, 0, src.height*2 );
  zxImageEdgeSobelH( &gsc, &dst );    zxImageDrawAll( &win, &dst, src.width, src.height*2 );
  zxImageEdgeSobel( &gsc, &dst );     zxImageDrawAll( &win, &dst, src.width*2, src.height*2 );

  zxImageEdgeLaplacian( &gsc, &dst ); zxImageDrawAll( &win, &dst, 0, src.height*3 );
  zxImageEdgeRoberts( &gsc, &dst );   zxImageDrawAll( &win, &dst, src.width, src.height*3 );
  zxImageEdgeForsen( &gsc, &dst );    zxImageDrawAll( &win, &dst, src.width*2, src.height*3 );
  getchar();

  zxImageDestroy( &src );
  zxImageDestroy( &gsc );
  zxImageDestroy( &dst );
  zxExit();
  return 0;
} 

#include <zx11/zximage.h>

int main(int argc, char *argv[])
{
  zxWindow win;
  zxImage img_org, img_mod;
  int w, h;

  zxInit();
  if( zxImageReadFile( &img_org, argc > 1 ? argv[1] : "../fig/lena_mini.jpg" ) == 0 )
    exit( 1 );
  zxImageAllocDefault( &img_mod, ( w = img_org.width ), ( h = img_org.height ) );

  zxWindowCreateAndOpen( &win, 0, 0, w*5, h*3 );
  zxImageDrawAll( &win, &img_org, w*2, 0 );
  zxImageDrawAll( &win, &img_org, w*2, h );
  zxImageDrawAll( &win, &img_org, w*2, h*2 );

  zxImageBrighten( &img_org, &img_mod, 0.5  );    zxImageDrawAll( &win, &img_mod, 0,   0 );
  zxImageBrighten( &img_org, &img_mod, 0.75 );    zxImageDrawAll( &win, &img_mod, w,   0 );
  zxImageBrighten( &img_org, &img_mod, 1.25 );    zxImageDrawAll( &win, &img_mod, w*3, 0 );
  zxImageBrighten( &img_org, &img_mod, 1.5  );    zxImageDrawAll( &win, &img_mod, w*4, 0 );

  zxImageContrast( &img_org, &img_mod, 0.2 );     zxImageDrawAll( &win, &img_mod, 0,   h );
  zxImageContrast( &img_org, &img_mod, 0.7 );     zxImageDrawAll( &win, &img_mod, w,   h );
  zxImageContrast( &img_org, &img_mod, 1.4 );     zxImageDrawAll( &win, &img_mod, w*3, h );
  zxImageContrast( &img_org, &img_mod, 1.8 );     zxImageDrawAll( &win, &img_mod, w*4, h );

  zxImageCorrectGamma( &img_org, &img_mod, 0.2 ); zxImageDrawAll( &win, &img_mod, 0,   h*2 );
  zxImageCorrectGamma( &img_org, &img_mod, 0.7 ); zxImageDrawAll( &win, &img_mod, w,   h*2 );
  zxImageCorrectGamma( &img_org, &img_mod, 3.0 ); zxImageDrawAll( &win, &img_mod, w*3, h*2 );
  zxImageCorrectGamma( &img_org, &img_mod, 9.0 ); zxImageDrawAll( &win, &img_mod, w*4, h*2 );
  zxFlush();
  getchar();

  zxImageDestroy( &img_org );
  zxImageDestroy( &img_mod );
  zxExit();
  return 0;
} 

#include <zx11/zxinput.h>
#include <zx11/zxfont.h>
#include <unistd.h>

const char *STRING_EN = "Hello world!";
const char *STRING_JA = "こんにちは世界";

void draw(zxWindow *win, zxFTData data[])
{
  zxWindowSetColorByName( win, "dark blue" );
  zxDrawFillCircle( win, zxWindowWidth(win)/2, zxWindowHeight(win)/2, zxWindowHeight(win)/4 );
  zxFTDrawString( win, &data[0],  10, 180, STRING_EN, 0x7f, 0x7f, 0x00, 0xff );
  zxFTDrawString( win, &data[1],  30, 190, STRING_EN, 0xaf, 0xaf, 0x00, 0xff );
  zxFTDrawString( win, &data[2],  50, 200, STRING_EN, 0xff, 0xff, 0x00, 0xff );
  zxFTDrawString( win, &data[0], 110, 280, STRING_JA, 0x00, 0x7f, 0x00, 0xff );
  zxFTDrawString( win, &data[1], 130, 290, STRING_JA, 0x00, 0xaf, 0x00, 0xff );
  zxFTDrawString( win, &data[2], 150, 300, STRING_JA, 0x00, 0xff, 0x00, 0xff );
  zxFlush();
}

int main(int argc, char* argv[])
{
  zxWindow win;
  zxFTData data[3];
  bool quit_flag = false;

  zxInit();
  zxWindowCreateAndOpen( &win, 0, 0, 640, 480 );
  zxWindowKeyEnable( &win );
  zxWindowMouseEnable( &win );
  zxWindowSetTitle( &win, "Xft test" );
  zxWindowSetBGColorByName( &win, "blue" );
  zxWindowClear( &win );

  zxFTCreateSimple( &data[0], &win, NULL, 24 );
  zxFTCreate( &data[1], &win, "VL ゴシック", 24, ZXFT_LIGHT, ZXFT_ITALIC );
  zxFTCreate( &data[2], &win, "MS PGothic", 24, ZXFT_BOLD, ZXFT_OBLIQUE );
  draw( &win, data );
  while( !quit_flag ){
    switch( zxNextEvent() ){
    case Expose:
      draw( &win, data );
      break;
    case KeyPress:
      if( zxKeySymbol() == XK_q ) quit_flag = true;
      break;
    default: ;
    }
  }
  zxFTDestroy( &data[0] );
  zxFTDestroy( &data[1] );
  zxFTDestroy( &data[2] );
  zxWindowClose( &win );
  zxExit();
  return 0;
}

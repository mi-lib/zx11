#include <zx11/zxutil.h>

int main(void)
{
  zxInit();
  printf( "depth -> %d\n", zxdepth );
  printf( "screen -> %d x %d\n", zxScreenWidth(), zxScreenHeight() );
  return 0;
}

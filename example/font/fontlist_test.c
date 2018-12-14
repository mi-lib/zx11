/* X11 available font sets list */

#include <zx11/zxutil.h>

int main(void)
{
  register int i;
  char **fname;
  int n;
  XFontStruct *fs;

  zxInit();

  fname = XListFontsWithInfo( zxdisplay, "*-*", 1000, &n, &fs );
  printf( "%d font sets has been found.\n", n );
  printf( "  fname   min   max   asc   des   heig \n" );
  for( i=0; i<n; i++ )
    printf( "%s %d %d %d %d %d\n",
      fname[i],
      fs[i].min_bounds.width,
      fs[i].max_bounds.width,
      fs[i].ascent,
      fs[i].descent,
      fs[i].ascent+fs[i].descent );

  return 0;
}

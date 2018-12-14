/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxpixel - pixel conversion
 */

#include <zx11/zxpixel.h>

/* ********************************************************** */
/* CLASS: zxPixel
 * RGB pixel
 * ********************************************************** */

static void zxPixel16RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b);
static zxPixel zxPixel16FromRGB(ubyte r, ubyte g, ubyte b);
static zxPixel zxPixel16FromFRGB(float r, float g, float b);
static zxPixel zxPixel16Negate(zxPixel p);

static void zxPixel24RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b);
static zxPixel zxPixel24FromRGB(ubyte r, ubyte g, ubyte b);
static zxPixel zxPixel24FromFRGB(float r, float g, float b);
static zxPixel zxPixel24Negate(zxPixel p);

static void zxPixel32RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b);
static zxPixel zxPixel32FromRGB(ubyte r, ubyte g, ubyte b);
static zxPixel zxPixel32FromFRGB(float r, float g, float b);
static zxPixel zxPixel32Negate(zxPixel p);

/* 16 bpp case */
void zxPixel16RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b)
{
  *r = ( p >> 11 ) & 0x1f;
  *g = ( p >> 6  ) & 0x1f;
  *b = ( p       ) & 0x1f;
}

zxPixel zxPixel16FromRGB(ubyte r, ubyte g, ubyte b)
{
  return ( ( r & 0x1f ) << 11 ) | ( ( g & 0x1f ) << 6 ) | ( b & 0x1f );
}

zxPixel zxPixel16FromFRGB(float r, float g, float b)
{
  return zxPixel16FromRGB(
    (ubyte)( (double)0x1f * r ),
    (ubyte)( (double)0x1f * g ),
    (ubyte)( (double)0x1f * b ) );
}

zxPixel zxPixel16Negate(zxPixel p)
{
  return ~(p) & 0x0000;
}

/* 24 bpp case */
void zxPixel24RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b)
{
  *r = ( p >> 16 ) & 0xff;
  *g = ( p >>  8 ) & 0xff;
  *b = ( p       ) & 0xff;
}

zxPixel zxPixel24FromRGB(ubyte r, ubyte g, ubyte b)
{
  return ( r << 16 ) | ( g << 8 ) | b;
}

zxPixel zxPixel24FromFRGB(float r, float g, float b)
{
  return zxPixel24FromRGB(
    (ubyte)( (double)0xff * r ),
    (ubyte)( (double)0xff * g ),
    (ubyte)( (double)0xff * b ) );
}

zxPixel zxPixel24Negate(zxPixel p)
{
  return ~(p) & 0x00ffffff;
}

/* 32 bpp case */
void zxPixel32RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b)
{
  *r = ( p >> 24 ) & 0xff;
  *g = ( p >> 16 ) & 0xff;
  *b = ( p >>  8 ) & 0xff;
}

zxPixel zxPixel32FromRGB(ubyte r, ubyte g, ubyte b)
{
  return ( r << 24 ) | ( g << 16 ) | ( b << 8 ) | 0xff;
}

zxPixel zxPixel32FromFRGB(float r, float g, float b)
{
  return zxPixel32FromRGB(
    (ubyte)( (double)0xff * r ),
    (ubyte)( (double)0xff * g ),
    (ubyte)( (double)0xff * b ) );
}

zxPixel zxPixel32Negate(zxPixel p)
{
  return ( ~(p) & 0xffffff00 ) | ( (p) & 0xff );
}

/* pixel manipulator */

zxPixelManip *zxPixelManipSet(zxPixelManip *pm, int depth)
{
  switch( ( pm->depth = depth ) ){
  case 16:
    pm->PixelRGB = zxPixel16RGB;
    pm->PixelFromRGB = zxPixel16FromRGB;
    pm->PixelFromFRGB = zxPixel16FromFRGB;
    pm->PixelNegate = zxPixel16Negate;
    break;
  case 24:
    pm->PixelRGB = zxPixel24RGB;
    pm->PixelFromRGB = zxPixel24FromRGB;
    pm->PixelFromFRGB = zxPixel24FromFRGB;
    pm->PixelNegate = zxPixel24Negate;
    break;
  case 32:
    pm->PixelRGB = zxPixel32RGB;
    pm->PixelFromRGB = zxPixel32FromRGB;
    pm->PixelFromFRGB = zxPixel32FromFRGB;
    pm->PixelNegate = zxPixel32Negate;
    break;
  case 8 :
  default:
    /* functions are less significant, because most of
     * 8 bit graphics systems prepare 'Pseudo Color' mode.
     */
    pm->PixelRGB = NULL;
    pm->PixelFromRGB = NULL;
    pm->PixelFromFRGB = NULL;
    pm->PixelNegate = NULL;
  }
  return pm;
}

zxPixel zxPixelConv(zxPixel pixel, zxPixelManip *src, zxPixelManip *dest)
{
  ubyte r, g, b;

  if( src->PixelRGB == dest->PixelRGB ) return pixel;
  src->PixelRGB( pixel, &r, &g, &b );
  if( src->depth == 16 && ( dest->depth == 24 || dest->depth == 32 ) ){
    r = ( r << 3 ) | ( r >> 2 );
    g = ( g << 3 ) | ( g >> 2 );
    b = ( b << 3 ) | ( b >> 2 );
  } else
  if( ( src->depth == 24 || src->depth == 32 ) && dest->depth == 16 ){
    r >>= 3;
    g >>= 3;
    b >>= 3;
  }
  return dest->PixelFromRGB( r, g, b );
}

zxPixel zxPixelBlend(zxPixelManip *pm, zxPixel p[], double w[], uint n)
{
  register uint i;
  ubyte r, g, b;
  double rt, gt, bt;

  for( rt=gt=bt=0, i=0; i<n; i++ ){
    pm->PixelRGB( p[i], &r, &g, &b );
    rt += (double)r * w[i];
    gt += (double)g * w[i];
    bt += (double)b * w[i];
  }
  return pm->PixelFromRGB( (zxPixel)rt, (zxPixel)gt, (zxPixel)bt );
}

zxPixel zxPixelAlphaBlend(zxPixelManip *pm, zxPixel p1, zxPixel p2, double alpha)
{
  ubyte r1, g1, b1;
  ubyte r2, g2, b2;
  double r, g, b;

  pm->PixelRGB( p1, &r1, &g1, &b1 );
  pm->PixelRGB( p2, &r2, &g2, &b2 );
  r = r1 + alpha * ( r2 - r1 );
  g = g1 + alpha * ( g2 - g1 );
  b = b1 + alpha * ( b2 - b1 );
  return pm->PixelFromRGB( r, g, b );
}

zxPixel zxPixelFromGS(zxPixelManip *pm, ubyte intensity)
{
  return pm->PixelFromRGB( intensity, intensity, intensity );
}

zxPixel zxPixelFromFGS(zxPixelManip *pm, float intensity)
{
  return pm->PixelFromFRGB( intensity, intensity, intensity );
}

/* strings -> pixel conversion */

zxPixel zxPixelFromHex(char *hex)
{
  zxPixel pixel;

  sscanf( hex, "%x", &pixel );
  return pixel;
}

zxPixel zxPixelFromStr(zxWindow *win, char *str)
{
  if( zStrIsHex( str ) ) return zxPixelFromHex( str );
  return zxGetColor( win, str );
}

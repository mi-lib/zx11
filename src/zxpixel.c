/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zxpixel - pixel conversion
 */

#include <zx11/zxpixel.h>
#include <math.h>

#define _ZXPIXEL16MAX 0x1f
#define _ZXPIXEL24MAX 0xff
#define _ZXPIXEL32MAX 0xff

/* 16 bpp case */
static void _zxPixel16RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b)
{
  *r = ( p >> 11 ) & _ZXPIXEL16MAX;
  *g = ( p >> 6  ) & _ZXPIXEL16MAX;
  *b = ( p       ) & _ZXPIXEL16MAX;
}

static void _zxPixel16FRGB(zxPixel p, float *r, float *g, float *b)
{
  ubyte _r, _g, _b;

  _zxPixel16RGB( p, &_r, &_g, &_b );
  *r = (double)_r / (double)_ZXPIXEL16MAX;
  *g = (double)_g / (double)_ZXPIXEL16MAX;
  *b = (double)_b / (double)_ZXPIXEL16MAX;
}

static zxPixel _zxPixel16FromRGB(ubyte r, ubyte g, ubyte b)
{
  return ( ( r & _ZXPIXEL16MAX ) << 11 ) | ( ( g & _ZXPIXEL16MAX ) << 6 ) | ( b & _ZXPIXEL16MAX );
}

static zxPixel _zxPixel16FromFRGB(float r, float g, float b)
{
  return _zxPixel16FromRGB(
    (ubyte)( (double)_ZXPIXEL16MAX * r ),
    (ubyte)( (double)_ZXPIXEL16MAX * g ),
    (ubyte)( (double)_ZXPIXEL16MAX * b ) );
}

static zxPixel _zxPixel16Negate(zxPixel p)
{
  return ~(p) & 0xffff;
}

/* 24 bpp case */
static void _zxPixel24RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b)
{
  *r = ( p >> 16 ) & _ZXPIXEL24MAX;
  *g = ( p >>  8 ) & _ZXPIXEL24MAX;
  *b = ( p       ) & _ZXPIXEL24MAX;
}

static void _zxPixel24FRGB(zxPixel p, float *r, float *g, float *b)
{
  ubyte _r, _g, _b;

  _zxPixel24RGB( p, &_r, &_g, &_b );
  *r = (double)_r / (double)_ZXPIXEL24MAX;
  *g = (double)_g / (double)_ZXPIXEL24MAX;
  *b = (double)_b / (double)_ZXPIXEL24MAX;
}

static zxPixel _zxPixel24FromRGB(ubyte r, ubyte g, ubyte b)
{
  return ( r << 16 ) | ( g << 8 ) | b;
}

static zxPixel _zxPixel24FromFRGB(float r, float g, float b)
{
  return _zxPixel24FromRGB(
    (ubyte)( (double)_ZXPIXEL24MAX * r ),
    (ubyte)( (double)_ZXPIXEL24MAX * g ),
    (ubyte)( (double)_ZXPIXEL24MAX * b ) );
}

static zxPixel _zxPixel24Negate(zxPixel p)
{
  return ~(p) & 0x00ffffff;
}

/* 32 bpp case */
static void _zxPixel32RGB(zxPixel p, ubyte *r, ubyte *g, ubyte *b)
{
  *r = ( p >> 24 ) & _ZXPIXEL32MAX;
  *g = ( p >> 16 ) & _ZXPIXEL32MAX;
  *b = ( p >>  8 ) & _ZXPIXEL32MAX;
}

static void _zxPixel32FRGB(zxPixel p, float *r, float *g, float *b)
{
  ubyte _r, _g, _b;

  _zxPixel32RGB( p, &_r, &_g, &_b );
  *r = (double)_r / (double)_ZXPIXEL32MAX;
  *g = (double)_g / (double)_ZXPIXEL32MAX;
  *b = (double)_b / (double)_ZXPIXEL32MAX;
}

static zxPixel _zxPixel32FromRGB(ubyte r, ubyte g, ubyte b)
{
  return ( r << 24 ) | ( g << 16 ) | ( b << 8 ) | _ZXPIXEL32MAX;;
}

static zxPixel _zxPixel32FromFRGB(float r, float g, float b)
{
  return _zxPixel32FromRGB(
    (ubyte)( (double)_ZXPIXEL32MAX * r ),
    (ubyte)( (double)_ZXPIXEL32MAX * g ),
    (ubyte)( (double)_ZXPIXEL32MAX * b ) );
}

static zxPixel _zxPixel32Negate(zxPixel p)
{
  return ( ~(p) & 0xffffff00 ) | ( (p) & _ZXPIXEL32MAX );
}

/* pixel manipulator */

static zxPixelManip __zxpixelmanip16 = {
  depth: 16,
  PixelRGB: _zxPixel16RGB,
  PixelFRGB: _zxPixel16FRGB,
  PixelFromRGB: _zxPixel16FromRGB,
  PixelFromFRGB: _zxPixel16FromFRGB,
  PixelNegate: _zxPixel16Negate,
};

static zxPixelManip __zxpixelmanip24 = {
  depth: 24,
  PixelRGB: _zxPixel24RGB,
  PixelFRGB: _zxPixel24FRGB,
  PixelFromRGB: _zxPixel24FromRGB,
  PixelFromFRGB: _zxPixel24FromFRGB,
  PixelNegate: _zxPixel24Negate,
};

static zxPixelManip __zxpixelmanip32 = {
  depth: 32,
  PixelRGB: _zxPixel32RGB,
  PixelFRGB: _zxPixel32FRGB,
  PixelFromRGB: _zxPixel32FromRGB,
  PixelFromFRGB: _zxPixel32FromFRGB,
  PixelNegate: _zxPixel32Negate,
};

zxPixelManip *zxPixelManipFind(int depth)
{
  switch( depth ){
  case 16: return &__zxpixelmanip16;
  case 24: return &__zxpixelmanip24;
  case 32: return &__zxpixelmanip32;
  default: ZRUNERROR( "unknown depth" );
  }
  return NULL;
}

zxPixel zxPixelConv(zxPixel pixel, zxPixelManip *src, zxPixelManip *dest)
{
  ubyte r, g, b;

  if( src->depth == dest->depth ) return pixel;
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
  uint i;
  ubyte r, g, b;
  double rt, gt, bt;

  for( rt=gt=bt=0, i=0; i<n; i++ ){
    pm->PixelRGB( p[i], &r, &g, &b );
    rt += (double)r * w[i];
    gt += (double)g * w[i];
    bt += (double)b * w[i];
  }
  return pm->PixelFromRGB( (zxPixel)zLimit(rt,0,0xff), (zxPixel)zLimit(gt,0,0xff), (zxPixel)zLimit(bt,0,0xff) );
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

zxPixel zxPixelNorm2(zxPixelManip *pm, zxPixel p1, zxPixel p2)
{
  ubyte r1, g1, b1;
  ubyte r2, g2, b2;
  double r, g, b;

  pm->PixelRGB( p1, &r1, &g1, &b1 );
  pm->PixelRGB( p2, &r2, &g2, &b2 );
  r = sqrt( r1*r1 + r2*r2 );
  g = sqrt( g1*g1 + g2*g2 );
  b = sqrt( b1*b1 + b2*b2 );
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

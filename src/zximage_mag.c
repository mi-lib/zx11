/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_mag - MAG (MA_KIchan Graphic format) support.
 */

#include <zx11/zximage_mag.h>

bool zxImageFileIsMAG(char filename[])
{
  return zxImageFileIdent( filename, (const unsigned char *)"MAKI", 4 );
}

typedef struct {
  /* explicit parameters */
  uint8_t top;          /* top of the header */
  uint8_t mcncode;      /* machine code */
  uint8_t mcnflag;      /* machine flag */
  uint8_t scrmode;      /* screen mode */
  uint16_t xmin;        /* minimum boundary of x*/
  uint16_t ymin;        /* minimum boundary of y */
  uint16_t xmax;        /* minimum boundary of x*/
  uint16_t ymax;        /* minimum boundary of y */
  uint32_t aoffset;     /* offset to flag A */
  uint32_t boffset;     /* offset to flag B */
  uint32_t bsize;       /* size of flag B */
  uint32_t pixeloffset; /* offset to pixel data */
  uint32_t pixelsize;   /* size of pixel data */

  /* implicit parameters */
  FILE  *fp;            /* file pointer */
  uint32_t head;        /* offset to the header */
  uint32_t asize;       /* size of flag A */
  uint32_t depth;       /* original image depth (16 or 256) */
  uint32_t bpl;         /* byte per line (w[dots]/8 or w[dots]/4) */
  uint32_t dpp;         /* dot per pixel  ( 4 or 2 ) */
  uint32_t palettesize; /* size of palette data (16 or 256) */
  zxPixel *palette;     /* palette */
  uint8_t *flag;        /* flag line */

  /* flag A */
  uint32_t flag_a_pos;
  uint8_t flag_a_buf, flag_a_mask;
  /* flag B */
  uint32_t flag_b_pos;
  /* pixel */
  uint32_t pixel_pos;
} zxMAGInfo;

static char _zxMAGReadByte(FILE *fp)
{
  if( feof( fp ) ){
    ZRUNERROR( "unexpected termination in MAG file" );
    return 0;
  }
  return fgetc( fp );
}

static int _zxMAGReadData(FILE *fp, char buf[], int len)
{
  int i;

  for( i=0; i<len; i++ )
    buf[i] = _zxMAGReadByte( fp );
  return len;
}

static int _zxMAGReadString(FILE *fp, char buf[], int len)
{
  _zxMAGReadData( fp, buf, len );
  buf[len] = '\0';
  return len;
}

int zxMAGDispComment(FILE *fp)
{
  char buf[BUFSIZ];
  char c;

  _zxMAGReadString( fp, buf, 8 ); /* check data */
  if( strncmp( buf, "MAKI", 4 ) ){
    ZRUNERROR( "this is not a MAG image" );
    return 0;
  }
  printf( "%s\n", buf );
  _zxMAGReadString( fp, buf, 4 ); /* machine code */
  printf( "%s\n", buf );
  _zxMAGReadString( fp, buf, 18 ); /* user name */
  printf( "%s\n", buf );
  while( 1 ){ /* memo */
    c = _zxMAGReadByte( fp );
    if( c == 0x1a ) break; /* eof */
    printf( "%c", c );
  }
  printf( "\n" );
  return 1;
}

int zxMAGDispCommentFile(const char filename[])
{
  FILE *fp;
  int result;

  if( !( fp = fopen( filename, "rb" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  result = zxMAGDispComment( fp );
  fclose( fp );
  return result;
}

static int _zxMAGReadComment(FILE *fp)
{
  char buf[BUFSIZ];
  char c;

  _zxMAGReadString( fp, buf, 8 ); /* check data */
  if( strncmp( buf, "MAKI", 4 ) ){
    ZRUNERROR( "this is not a MAG image" );
    return 0;
  }
  _zxMAGReadString( fp, buf, 4 ); /* machine code */
  _zxMAGReadString( fp, buf, 18 ); /* user name */
  while( 1 ){ /* memo */
    c = _zxMAGReadByte( fp );
    if( c == 0x1a ) break; /* eof */
  }
  return 1;
}

static int _zxMAGReadHeader(zxMAGInfo *info)
{
  info->head = ftell( info->fp );
  _zxMAGReadData( info->fp, (char *)info, 32 );
  info->asize = info->boffset - info->aoffset;
  info->palettesize = ( info->aoffset - 32 ) / 3;

  return 1;
}

static int _zxMAGReadPalette(zxMAGInfo *info)
{
  uint32_t i;
  uint8_t r, g, b;
  zxPixelManip src, dest;

  switch( info->palettesize ){
  case 16:
    info->depth = 16;
    info->dpp   = 4;
    break;
  case 256:
    info->depth = 24;
    info->dpp   = 2;
    break;
  default:
    ZRUNERROR( "unsupported depth" );
    return 0;
  }
  info->bpl = ( info->xmax - info->xmin + 1 ) / info->dpp / 2;
  info->palette = zAlloc( zxPixel, info->palettesize );

  if( info->palette == NULL ){
    ZRUNERROR( "not enough memory for palette" );
    return 0;
  }
  zxPixelManipSet( &src, info->depth );
  zxPixelManipSetDefault( &dest );
  for( i=0; i<info->palettesize; i++ ){
    _zxMAGReadData( info->fp, (char *)&g, 1 );
    _zxMAGReadData( info->fp, (char *)&r, 1 );
    _zxMAGReadData( info->fp, (char *)&b, 1 );
    if( info->depth == 16 ){
      r >>= 3;
      g >>= 3;
      b >>= 3;
    }
    info->palette[i] = zxPixelConv( src.PixelFromRGB(r,g,b),
      &src, &dest );
  }
  return 1;
}

static uint8_t _zxMAGReadFlagA(zxMAGInfo *info)
{
  uint8_t flag;

  if( info->flag_a_mask == 0 ){
    fseek( info->fp,
      info->head+info->aoffset+info->flag_a_pos, SEEK_SET );
    info->flag_a_buf = _zxMAGReadByte( info->fp );
    info->flag_a_mask = 0x80; /* MSB on */
    if( ++info->flag_a_pos > info->asize ){
      ZRUNERROR( "inconsistent flag A" );
      return 0;
    }
  }
  flag = info->flag_a_buf & info->flag_a_mask ? 1 : 0;
  info->flag_a_mask >>= 1;
  return flag;
}

static uint8_t _zxMAGReadFlagB(zxMAGInfo *info)
{
  fseek( info->fp, info->head+info->boffset+info->flag_b_pos, SEEK_SET );
  if( ++info->flag_b_pos > info->bsize ){
    ZRUNERROR( "inconsistent flag B" );
    return 0;
  }
  return _zxMAGReadByte( info->fp );
}

static uint8_t _zxMAGReadFlag(zxMAGInfo *info, int i, int line)
{
  uint8_t b;

  b = _zxMAGReadFlagA( info ) ? _zxMAGReadFlagB( info ) : 0;
  info->flag[i] = ( line == 0 ) ? b : info->flag[i] ^ b;
  return info->flag[i];
}

static uint16_t _zxMAGReadPixel(zxMAGInfo *info)
{
  uint16_t buf;

  fseek( info->fp,
    info->head+info->pixeloffset+info->pixel_pos, SEEK_SET );
  if( fread( &buf, 2, 1, info->fp ) != 1 );
  if( ( info->pixel_pos += 2 ) > info->pixelsize ){
    ZRUNERROR( "inconsistent pixel size" );
    return 0;
  }
  return buf;
}

static void _zxImageSetPixelMAG(zxImage *img, zxMAGInfo *info, int x, int y, uint16_t pixelid)
{
  int i, j;
  uint8_t msb, lsb, dpb /* dot per byte */;

  lsb =   pixelid        & 0xff;
  msb = ( pixelid >> 8 ) & 0xff;
  dpb = info->dpp / 2;
  j = x*info->dpp - 1;
  for( i=0; i<dpb; i++ ){
    zxImageCellFromPixel( img, j-i+dpb,       y,
      info->palette[lsb % info->palettesize] );
    lsb >>= ( 16 / info->dpp );
    zxImageCellFromPixel( img, j-i+info->dpp, y,
      info->palette[msb % info->palettesize] );
    msb >>= ( 16 / info->dpp );
  }
}

static void _zxImageCopyPixelMAG(zxImage *img, zxMAGInfo *info, int x, int y, uint8_t flag)
{
  int sx, sy;
  struct {
    int dx, dy;
  } d[] = {
    { 0,  0 }, { 1, 0 }, { 2, 0 }, { 4, 0 },
    { 0,  1 }, { 1, 1 },
    { 0,  2 }, { 1, 2 }, { 2, 2 },
    { 0,  4 }, { 1, 4 }, { 2, 4 },
    { 0,  8 }, { 1, 8 }, { 2, 8 },
    { 0, 16 },
  };

  sx = x - d[flag].dx;
  sy = y - d[flag].dy;
  memcpy( zxImageAddr(img,x*info->dpp,y),
    zxImageAddr(img,sx*info->dpp,sy),
    img->bpp*info->dpp );
}

static int _zxImageReadPixelMAG(zxImage *img, zxMAGInfo *info)
{
  uint8_t flag, msb, lsb;
  uint i, j;

  info->flag = zAlloc( uint8_t, info->bpl );
  if( info->flag == NULL ){
    ZRUNERROR( "not enough memory for flag buffer" );
    return 0;
  }
  info->flag_a_pos = 0;
  info->flag_a_buf = 0;
  info->flag_a_mask = 0;
  info->flag_b_pos = 0;
  info->pixel_pos = 0;
  for( i=0; i<img->height; i++ ){
    for( j=0; j<info->bpl; j++ ){
      flag = _zxMAGReadFlag( info, j, i );
      msb = ( flag >> 4 ) & 0xf;
      lsb =   flag        & 0xf;
      if( msb == 0 )
        _zxImageSetPixelMAG( img, info, j*2, i, _zxMAGReadPixel(info) );
      else
        _zxImageCopyPixelMAG( img, info, j*2, i, msb );
      if( lsb == 0 )
        _zxImageSetPixelMAG( img, info, j*2+1, i, _zxMAGReadPixel(info) );
      else
        _zxImageCopyPixelMAG( img, info, j*2+1, i, lsb );
    }
  }
  return 1;
}

int zxImageReadMAG(FILE *fp, zxImage *img)
{
  zxMAGInfo info;

  info.fp = fp;
  _zxMAGReadComment( info.fp );
  _zxMAGReadHeader( &info );
  if( _zxMAGReadPalette( &info ) ){
    if( zxImageAllocDefault( img,
      info.xmax-info.xmin+1, info.ymax-info.ymin+1 ) ){
      if( _zxImageReadPixelMAG( img, &info ) )
        free( info.flag );
    }
    free( info.palette );
  }
  return 1;
}

int zxImageReadMAGFile(zxImage *img, char filename[])
{
  FILE *fp;

  fp = fopen( filename, "rb" );
  if( fp == NULL ){
    ZOPENERROR( filename );
    return 0;
  }
  zxImageReadMAG( fp, img );
  fclose( fp );
  return 1;
}

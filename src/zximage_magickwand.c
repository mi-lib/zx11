/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_magickwand - MagickWand (C API of ImageMagick) support.
 */

#include <zx11/zximage_magickwand.h>

static void _MagickWandGenesis(void)
{
  if( !IsMagickWandInstantiated() )
    MagickWandGenesis();
}

static void _MagickWandThrowException(MagickWand *wand)
{
  char *description;
  ExceptionType severity;

  description = MagickGetException( wand, &severity );
  eprintf( "%s %s %lu %s\n", GetMagickModule(), description );
  MagickRelinquishMemory( description );
}

static MagickPixelPacket *_MagickPixelPacketFromNormalizedFloat(MagickPixelPacket *pixel, float red, float green, float blue)
{
  pixel->red   = 0xffff * red;
  pixel->green = 0xffff * green;
  pixel->blue  = 0xffff * blue;
  return pixel;
}

zxImage *zxImageFromMagickWand(zxImage *img, MagickWand *wand)
{
  MagickPixelPacket pixel;
  PixelIterator *iterator;
  PixelWand **pixels;
  unsigned long int w;
  long int i, j;
  zxPixelManip pm;

  _MagickWandGenesis();
  if( !zxImageAllocDefault( img, MagickGetImageWidth(wand), MagickGetImageHeight(wand) ) )
    return NULL;
  zxPixelManipSetDefault( &pm );
  if( !( iterator = NewPixelIterator( wand ) ) )
    _MagickWandThrowException( wand );
  for( i=0; i<img->height; i++ ){
    if( !( pixels = PixelGetNextIteratorRow( iterator, &w ) ) || w != img->width ){
      _MagickWandThrowException( wand );
      break;
    }
    for( j=0; j<img->width; j++ ){
      PixelGetMagickColor( pixels[j], &pixel );
      zxImageCellFromRGB( img, &pm, j, i, pixel.red, pixel.green, pixel.blue );
    }
    PixelSyncIterator( iterator );
  }
  DestroyPixelIterator( iterator );
  return img;
}

MagickWand *zxImageToMagickWand(zxImage *img, MagickWand *wand)
{
  MagickPixelPacket pixel;
  PixelIterator *iterator;
  PixelWand *dummy_pixel;
  PixelWand **pixels;
  zxPixelManip pm;
  long int i, j;
  unsigned long int w;
  float r, g, b;

  _MagickWandGenesis();
  if( !( dummy_pixel = NewPixelWand() ) ){
    _MagickWandThrowException( wand );
    return NULL;
  }
  MagickNewImage( wand, img->width, img->height, dummy_pixel );
  DestroyPixelWand( dummy_pixel );
  if( !( iterator = NewPixelIterator( wand ) ) ){
    _MagickWandThrowException( wand );
    return NULL;
  }
  zxPixelManipSetDefault( &pm );
  for( i=0; i<img->height; i++ ){
    if( !( pixels = PixelGetNextIteratorRow( iterator, &w ) ) || w != img->width ) break;
    for( j=0; j<img->width; j++ ){
      zxImageCellFRGB( img, &pm, j, i, &r, &g, &b );
      PixelSetMagickColor( pixels[j], _MagickPixelPacketFromNormalizedFloat( &pixel, r, g, b ) );
    }
    PixelSyncIterator( iterator );
  }
  DestroyPixelIterator( iterator );
  return wand;
}

int zxImageReadFileMagickWand(zxImage *img, const char *filename)
{
  MagickWand *wand;

  _MagickWandGenesis();
  wand = NewMagickWand();
  if( MagickReadImage( wand, filename ) == MagickFalse ){
    _MagickWandThrowException( wand );
    return 0;
  }
  img = zxImageFromMagickWand( img, wand );
  DestroyMagickWand( wand );
  return 1;
}

int zxImageWriteFileMagickWand(zxImage *img, const char *filename)
{
  MagickWand *wand;
  int retval = 1;

  _MagickWandGenesis();
  if( !( wand = NewMagickWand() ) )
    _MagickWandThrowException( wand );
  if( !zxImageToMagickWand( img, wand ) || MagickWriteImages( wand, filename, MagickTrue ) == MagickFalse ){
    _MagickWandThrowException( wand );
    retval = 0;
  }
  DestroyMagickWand( wand );
  return retval;
}

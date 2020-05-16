/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_tiff - TIFF (Tag Image File Format) support on zxImage (requires libtiff).
 */

#include <zx11/zximage_tiff.h>

bool zxImageFileIsTIFF(char filename[])
{
#define ZX_TIFF_IDENT_SIZE 4
  const char __zx_tiff_ident1[] = {
    0x4d, 0x4d, 0x00, 0x2a,
  };
  const char __zx_tiff_ident2[] = {
    0x49, 0x49, 0x2a, 0x00,
  };
  return zxImageFileIdent( filename, __zx_tiff_ident1, ZX_TIFF_IDENT_SIZE ) ||
         zxImageFileIdent( filename, __zx_tiff_ident2, ZX_TIFF_IDENT_SIZE );
}

#define zxTIFFError(tiff,msg) do{\
  ZRUNERROR( msg );\
  TIFFClose( tiff );\
  return 0;\
} while(0)

int zxImageReadTIFFFile(zxImage *img, char filename[])
{
  TIFF *tiff;
  uint length, width;
  ubyte bps, spp;
  uint *buf, p;
  ubyte r, g, b;
  zxPixelManip pm;
  register int i, j;
  int ret = 1;

  if( !( tiff = TIFFOpen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  if( !TIFFGetField( tiff, TIFFTAG_IMAGELENGTH, &length ) )
    zxTIFFError( tiff, "invalid image length" );
  if( !TIFFGetField( tiff, TIFFTAG_IMAGEWIDTH, &width ) )
    zxTIFFError( tiff, "invalid image width" );
  if( !TIFFGetField( tiff, TIFFTAG_BITSPERSAMPLE, &bps ) || bps != 8 )
    zxTIFFError( tiff, "invalid bit-per-sample" );
  if( !TIFFGetField( tiff, TIFFTAG_SAMPLESPERPIXEL, &spp ) )
    zxTIFFError( tiff, "invalid sample-per-pixel" );

  if( !( buf = zAlloc( uint, width*length ) ) )
    zxTIFFError( tiff, "bad memory allocation" );
  if( !TIFFReadRGBAImage( tiff, width, length, buf, 0 ) ){
    ZRUNERROR( "cannot read TIFF image");
    ret = 0;
    goto TERMINATE;
  }
  zxImageAllocDefault( img, width, length );
  zxPixelManipSetDefault( &pm );
  for( i=0; i<length; i++ )
    for( j=0; j<width; j++ ){
      p = buf[ width * (length - 1 - i) + j ];
      r = TIFFGetR( p );
      g = TIFFGetG( p );
      b = TIFFGetB( p );
      zxImageCellFromRGB( img, &pm, j, i, r, g, b );
    }
 TERMINATE:
  free( buf );
  TIFFClose( tiff );
  return ret;
}

int zxImageWriteTIFFFile(zxImage *img, char filename[], int cmpmethod)
{
  TIFF *tiff;
  uint bps = 8;
  uint spp = 3;
  ubyte *buf, *p;
  register int i, j;
  zxPixelManip pm;
  int ret = 1;

  if( !( tiff = TIFFOpen( filename, "w" ) ) ){
    ZOPENERROR( filename );
    return 0;
  }
  if( !TIFFSetField( tiff, TIFFTAG_COMPRESSION, cmpmethod ) )
    zxTIFFError( tiff, "invalid compression method" );
  if( !TIFFSetField( tiff, TIFFTAG_IMAGEWIDTH, img->width ) )
    zxTIFFError( tiff, "invalid image width" );
  if( !TIFFSetField( tiff, TIFFTAG_IMAGELENGTH, img->height ) )
    zxTIFFError( tiff, "invalid image length" );
  if( !TIFFSetField( tiff, TIFFTAG_BITSPERSAMPLE, bps ) )
    zxTIFFError( tiff, "invalid bit-per-sample" );
  if( !TIFFSetField( tiff, TIFFTAG_SAMPLESPERPIXEL, spp ) )
    zxTIFFError( tiff, "invalid sample-per-pixel" );
  if( !TIFFSetField( tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB ) )
    zxTIFFError( tiff, "invalid photometric" );
  if( !TIFFSetField( tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG ) )
    zxTIFFError( tiff, "invalid planarconfiguration" );
  if( !TIFFSetField( tiff, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB ) )
    zxTIFFError( tiff, "invalid fill order" );
  if( !TIFFSetField( tiff, TIFFTAG_XRESOLUTION, ZXTIFF_RESOLUTION_DEFAULT ) )
    zxTIFFError( tiff, "invalid x-resolution" );
  if( !TIFFSetField( tiff, TIFFTAG_YRESOLUTION, ZXTIFF_RESOLUTION_DEFAULT ) )
    zxTIFFError( tiff, "invalid y-resolution" );
  if( !TIFFSetField( tiff, TIFFTAG_ROWSPERSTRIP, img->height ) )
    zxTIFFError( tiff, "invalid row-per-strip" );

  if( !( buf = zAlloc( ubyte, img->width*img->height*3 ) ) )
    zxTIFFError( tiff, "bad memory allocation" );
  zxPixelManipSetDefault( &pm );
  for( p=buf, i=0; i<img->height; i++ )
    for( j=0; j<img->width; j++, p+=3 )
      zxImageCellRGB( img, &pm, j, i, p, p+1, p+2 );
  if( TIFFWriteEncodedStrip( tiff, 0, buf, img->width * img->height * 3 ) < 0 ){
    ZRUNERROR( "failed to encode TIFF" );
    ret = 0;
  }
  free( buf );
  TIFFClose( tiff );
  return ret;
}

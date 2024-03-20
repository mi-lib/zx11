/* ZX11 - a wrapper for interface library to the X Window System
 * Copyright (C) 1999 Tomomichi Sugihara (Zhidao)
 *
 * zximage_freetype - Freetype2 support.
 */

#include <zx11/zximage.h>
#include <zm/zm.h>

static FT_Library zxfreetypelibrary;

bool zxFTImageInitLibrary(void)
{
  FT_Error error;

  if( ( error = FT_Init_FreeType( &zxfreetypelibrary ) ) ){
    ZRUNERROR( "error code=%d: cannot initialize Freetype font library", error );
    return false;
  }
  return true;
}

void zxFTImageExitLibrary(void)
{
  FT_Done_FreeType( zxfreetypelibrary );
}

void zxFTImageSetCoord(zxFTImage *font, double x, double y, double angle)
{
  /* rotation */
  angle = zDeg2Rad( angle ); /* convert from degree to radian */
  font->matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  font->matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  font->matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  font->matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
  /* translation */
  font->pen.x = x * 64;
  font->pen.y = y * 64;
}

bool zxFTImageLoadFont(zxFTImage *font, const char *fontfilename, uint size, uint dpi)
{
  FT_Error error;

  /* create face object */
  if( ( error = FT_New_Face( zxfreetypelibrary, fontfilename, 0, &font->face ) ) ){
    ZRUNERROR( "error code=%d: cannot create new font face", error );
    return false;
  }
  /* set character size at specified dpi */
  if( ( error = FT_Set_Char_Size( font->face, size * 64, 0, dpi, 0 ) ) ){
    ZRUNERROR( "error code=%d: failed to set font size and DPI", error );
    return false;
  }
  zxFTImageSetCoord( font, 0, 0, 0 );
  return true;
}

void zxFTImageUnloadFont(zxFTImage *font)
{
  FT_Done_Face( font->face );
}

static void _zxFTImagePutChar(zxImage *img, zxPixelManip *pm, zxFTImage *font, uword c, float red, float green, float blue)
{
  FT_GlyphSlot slot;
  FT_UInt i, j, x, y;
  ubyte val;

  slot = font->face->glyph;
  if( FT_Load_Char( font->face, c, FT_LOAD_RENDER ) )
    return; /* ignore errors */
  for( i=0; i<slot->bitmap.rows; i++ ){
    for( j=0; j<slot->bitmap.width; j++ ){
      x = slot->bitmap_left + j;
      y = img->height - slot->bitmap_top + i;
      if( j >= img->width || i >= img->height ) continue;
      val = slot->bitmap.buffer[i*slot->bitmap.width+j];
      if( val > 0 ){
        zxImageCellFromFRGB( img, pm, x, y, red*val/(float)0xff, green*val/(float)0xff, blue*val/(float)0xff );
        img->mask_buf ? zxImageSetMask( img, x, y, 0xff ) : 0;
      }
    }
  }
}

#define _zxFTImageAdvance(font) do{\
  (font)->pen.x += (font)->face->glyph->advance.x;\
  (font)->pen.y += (font)->face->glyph->advance.y;\
} while(0)

void zxImageDrawString(zxImage *img, zxFTImage *font, int x, int y, const uword *string, float red, float green, float blue)
{
  zxPixelManip pm;
  uword *cp;

  zxPixelManipSetDefault( &pm );
  zxFTImageSetCoord( font, x, y, 0 );
  for( cp=(uword *)string; *cp; cp++ ){
    FT_Set_Transform( font->face, &font->matrix, &font->pen );
    _zxFTImagePutChar( img, &pm, font, *cp, red, green, blue );
    _zxFTImageAdvance( font );
  }
}

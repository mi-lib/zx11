#include <zx11/zximage.h>

void assert_bitmap_size(void)
{
  zBitmap bitmap;
  bool result;

  zBitmapAlloc( &bitmap, 7, 3 );
  result = bitmap.byte_per_line == 1;
  zBitmapDestroy( &bitmap );
  zAssert( zBitmapAlloc (7 bits), result );
  zBitmapAlloc( &bitmap, 8, 3 );
  result = bitmap.byte_per_line == 1;
  zBitmapDestroy( &bitmap );
  zAssert( zBitmapAlloc (8 bits), result );
  zBitmapAlloc( &bitmap, 9, 3 );
  result = bitmap.byte_per_line == 2;
  zBitmapDestroy( &bitmap );
  zAssert( zBitmapAlloc (9 bits), result );
  zBitmapAlloc( &bitmap, 25, 3 );
  result = bitmap.byte_per_line == 4;
  zBitmapDestroy( &bitmap );
  zAssert( zBitmapAlloc (25 bits), result );
}

void assert_bitmap_pattern(void)
{
  zBitmap bitmap;
  ubyte pattern_org[3], pattern_dec[3];
  uint i, j, k;
  const int row_size = 10;
  bool result = true;

  zBitmapAlloc( &bitmap, 8, row_size );
  for( i=0; i<row_size; i++ ){
    pattern_org[0] = zRandI( 0, 0xff );
    for( k=0; k<8; k++ ){
      zBitmapPutBit( &bitmap, k, i, ( pattern_org[0] & ( 0x1 << k ) ) >> k );
    }
    for( pattern_dec[0]=0, k=0; k<8; k++ ){
      pattern_dec[0] |= zBitmapGetBit( &bitmap, k, i ) << k;
    }
    if( pattern_org[0] != pattern_dec[0] ) result = false;
  }
  zBitmapDestroy( &bitmap );
  zAssert( zBitmapPutBit / zBitmapGetBit (1-byte per line), result );

  zBitmapAlloc( &bitmap, 24, row_size );
  for( i=0; i<row_size; i++ ){
    for( j=0; j<3; j++ ){
      pattern_org[j] = zRandI( 0, 0xff );
      for( k=0; k<8; k++ ){
        zBitmapPutBit( &bitmap, j*8+k, i, ( pattern_org[j] & ( 0x1 << k ) ) >> k );
      }
    }
    for( j=0; j<3; j++ ){
      for( pattern_dec[j]=0, k=0; k<8; k++ ){
        pattern_dec[j] |= zBitmapGetBit( &bitmap, j*8+k, i ) << k;
      }
    }
    for( j=0; j<3; j++ )
      if( pattern_org[j] != pattern_dec[j] ) result = false;
  }
  zBitmapDestroy( &bitmap );
  zAssert( zBitmapPutBit / zBitmapGetBit (3-byte per line), result );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_bitmap_size();
  assert_bitmap_pattern();
  return 0;
} 

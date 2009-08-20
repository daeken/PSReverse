#ifndef __J2K__LZH__LZHLEncoder2_CPP__
#define __J2K__LZH__LZHLEncoder2_CPP__

#include "Incs.h"

void LZHLEncoder::_callStat() {
  nextStat = 2;   // to avoid recursion, >=2

  _put( NHUFFSYMBOLS - 2 );

  int groups[ 16 ];
  stat->calcStat( groups );

  int lastNBits = 0;

  for( int i=0; i < 16 ; ++i ) {
    int nBits = groups[ i ];
    assert( nBits >= lastNBits && nBits <= 8 );
    int delta = nBits - lastNBits;
    lastNBits = nBits;
    _putBits( delta + 1, 1 );
  }
}

void LZHLEncoder::putRaw( const BYTE* src, size_t sz ) {
  for( const BYTE* srcEnd = src + sz; src < srcEnd ; ++src ) {
     _put( *src );
  }
}

void LZHLEncoder::putMatch( const BYTE* src, size_t nRaw, size_t matchOver, size_t disp )
{
  assert( nRaw <= maxRaw );
  assert( matchOver <= maxMatchOver );
  assert( disp >= 0 && disp < LZBUFSIZE );
  putRaw( src, nRaw );
  struct MatchOverItem { int symbol; int nBits; UINT16 bits; };

  static MatchOverItem _matchOverTable[] = {
    { 264, 1, 0x00 },

    { 265, 2, 0x00 },
    { 265, 2, 0x02 },

    { 266, 3, 0x00 },
    { 266, 3, 0x02 },
    { 266, 3, 0x04 },
    { 266, 3, 0x06 },

    { 267, 4, 0x00 },
    { 267, 4, 0x02 },
    { 267, 4, 0x04 },
    { 267, 4, 0x06 },
    { 267, 4, 0x08 },
    { 267, 4, 0x0A },
    { 267, 4, 0x0C },
    { 267, 4, 0x0E },
  };

  if ( matchOver < 8 ) {
    _put( 256 + matchOver );

  } else if ( matchOver < 38 ) {
    matchOver -= 8;
    MatchOverItem* item = &_matchOverTable[ matchOver >> 1 ];
    _put( item->symbol, item->nBits, item->bits | (matchOver & 0x01) );

  } else {
    matchOver -= 38;
    MatchOverItem* item = &_matchOverTable[ matchOver >> 5 ];
    _put( item->symbol + 4 );
    _putBits( item->nBits + 4, ( item->bits << 4 ) | (matchOver & 0x1F) );
  }

  static struct DispItem { int nBits; UINT16 bits; } _dispTable[] = {
    #include "Table/hdisp.tbl"
  };

  #if LZBUFBITS < 8
    #error
  #endif

  DispItem* item = &_dispTable[ disp >> (LZBUFBITS - 7) ];
  int nBits = item->nBits + (LZBUFBITS - 7);
  UINT32 bits = ( ((UINT32)item->bits) << (LZBUFBITS - 7) ) | ( disp & ( ( 1 << (LZBUFBITS - 7) ) - 1 ) );

  #if LZBUFBITS >= 15
    if ( nBits > 16 ) {
      assert( nBits <= 32 );
      _putBits( nBits - 16, bits >> 16 );
      _putBits( 16, bits & 0xFFFF );

    } else
  #endif

    {
      assert( nBits <= 16 );
      _putBits( nBits, bits );
    }
}

size_t LZHLEncoder::flush() {
 _put( NHUFFSYMBOLS - 1 );

 while( nBits > 0 ) {
   *dst++ = (BYTE)( bits >> 24 );
   nBits -= 8;
   bits <<= 8;
 }

 return dst - dstBegin;
}

#endif

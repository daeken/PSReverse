#ifndef __J2K__LZH__LZHLEncoder_CPP__
#define __J2K__LZH__LZHLEncoder_CPP__

#include "Incs.h"

LZHLEncoder::LZHLEncoder( LZHLEncoderStat* stat_, BYTE* dst_ )
  : stat( stat_ ),
    sstat( stat_->stat ),
    nextStat( stat_->nextStat ),
    dst( dst_ ),
    dstBegin( dst_ ),
    bits( 0 ),
    nBits( 0 )
{ }

LZHLEncoder::~LZHLEncoder() { }

void LZHLEncoder::_putBits( int codeBits, UINT32 code ) {
  assert( codeBits <= 16 );
  bits |= ( code << ( 32 - nBits - codeBits ) );
  nBits += codeBits;

  if ( nBits >= 16 ) {
    *dst++ = (BYTE)( bits >> 24 );
    *dst++ = (BYTE)( bits >> 16 );

    nBits -= 16;
    bits <<= 16;
  }
}

void LZHLEncoder::_put( UINT16 symbol ) {
  assert( symbol < NHUFFSYMBOLS );

  if ( --nextStat <= 0 ) {
    _callStat();
  }

  ++sstat[ symbol ];

  LZHLEncoderStat::Symbol* item = &stat->symbolTable[ symbol ];
  assert( item->nBits >= 0 );

  _putBits( item->nBits, item->code );
}

void LZHLEncoder::_put( UINT16 symbol, int codeBits, UINT32 code ) {
  assert( symbol < NHUFFSYMBOLS );
  assert( codeBits <= 4 );

  if ( --nextStat <= 0 ) {
    _callStat();
  }

  ++sstat[ symbol ];

  LZHLEncoderStat::Symbol* item = &stat->symbolTable[ symbol ];
  assert( item->nBits >= 0 );

  int nBits = item->nBits;
  _putBits( nBits + codeBits, ( item->code << codeBits ) | code );
}

#endif

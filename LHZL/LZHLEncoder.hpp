#ifndef __J2K__LZH__LZHLEncoder_HPP__
#define __J2K__LZH__LZHLEncoder_HPP__

#include "Incs.h"

class LZHLEncoder {
public:
  enum { maxMatchOver = 517, maxRaw = 64 };
  LZHLEncoder( LZHLEncoderStat* stat_, BYTE* dst_ );
  ~LZHLEncoder();

private:
  LZHLEncoderStat* stat;
  HUFFINT* sstat;
  int& nextStat;

  BYTE* dst;
  BYTE* dstBegin;
  UINT32 bits;
  int nBits;

public:
  static size_t calcMaxBuf( size_t rawSz ) {
     return rawSz + ( rawSz >> 1 ) + 32;
  }

public:
  size_t flush();

  void putRaw( const BYTE* src, size_t sz );
  void putMatch( const BYTE* src, size_t nRaw, size_t matchOver, size_t disp );

private:
  void _callStat();

  void _put( UINT16 symbol );
  void _put( UINT16 symbol, int codeBits, UINT32 code );
  void _putBits( int codeBits, UINT32 code );
};

#endif

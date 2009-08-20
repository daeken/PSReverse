#ifndef __J2K__LZH__LZHLCompressor_HPP__
#define __J2K__LZH__LZHLCompressor_HPP__

#include "Incs.h"

class LZHLCompressor : private LZBuffer {
public:
  LZHLCompressor();
  virtual ~LZHLCompressor();

public:
  static size_t calcMaxBuf( size_t rawSz ) {
    return LZHLEncoder::calcMaxBuf( rawSz );
  }

  size_t compress( BYTE* dst, const BYTE* src, size_t sz );

private:
  void _wrapTable();
  LZHASH _updateTable( LZHASH hash, const BYTE* src, LZPOS pos, int len );

private:
  LZHLEncoderStat stat;
  LZTableItem* table;
};

#endif

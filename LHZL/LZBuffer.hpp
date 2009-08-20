#ifndef __J2K__LZH__LZBuffer_HPP__
#define __J2K__LZH__LZBuffer_HPP__

#include "Incs.h"

class LZBuffer {
public:
  LZBuffer();
  ~LZBuffer();

protected:
  static int _wrap( LZPOS pos );
  static int _distance( int diff );

  void _toBuf( BYTE );
  void _toBuf( const BYTE*, size_t sz );
  void _bufCpy( BYTE* dst, int pos, size_t sz );
  int _nMatch( int pos, const BYTE* p, int nLimit );

public:
  BYTE* buf;
  LZPOS bufPos;
};

#endif

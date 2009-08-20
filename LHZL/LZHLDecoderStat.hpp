#ifndef __J2K__LZH__LZHLDecoderStat_HPP__
#define __J2K__LZH__LZHLDecoderStat_HPP__

#include "Incs.h"

class LZHLDecoderStat : public HuffStat {
public:
  LZHLDecoderStat();
  virtual ~LZHLDecoderStat();

public:
  struct Group { int nBits; int pos; };

public:
  static Group groupTable0[];
  Group groupTable[ 16 ];

  static HUFFINT symbolTable0[];
  HUFFINT* symbolTable;
};

#endif

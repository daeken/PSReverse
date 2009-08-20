#ifndef __J2K__LZH__LZHLDecoderStat_CPP__
#define __J2K__LZH__LZHLDecoderStat_CPP__

#include "Incs.h"

LZHLDecoderStat::LZHLDecoderStat() {
  symbolTable = new HUFFINT[ NHUFFSYMBOLS ];
  memcpy( symbolTable, symbolTable0, sizeof(HUFFINT)*NHUFFSYMBOLS );
  memcpy( groupTable,  groupTable0,  sizeof(Group)*16 );
}

LZHLDecoderStat::~LZHLDecoderStat() {
  delete [] symbolTable;
}

LZHLDecoderStat::Group LZHLDecoderStat::groupTable0[ 16 ] =  {
  #include "Table/hdec_g.tbl"
};

HUFFINT LZHLDecoderStat::symbolTable0[ NHUFFSYMBOLS ] =  {
  #include "Table/hdec_s.tbl"
};

#endif

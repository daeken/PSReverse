#ifndef __J2K__LZH__HuffStat_HPP__
#define __J2K__LZH__HuffStat_HPP__

#include "Incs.h"

class HuffStat {
public:
  HuffStat();
  virtual ~HuffStat();
public:
  HUFFINT* stat;

protected:
  int makeSortedTmp( HuffStatTmpStruct* );

};

#endif

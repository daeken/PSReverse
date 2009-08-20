#ifndef __J2K__LZH__LZHLEncoderStat_HPP__
#define __J2K__LZH__LZHLEncoderStat_HPP__

class LZHLEncoderStat : public HuffStat {
public:
  struct Symbol {
    HUFFINT  nBits;
    HUFFUINT code;
  };

public:
  LZHLEncoderStat();
  ~LZHLEncoderStat();

public:
  void calcStat( int* groups );

private:
  inline static void _addGroup( int* groups, int group, int nBits );

public:
  int nextStat;

  static Symbol symbolTable0[];
  Symbol* symbolTable;
};

#endif

#ifndef __J2K__LZH__LZHLEncoderStat_CPP__
#define __J2K__LZH__LZHLEncoderStat_CPP__

#include "Incs.h"

LZHLEncoderStat::LZHLEncoderStat() 
{
  nextStat = HUFFRECALCLEN;
  symbolTable = new Symbol[ NHUFFSYMBOLS ];
  memcpy( symbolTable, symbolTable0, sizeof(Symbol)*NHUFFSYMBOLS );
}

LZHLEncoderStat::~LZHLEncoderStat() 
{
  delete [] symbolTable;
}

LZHLEncoderStat::Symbol LZHLEncoderStat::symbolTable0[ NHUFFSYMBOLS ] =  {
   #include "Table/henc.tbl"
};

inline void LZHLEncoderStat::_addGroup( int* groups, int group, int nBits )
{
  assert( nBits <= 8 );

  //Bubble sort
  for( int j=group; j > 0 && nBits < groups[ j - 1 ] ; --j )
      groups[ j ] = groups[ j - 1 ];

  groups[ j ] = nBits;
}

void LZHLEncoderStat::calcStat( int* groups ) 
{
  HuffStatTmpStruct s[ NHUFFSYMBOLS ];
  int total = makeSortedTmp( s );

  nextStat = HUFFRECALCLEN;

  int pos = 0;
  int nTotal = 0;

  for ( int group=0; group < 14 ; ++group ) 
  {
    int avgGroup = ( total - nTotal )/( 16 - group );
    int i = 0, n = 0, nn = 0;

    for ( int nBits=0 ;; ++nBits ) 
    {
      int over = 0;
      int nItems = 1 << nBits;

      if ( pos + i + nItems > NHUFFSYMBOLS ) 
      {
        nItems = NHUFFSYMBOLS - pos;
        over = 1;
      }

      for ( ; i < nItems ; ++i )
        nn += s[ pos + i ].n;

      if ( over || nBits >= 8 || nn > avgGroup ) 
      {
        if ( nBits == 0 || abs( n - avgGroup ) > abs( nn - avgGroup ) ) {
          n = nn;
        } else {
          --nBits;
        }

        _addGroup( groups, group, nBits );
        nTotal += n;
        pos += 1 << nBits;

        break;

      } else {
        n = nn;
      }
    }
  }

  int bestNBits = 0, bestNBits15 = 0;
  int best = 0x7FFFFFFF;
  int i = 0, nn = 0, left = 0;

  for ( int j=pos; j < NHUFFSYMBOLS ; ++j )
    left += s[ j ].n;

  for ( int nBits = 0 ;; ++nBits ) {
    int nItems = 1 << nBits;
    if ( pos + i + nItems > NHUFFSYMBOLS )
      break;

    for ( ; i < nItems ; ++i )
      nn += s[ pos + i ].n;

    int nItems15 = NHUFFSYMBOLS - ( pos + i );

    for ( int nBits15=0 ;; ++nBits15 )
      if ( 1 << nBits15 >= nItems15 )
        break;
        
    assert( left >= nn );

    if ( nBits <= 8 && nBits15 <= 8 ) {
      int n = nn * nBits + ( left - nn ) * nBits15;

      if ( n < best ) {
        best = n;
        bestNBits   = nBits;
        bestNBits15 = nBits15;

      } else {
        break;  // PERF optimization
      }
    }
  }

  int pos15 = pos + ( 1 << bestNBits );
  _addGroup( groups, 14, bestNBits );
  _addGroup( groups, 15, bestNBits15 );

  pos = 0;

  for ( j=0; j < 16 ; ++j ) {
    int nBits = groups[ j ];
    int nItems = 1 << nBits;
    int maxK = min( nItems, NHUFFSYMBOLS - pos );

    for ( int k=0; k < maxK ; ++k ) {
      int symbol = s[ pos + k ].i;
      symbolTable[ symbol ].nBits = nBits + 4;
      symbolTable[ symbol ].code = ( j << nBits ) | k;
    }

    pos += 1 << nBits;
  }
}

#endif

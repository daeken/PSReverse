#ifndef __J2K__LZH__LZHMacro_HPP__
#define __J2K__LZH__LZHMacro_HPP__

#ifndef BYTE
  #define BYTE   unsigned char
#endif

#ifndef INT16
  #define INT16  signed short
#endif

#ifndef INT32
  #define INT32  signed long
#endif

#ifndef UINT16
  #define UINT16 unsigned short
#endif

#ifndef UINT32
  #define UINT32 unsigned long
#endif

#ifndef min
  #define min( a, b ) ( (a) < (b) ? (a) : (b) )
#endif

#ifndef max
  #define max( a, b ) ( (a) > (b) ? (a) : (b) )
#endif

#ifdef _MSC_VER
  #pragma intrinsic( memcpy, memset, _rotl )
  #define ROTL( x, y ) _rotl( x, y )
#else
  #define ROTL( x, y ) ( ( (x) << (y) ) | ( (x) >> (32-(y)) ) )
#endif

#define LZMIN 4

//{{{{{{{{{{{{{{{{{ USER - TUNABLE ********************************************

//Affect format
#define LZBUFBITS 14
//LZBUFBITS is a log2(LZBUFSIZE) and must be in range 10 - 16

//NOT affect format
#define LZMATCH 5

#define LZSLOWHASH
#define LZTABLEBITS 15
//LZTABLEBITS is a log2(LZTABLESIZE) and should be in range 9 - 17

#define LZOVERLAP
#define LZBACKWARDMATCH
#define LZLAZYMATCH
#define LZSKIPHASH 1024

#define HUFFRECALCLEN 4096
//HUFFRECALCLEN should be <= 16384

//}}}}}}}}}}}}}}}}} USER - TUNABLE ********************************************

#define LZTABLESIZE (1<<(LZTABLEBITS))
#define LZBUFSIZE (1<<(LZBUFBITS))

/**************************************************************************/
#define LZPOS UINT32
#define LZBUFMASK ( (LZBUFSIZE) - 1 )

#define LZTABLEINT UINT16
typedef LZTABLEINT LZTableItem;

#define LZHASH UINT32
/**************************************************************************/

#define HUFFINT      INT16
#define HUFFUINT     UINT16
#define NHUFFSYMBOLS ( 256 + 16 + 2 )

/**************************************************************************/

#ifdef LZSLOWHASH
#define LZHASHSHIFT 5

#define UPDATE_HASH( hash, c )          \
    {                                   \
    hash ^= (c);                        \
    hash = ROTL( hash, LZHASHSHIFT );   \
    }

#define UPDATE_HASH_EX( hash, src )                     \
    {                                                   \
    hash ^= ROTL( (src)[ 0 ], LZHASHSHIFT * LZMATCH );  \
    hash ^= (src)[ LZMATCH ];                           \
    hash = ROTL( hash, LZHASHSHIFT );                   \
    }

#define HASH_POS(hash) ((( (hash) * 214013 + 2531011) >> (32-LZTABLEBITS)) )

#else

#define LZHASHSHIFT (((LZTABLEBITS)+(LZMATCH)-1)/(LZMATCH))

#define UPDATE_HASH( hash, c ) { hash = ( hash << LZHASHSHIFT ) ^ (c); }
#define UPDATE_HASH_EX( hash, src )  { hash = ( hash << LZHASHSHIFT ) ^ (src)[ LZMATCH ]; }
#define LZHASHMASK ((LZTABLESIZE)-1)
#define HASH_POS( hash ) ( (hash) & LZHASHMASK )
#endif

#define HUFFRECALCSTAT( s ) ( (s) >> 1 )

#endif

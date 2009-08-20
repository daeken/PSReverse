#ifndef __J2K__LZH__LZBuffer_CPP__
#define __J2K__LZH__LZBuffer_CPP__

#include "Incs.h"

LZBuffer::LZBuffer() 
{
  buf = new BYTE[ LZBUFSIZE ];
  bufPos = 0;
}

LZBuffer::~LZBuffer() 
{
  delete [] buf;
}

int LZBuffer::_wrap( LZPOS pos ) 
{
  return ( pos & LZBUFMASK );
}

int LZBuffer::_distance( int diff ) 
{
  return ( diff & LZBUFMASK );
}

void LZBuffer::_toBuf( BYTE c ) 
{
  buf[ _wrap( bufPos++ ) ] = c;
}

void LZBuffer::_toBuf( const BYTE* src, size_t sz ) 
{
  assert( sz < LZBUFSIZE );
  int begin = _wrap( bufPos );
  int end = begin + sz;

  if ( end > LZBUFSIZE ) 
  {
    size_t left = LZBUFSIZE - begin;
    memcpy( buf + begin, src, left );
    memcpy( buf, src + left, sz - left );
  } 
  else 
  {
    memcpy( buf + begin, src, sz );
  }

  bufPos += sz;
}

void LZBuffer::_bufCpy( BYTE* dst, int pos, size_t sz ) 
{
  assert( sz < LZBUFSIZE );
  int begin = _wrap( pos );
  int end = begin + sz;

  if ( end > LZBUFSIZE ) 
  {
    size_t left = LZBUFSIZE - begin;
    memcpy( dst, buf + begin, left );
    memcpy( dst + left, buf, sz - left );
  } 
  else 
  {
    memcpy( dst, buf + begin, sz );
  }
}

int LZBuffer::_nMatch( int pos, const BYTE* p, int nLimit ) 
{
  assert( nLimit < LZBUFSIZE );
  int begin = pos;
  if ( LZBUFSIZE - begin >= nLimit ) 
  {
    for ( int i = 0; i < nLimit ; i++ )
       if ( buf[ begin + i ] != p[ i ] )
          return i;

    return nLimit;

   } 
   else 
   {
     for ( int i = begin; i < LZBUFSIZE ; i++ )
       if ( buf[ i ] != p[ i - begin ] )
         return i - begin;

     int shift = LZBUFSIZE - begin;
     int n = nLimit - shift;

     for( i = 0; i < n ; i++ )
       if( buf[ i ] != p[ shift + i ] )
          return shift + i;

     return nLimit;
   }
}

#endif

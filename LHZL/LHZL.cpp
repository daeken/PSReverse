// LHZL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Incs.h"

typedef LZHLDecoderStat::Group Group;

extern "C" {

__declspec(dllexport) void *initComp() {
	return new LZHLCompressor();
}

__declspec(dllexport) size_t compress(LZHLCompressor *comp, unsigned char *buf, size_t size, unsigned char *ret) {
	return comp->compress(ret, buf, size);
}

__declspec(dllexport) void *initDecomp() {
	return new LZHLDecompressor();
}

__declspec(dllexport) size_t decompress(LZHLDecompressor *decomp, unsigned char *buf, size_t size, unsigned char *ret, size_t retsize) {
	decomp->decompress(ret, &retsize, buf, &size);
	return retsize;
}

}

from ctypes import *

class Compressor(object):
	def __init__(self):
		self.module = CDLL('LHZL')
		self.handle = self.module.initComp()
	
	def compress(self, data):
		ret = (c_ubyte * (len(data)*2))()
		size = self.module.compress(c_void_p(self.handle), c_char_p(data), len(data), ret)
		return ''.join(map(chr, ret[:size]))

class Decompressor(object):
	def __init__(self):
		self.module = CDLL('LHZL')
		self.handle = self.module.initDecomp()
	
	def decompress(self, data):
		size = c_int(65536)
		ret = (c_ubyte * 65536)()
		size = self.module.decompress(c_void_p(self.handle), c_char_p(data), c_int(len(data)), ret, byref(size))
		return ''.join(map(chr, ret[:size]))

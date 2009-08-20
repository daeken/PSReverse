"""
Run this against your Pokerstars binary to replace the official cert with yours.
Leave the patched binary in a different filename so the patcher doesn't replace it.

Make sure you use the public key.
"""

def usage(fn):
	print 'Usage: %s <original binary filename> <new binary filename> <old cert> <new cert>' % fn

def main(*args):
	old = file(args[0], 'rb').read()
	oldCert = file(args[2], 'rb').read()
	newCert = file(args[3], 'rb').read()
	
	i = old.find(oldCert)
	while i != -1:
		print 'Found cert at %08x' % i
		old = old[:i] + newCert + old[i+len(oldCert):]
		i = old.find(oldCert, i)
	
	file(args[1], 'wb').write(old)

if __name__=='__main__':
	import sys
	if len(sys.argv) == 5:
		main(*sys.argv[1:])
	else:
		usage(sys.argv[0])

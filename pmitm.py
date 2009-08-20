"""
PokerStars man-in-the-middle proxy.

Run with no arguments, it will bind to *:443 and log pokerstars traffic to log.txt.
Make sure you use a patched client with PSHooker.
"""

import lzhl, select, socket, ssl, sys, thread

def dump(log, data, dir):
	print >>log, dir, `data`

printable = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()_+=-[]{};\':",./<>?\\|'
def pretty(log, data, dir):
	print >>log, dir, len(data), 'bytes:'
	
	size = len(data)
	for i in xrange(0, size, 16):
		print >>log, '%04x' % i, '', 
		for j in xrange(16):
			if j == 8:
				print >>log, '', 
			if i+j < size:
				print >>log, '%02x' % ord(data[i+j]), 
			else:
				print >>log, '  ', 
		
		log.write('| ') 
		for j in xrange(16):
			if j == 8:
				log.write(' ')
			if i+j < size:
				if data[i+j] in printable:
					log.write(data[i+j])
				else:
					log.write('.')
			else:
				log.write(' ') 
		print >>log, ''
	print >>log, ''

def readall(sock, size):
	ret = ''
	while len(ret) < size:
		ret += sock.read(size-len(ret))
	return ret

def client(sock):
	print 'new client'
	sock = ssl.wrap_socket(sock, certfile='mitmcert.key', server_side=True, ssl_version=ssl.PROTOCOL_SSLv3)
	
	oserv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	serv = ssl.wrap_socket(oserv, ssl_version=ssl.PROTOCOL_SSLv3, cert_reqs=ssl.CERT_REQUIRED, ca_certs='officialcert.pub')
	serv.connect(('77.87.178.69', 443))
	
	log = file('log.txt', 'wb')
	cinit = False
	sinit = False
	
	cdecomp = lzhl.Decompress()
	sdecomp = lzhl.Decompress()
	while True:
		rlist, _, __ = select.select([sock, serv], [], [])
		
		if sock in rlist:
			if not cinit:
				init = sock.read(1024)
				serv.write(init)
				print `init`
				#pretty(sys.stdout, init, '->')
				cinit = True
			else:
				data = readall(sock, 2)
				size = (ord(data[0]) << 8) | ord(data[1])
				cdata = readall(sock, size)
				serv.write(data+cdata)
				cdata = cdecomp.decompress(cdata)
				#print 'Wrote', size
				dump(log, cdata, '->')
				#pretty(log, cdata, '->')
				log.flush()
		if serv in rlist:
			if not sinit:
				init = serv.read(1024)
				sock.write(init)
				print `init`
				#pretty(sys.stdout, init, '<-')
				sinit = True
			else:
				data = readall(serv, 2)
				size = (ord(data[0]) << 8) | ord(data[1])
				cdata = readall(serv, size)
				sock.write(data+cdata)
				cdata = sdecomp.decompress(cdata)
				#print 'Read', size
				dump(log, cdata, '<-')
				#pretty(log, cdata, '<-')
				log.flush()

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('', 443))
sock.listen(5)

while True:
	thread.start_new_thread(client, (sock.accept()[0], ))

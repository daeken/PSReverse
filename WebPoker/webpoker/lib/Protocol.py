from Struct import Struct, defStructBE
uint8, uint16, uint32 = Struct.uint8, Struct.uint16, Struct.uint32

# XXX: Set this!
logPath = 'F:/Projects/PStarsReverse/log.txt'

class Protocol(object):
	def __init__(self):
		self.load()
	
	def load(self):
		fp = file(logPath, 'r')
		ldata = []
		for line in fp.readlines():
			try:
				dir, data = line.split(' ', 1)
				ldata.append((dir, eval(data)))
			except:
				break
		
		#self.data = [(True, dir, data[2:], []) for dir, data in ldata]
		#return
		
		packets = []
		cur = {'<-' : None, '->' : None}
		while len(ldata):
			dir, data = ldata.pop(0)
			if cur[dir] != None:
				cdata, needed = cur[dir]
				cdata += data[:needed]
				used = min(needed, len(data))
				needed -= needed
				if needed == 0:
					cur[dir] = None
					data = data[used:]
					packets.append((dir, cdata))
				else:
					cur[dir] = cdata, needed
					continue
			
			while len(data):
				size = (ord(data[0]) << 8) | ord(data[1])
				if len(data) < size:
					cur[dir] = data[2:], size-len(data)
					break
				else:
					packets.append((dir, data[2:size]))
				data = data[size:]
		
		msgs = []
		cur = {'<-' : None, '->' : None}
		for dir, data in packets:
			if cur[dir] != None:
				cur[dir].append(data[2:])
				if (ord(data[0]) & 0x40) == 0x40:
					cur[dir] = None
			elif (ord(data[0]) & 0xC0) == 0x80:
				if data[1] == '\0':
					data = data[6:]
				else:
					data = data[2:]
				cur[dir] = Message(dir, data)
				msgs.append(cur[dir])
			else:
				msgs.append(Message(dir, data[2:]))
		
		to = {}
		self.data = []
		for msg in msgs:
			tup = (msg.opcode()-1, msg.id())
			if tup in to:
				to[tup].response = msg
			else:
				self.data.append(msg)
				to[(msg.opcode(), msg.id())] = msg

printable = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()_+=-[]{};\':",./<>?\\| '
class Message(object):
	def __init__(self, dir, data):
		self.dir, self.data = dir, data
		self.response = None
	
	def opcode(self):
		return ord(self.data[2])
	
	def id(self):
		if self.data[1] == '\0':
			return self.data[3:7]
		else:
			return self.data[1:5]
	
	def append(self, data):
		self.data += data
	
	def dump(self, log):
		if self.opcode() == 0x38:
			ip, inst, name = self.data[6:].rstrip('\0').split('\0')
			print >>log, '<br />Get %s::%s<br />' % (inst, name)
			
			if (inst, name) == ('Alt2LobbyServerInstance', 'display.HoldemPM.NL'):
				data = self.response.data[6:]
				count = (ord(data[0x2a]) << 8) | ord(data[0x2b])
				print >>log, count, 'tables<br />'
				off = 0x2d
				
				for i in xrange(count):
					name, ip, id, rest = data[off+0x1c:off+0x11c].split('\0', 3)
					print >>log, name, '&ndash;', id, '<br />'
					off += 0x1c + len(name) + len(ip) + len(id) + 3
					if data[off+0x12:off+0x16] == 'fast':
						off += 4
					off += 0x31
				
				print >>log, len(data)-off
		
		print >>log, '<pre>'
		print >>log, '%s %02x' % (self.dir, self.opcode())
		
		size = len(self.data)
		for i in xrange(0, size, 16):
			log.write('%04x  ' % i)
			for j in xrange(16):
				if j == 8:
					log.write(' ')
				if i+j < size:
					log.write('%02x ' % ord(self.data[i+j]))
				else:
					log.write('   ')
			
			log.write('| ')
			for j in xrange(16):
				if j == 8:
					log.write(' ')
				if i+j < size:
					if self.data[i+j] in printable:
						log.write(self.data[i+j].replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;'))
					else:
						log.write('.')
				else:
					log.write(' ')
			log.write('\n')
		print >>log, '</pre>'

%for i, msg in ((i, log[i]) for i in xrange(len(log))):
	<a href="/message/${i}" target="message" />${msg.dir} ${'%08x' % i} &ndash; ${'%05i' % len(msg.data) | n} (${'%02x' % msg.opcode()})<br />
%endfor

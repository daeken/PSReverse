import logging
import cStringIO as StringIO

from pylons import request, response, session, tmpl_context as c
from pylons.controllers.util import abort, redirect_to

from webpoker.lib.base import BaseController, render
from webpoker.lib.Protocol import Protocol

log = logging.getLogger(__name__)

proto = Protocol()

class LogviewerController(BaseController):
	def index(self):
		return render('Frames.mako')
	
	def messagelist(self):
		return render('MessageList.mako', dict(log=proto.data))
	
	def message(self, id):
		id = int(id)
		msg = proto.data[id]
		
		fp = StringIO.StringIO()
		msg.dump(fp)
		
		if msg.response != None:
			fp.write('<br /><br />')
			msg.response.dump(fp)
		
		return (
				'<a href="/message/%i" target="message">&lt;&ndash;</a> %x <a href="/message/%i" target="message">&ndash;&gt;</a><br />' + 
				fp.getvalue().replace('%', '%%')
			) % (id-1, id, id+1)

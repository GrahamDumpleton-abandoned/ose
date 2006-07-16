# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc

from netrpc.common import _escape
from netrpc.xmlrpc import _Decoder,_Encoder


class _RpcServlet(netsvc.HttpServlet,netsvc.Service):

  def __init__(self,session,binding):
    netsvc.HttpServlet.__init__(self,session)
    netsvc.Service.__init__(self,"","")
    self._binding = binding
    self._contentLength = 0
    self._decoder = None

  def destroyServlet(self):
    netsvc.HttpServlet.destroyServlet(self)
    netsvc.Service.destroyReferences(self)

  def processRequest(self):
    try:
      if self.requestMethod() != "POST":
	self.sendError(400,"Only POST method supported.")
	return
      if self.contentLength() <= 0:
	self.sendError(400,"Invalid content length.")
	return
      self._decoder = _Decoder()
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def processContent(self,content):
    try:
      try:
	self._decoder.feed(content)
	self._contentLength = self._contentLength + len(content)
	if self._contentLength < self.contentLength():
	  return
	if self._contentLength > self.contentLength():
	  error = netsvc.SERVER_REQUEST_DECODING_ERROR
	  description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	  origin = "netsvc.xmlrpc"
	  details = "Invalid content length."
	  raise netsvc.ServiceFailure(error,description,origin,details)
	content,method,params = self._decoder.result()
      except netsvc.ServiceFailure:
	raise
      except:
	error = netsvc.SERVER_REQUEST_DECODING_ERROR
	description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	origin = "netsvc.xmlrpc"
	details = netsvc.exceptionDescription()
	raise netsvc.ServiceFailure(error,description,origin,details)

      if content != "request":
	error = netsvc.SERVER_REQUEST_DECODING_ERROR
	description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	origin = "netsvc.xmlrpc"
	details = "Invalid XML-RPC message."
	raise netsvc.ServiceFailure(error,description,origin,details)

      service = self.serviceEndPoint(self._binding)
      id = apply(service.__getattr__(method),params)
      self.processResponse(self.processResult,id)
      self.processFailure(self.processResult,id)

    except netsvc.ServiceFailure,exception:
      try:
	params = {}
	params["faultCode"] = exception.error
	description = exception.description
        if exception.origin != "":
	  description = "%s -- %s" % (exception.origin,description)
	if exception.details != "":
	  description = "%s\n\n%s" % (description,exception.details)
	params["faultString"] = description
	encoder = _Encoder()
	body = encoder.convert(params,"fault")
        template = '<?xml version="1.0"?>\n<methodResponse>\n' \
                   '<fault>\n%s</fault>\n</methodResponse>\n'
        body = template % body
	self.sendResult(body)
      except:
	self.sendError(500,_escape(netsvc.exceptionDetails()))
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def processResult(self,params):
    try:
      if self.failureOccurred():
	exception = self.currentFailure()
        error = exception.error()
        description = exception.description()
        origin = exception.origin()
        details = exception.details()
	raise netsvc.ServiceFailure(error,description,origin,details)

      encoder = _Encoder()
      body = encoder.convert(params,"response")
      template = '<?xml version="1.0"?>\n<methodResponse>\n' \
		 '<params>\n%s</params>\n</methodResponse>\n'
      body = template % body
      self.sendResult(body)
    except netsvc.ServiceFailure,exception:
      try:
	params = {}
	params["faultCode"] = exception.error
	description = exception.description
        if exception.origin != "":
	  description = "%s -- %s" % (exception.origin,description)
	if exception.details != "":
	  description = "%s\n\n%s" % (description,exception.details)
	params["faultString"] = description
	encoder = _Encoder()
	body = encoder.convert(params,"fault")
        template = '<?xml version="1.0"?>\n<methodResponse>\n' \
                   '<fault>\n%s</fault>\n</methodResponse>\n'
        body = template % body
	self.sendResult(body)
      except:
	self.sendError(500,_escape(netsvc.exceptionDetails()))
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def sendResult(self,body):
    self.sendResponse(200)
    self.sendHeader("Content-Type","text/xml")
    self.sendHeader("Content-Length",str(len(body)))
    self.endHeaders()
    self.sendContent(body)
    self.endContent()

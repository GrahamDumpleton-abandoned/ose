# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import string

from netrpc.common import _escape

# The soaplib module is from PythonWare, found at:
#    http://www.pythonware.com/products

import soaplib

class _RpcServlet(netsvc.HttpServlet,netsvc.Service):

  def __init__(self,session,binding):
    netsvc.HttpServlet.__init__(self,session)
    netsvc.Service.__init__(self,"","")
    self._binding = binding
    self._method = None
    self._contentLength = 0
    self._content = []

  def destroyServlet(self):
    netsvc.HttpServlet.destroyServlet(self)
    netsvc.Service.destroyReferences(self)

  def decodeValue(self,name,value):
    if name == "xsd:base64Binary":
      return soaplib.Binary(value)
    elif name == "xsd:boolean":
      return soaplib.Boolean(int(value))
    elif name == "xsd:dateTime":
      # XXX Supply same format as soaplib.
      value = string.replace(value,"-","")
      object = soaplib.DateTime()
      object.value = value
      return object
    return netsvc.decodeValue(self,name,value)

  def encodeObject(self,object):
    if isinstance(object,soaplib.Boolean):
      return ("xsd:boolean",str(object.value))
    elif isinstance(object,soaplib.Binary):
      return ("xsd:base64Binary",object.data)
    elif isinstance(object,soaplib.DateTime):
      value = object.value
      # XXX This needs to be made more rigourous.
      if len(value) == 17:
	value = "%s-%s-%s" % (value[0:4],value[4:6],value[6:])
      return ("xsd:dateTime",value)
    return netsvc.encodeObject(self,object)

  def processRequest(self):
    try:
      if self.requestMethod() != "POST":
	self.sendError(400,"Only POST method supported.")
	return
      if self.contentLength() <= 0:
	self.sendError(400,"Invalid content length.")
	return
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def processContent(self,content):
    try:
      try:
	if self._content == None:
	  return
	self._content.append(content)
	self._contentLength = self._contentLength + len(content)
	if self._contentLength < self.contentLength():
	  return
	request = string.join(self._content,"")
	self._method,params = soaplib.loads(request)
	self._contentLength = 0
	self._content = None
      except netsvc.ServiceFailure:
	netsvc.logException()
	raise
      except:
	error = netsvc.SERVER_REQUEST_DECODING_ERROR
	description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	origin = "netsvc.soap"
	details = netsvc.exceptionDescription()
	raise netsvc.ServiceFailure(error,description,origin,details)

      service = self.serviceEndPoint(self._binding)
      id = apply(service.__getattr__(self._method),params)
      self.processResponse(self.processResult,id)
      self.processFailure(self.processResult,id)

    except netsvc.ServiceFailure,exception:
      try:
	faultCode = exception.error
	description = exception.description
        if exception.origin != "":
	  description = "%s -- %s" % (exception.origin,description)
	if exception.details != "":
	  description = "%s\n\n%s" % (description,exception.details)
	faultString = description
        response = soaplib.dumps(soaplib.Fault(faultCode,faultString))
	self.sendResult(500,response)
      except:
	netsvc.logException()
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

      response = soaplib.dumps(soaplib.Response(self._method,None,params),1)
      self.sendResult(200,response)
    except netsvc.ServiceFailure,exception:
      try:
	faultCode = exception.error
	description = exception.description
        if exception.origin != "":
	  description = "%s -- %s" % (exception.origin,description)
	if exception.details != "":
	  description = "%s\n\n%s" % (description,exception.details)
	faultString = description
        response = soaplib.dumps(soaplib.Fault(faultCode,faultString))
	self.sendResult(500,response)
      except:
	self.sendError(500,_escape(netsvc.exceptionDetails()))
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def sendResult(self,status,body):
    self.sendResponse(status)
    self.sendHeader("Content-Type","text/xml")
    self.sendHeader("Content-Length",str(len(body)))
    self.endHeaders()
    self.sendContent(body)
    self.endContent()

# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import string

from netrpc.common import _escape

# The _xmlrpc module is from py-xmlrpc, found at:
#    http://sourceforge.net/projects/py-xmlrpc

import _xmlrpc

_booleanType = type(_xmlrpc.boolean(0))
_base64Type = type(_xmlrpc.base64(""))
_dateTimeType = type(_xmlrpc.dateTime(1,1,1,1,1,1))

class _RpcServlet(netsvc.HttpServlet,netsvc.Service):

  def __init__(self,session,binding):
    netsvc.HttpServlet.__init__(self,session)
    netsvc.Service.__init__(self,"","")
    self._binding = binding
    self._contentLength = 0
    self._content = []

  def destroyServlet(self):
    netsvc.HttpServlet.destroyServlet(self)
    netsvc.Service.destroyReferences(self)

  def decodeValue(self,name,value):
    if name == "xsd:base64Binary":
      return _xmlrpc.base64(value)
    elif name == "xsd:boolean":
      return _xmlrpc.boolean(int(value))
    elif name == "xsd:dateTime":
      value = string.replace(value,":"," ")
      value = string.replace(value,"T"," ")
      value = string.replace(value,"-"," ")
      value = string.splitfields(value)
      value = map(lambda x: int(x),value)
      return apply(_xmlrpc.dateTime,value)
    return netsvc.decodeValue(name,value)

  def encodeObject(self,object):
    if type(object) == _booleanType:
      return ("xsd:boolean",str(int(not not object)))
    elif type(object) == _base64Type:
      if _xmlrpc.VERSION in ['0.8.7']:
	return ("xsd:base64Binary",str(object)[1:-1])
      else:
	return ("xsd:base64Binary",str(object))
    elif type(object) == _dateTimeType:
      value = "%04d-%02d-%02dT%02d:%02d:%02d" % object.date()
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
	self._content.insert(0,"\r\n");
	self._content.insert(0,"Content-Type: text/xml\r\n");
	self._content.insert(0,"POST / HTTP/1.0\r\n");
	request = string.join(self._content,"")
	method,params,extra = _xmlrpc.parseRequest(request)
	self._contentLength = 0
	self._content = None
      except netsvc.ServiceFailure:
	netsvc.logException()
	raise
      except:
	error = netsvc.SERVER_REQUEST_DECODING_ERROR
	description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	origin = "netsvc.xmlrpc"
	details = netsvc.exceptionDescription()
	raise netsvc.ServiceFailure(error,description,origin,details)

      service = self.serviceEndPoint(self._binding)
      id = apply(service.__getattr__(method),params)
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
	response = _xmlrpc.buildFault(faultCode,faultString,{})
	self.sendComplete(response)
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

      response = _xmlrpc.buildResponse(params,{})
      self.sendComplete(response)
    except netsvc.ServiceFailure,exception:
      try:
	faultCode = exception.error
	description = exception.description
        if exception.origin != "":
	  description = "%s -- %s" % (exception.origin,description)
	if exception.details != "":
	  description = "%s\n\n%s" % (description,exception.details)
	faultString = description
	response = _xmlrpc.buildFault(faultCode,faultString,{})
	self.sendComplete(response)
      except:
	self.sendError(500,_escape(netsvc.exceptionDetails()))
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

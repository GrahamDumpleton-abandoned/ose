# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import string

from netrpc.common import _escape

# The xmlrpclib module is from PythonWare, found at:
#    http://www.pythonware.com/products

import xmlrpclib

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
      return xmlrpclib.Binary(value)
    elif name == "xsd:boolean":
      return xmlrpclib.Boolean(int(value))
    elif name == "xsd:dateTime":
      value = string.replace(value,"-","")
      object = xmlrpclib.DateTime()
      object.value = value
      return object
    return netsvc.decodeValue(name,value)

  def encodeObject(self,object):
    if isinstance(object,xmlrpclib.Boolean):
      return ("xsd:boolean",str(object.value))
    elif isinstance(object,xmlrpclib.Binary):
      return ("xsd:base64Binary",object.data)
    elif isinstance(object,xmlrpclib.DateTime):
      value = object.value
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
	params,method = xmlrpclib.loads(request)
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
        response = xmlrpclib.dumps(xmlrpclib.Fault(faultCode,faultString))
	self.sendResult(response)
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

      response = xmlrpclib.dumps((params,),methodresponse=1)
      self.sendResult(response)
    except netsvc.ServiceFailure,exception:
      try:
	faultCode = exception.error
	description = exception.description
        if exception.origin != "":
	  description = "%s -- %s" % (exception.origin,description)
	if exception.details != "":
	  description = "%s\n\n%s" % (description,exception.details)
	faultString = description
        response = xmlrpclib.dumps(xmlrpclib.Fault(faultCode,faultString))
	self.sendResult(response)
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

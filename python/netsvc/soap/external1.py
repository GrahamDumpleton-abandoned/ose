# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import string

from netrpc.common import _escape

# The SOAP module is from pywebsvcs, found at:
#    http://sourceforge.net/projects/pywebsvcs

import SOAP

class xsdType(SOAP.anyType):
  def __init__(self,data,name):
    SOAP.anyType.__init__(self,data,name)
    self._type = name

class xsd2Type(SOAP.anyType):
  _validURIs = (SOAP.NS.XSD2,)
  def __init__(self,data,name):
    SOAP.anyType.__init__(self,data,name)
    self._type = name

class xsd3Type(SOAP.anyType):
  _validURIs = (SOAP.NS.XSD3,)
  def __init__(self,data,name):
    SOAP.anyType.__init__(self,data,name)
    self._type = name

class _RpcServlet(netsvc.HttpServlet,netsvc.Service):

  def __init__(self,session,binding):
    netsvc.HttpServlet.__init__(self,session)
    netsvc.Service.__init__(self,"","")
    self._binding = binding
    self._method = None
    self._content = []
    self._contentLength = 0

  def destroyServlet(self):
    netsvc.HttpServlet.destroyServlet(self)
    netsvc.Service.destroyReferences(self)

  def decodeValue(self,name,string):
    # Builtin types already decoded by C++.
    # Binary special case as content decoded.
    if name == "xsd:base64Binary":
      return SOAP.binaryType(string)
    elif name == "xsd:boolean":
      return xsdType(string,"boolean")
    elif name == "xsd:date":
      return xsdType(string,"date")
    elif name == "xsd:dateTime":
      return xsdType(string,"timeInstant")
    elif name == "xsd:time":
      return xsdType(string,"time")
    elif name == "xsd:duration":
      return xsdType(string,"timeDuration")
    elif name[0:4] == "xsd:":
      return xsdType(string,name[4:])
    return netsvc.Service.decodeValue(self,name,string)

  def encodeObject(self,object):
    # Boolean, date types etc undistinguishable.
    if isinstance(object,SOAP.arrayType):
      return (None,list(object))
    elif isinstance(object,SOAP.typedArrayType):
      return (None,list(object))
    elif isinstance(object,SOAP.structType):
      return (None,object._asdict)
    return netsvc.Service.encodeObject(self,object)

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
        self._content.append(content)
	self._contentLength = self._contentLength + len(content)
	if self._contentLength < self.contentLength():
	  return
	if self._contentLength > self.contentLength():
	  error = netsvc.SERVER_REQUEST_DECODING_ERROR
	  description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	  origin = "netsvc.soap"
	  details = "Invalid content length."
	  raise netsvc.ServiceFailure(error,description,origin,details)
	content = string.join(self._content,"")
        (request,header,body,attrs) = SOAP.parseSOAPRPC(
            content,header=1,body=1,attrs=1)
	self._method = request._name
	params = request._aslist
      except netsvc.ServiceFailure:
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
	detail = {}
	detail["error"] = exception.error
	detail["description"] = exception.description
	detail["origin"] = exception.origin
	detail["details"] = exception.details
	body = SOAP.buildSOAP(SOAP.faultType(
	    "%s:Client"%SOAP.NS.ENV_T,"Request Failed",detail))
	self.sendResult(500,body)
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
      body = SOAP.buildSOAP(kw={'%sResponse'%self._method:{'Result':params}})
      self.sendResult(200,body)
    except netsvc.ServiceFailure,exception:
      try:
	detail = {}
	detail["error"] = exception.error
	detail["description"] = exception.description
	detail["origin"] = exception.origin
	detail["details"] = exception.details
	if exception.origin == "netsvc" and \
	    (exception.error == netsvc.SERVER_METHOD_UNAVAILABLE or \
	    exception.error == netsvc.SERVER_PARAMETERS_INCORRECT or \
	    exception.error == netsvc.SERVER_REQUEST_DECODING_ERROR):
	  body = SOAP.buildSOAP(SOAP.faultType(
	      "%s:Client"%SOAP.NS.ENV_T,"Request Failed",detail))
	else:
	  body = SOAP.buildSOAP(SOAP.faultType(
	      "%s:Server"%SOAP.NS.ENV_T,"Request Failed",detail))
	self.sendResult(500,body)
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


# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import string
import StringIO

from netrpc.common import _escape
import netrpc.soap

# The ZSI module is from Zolera, found at:
#    http://www.zolera.com

SCHEMA_URI = "http://www.dscpl.com.au/schemas/OSE/"

FAULT_DETAIL = """<OSE:ServiceFailure xmlns:OSE="%s">
<OSE:error>%d</OSE:error>
<OSE:description>%s</OSE:description>
<OSE:origin>%s</OSE:origin>
<OSE:details>%s</OSE:details>
</OSE:ServiceFailure>
"""

import ZSI

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

	ps = ZSI.ParsedSoap(content)

	data = ZSI._child_elements(ps.body_root)
        if len(data) == 0:
	  params = []
        else:
	  try:
	    tc = ZSI.TC.Any()
	    params = []
	    for e in data:
	      params.append(tc.parse(e,ps))
	  except ZSI.EvaluateException:
	    raise

	self._method = ps.body_root.localName

      except netsvc.ServiceFailure:
	raise
      except ZSI.EvaluateException:
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

    except ZSI.EvaluateException, e:
      fault = ZSI.FaultFromZSIException(e)
      self.sendResult(500,fault.AsSOAP())
    except netsvc.ServiceFailure,exception:
      try:
	detail = FAULT_DETAIL % (SCHEMA_URI,
	    exception.error,_escape(exception.description),
	    _escape(exception.origin),_escape(exception.details))
	fault = ZSI.Fault(ZSI.Fault.Client,
	    "Request Failed",None,detail,None)
	body = fault.AsSOAP()
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

      params = [params]
      reply = StringIO.StringIO()
      ZSI.SoapWriter(reply).serialize(params,
	  ZSI.TC.Any(aslist=1,pname=self._method + 'Response'))

      self.sendResult(200,reply.getvalue())
    except netsvc.ServiceFailure,exception:
      try:
	detail = FAULT_DETAIL % (SCHEMA_URI,
	    exception.error,_escape(exception.description),
	    _escape(exception.origin),_escape(exception.details))

	if exception.origin == "netsvc" and \
	    (exception.error == netsvc.SERVER_METHOD_UNAVAILABLE or \
	    exception.error == netsvc.SERVER_PARAMETERS_INCORRECT or \
	    exception.error == netsvc.SERVER_REQUEST_DECODING_ERROR):
	  fault = ZSI.Fault(ZSI.Fault.Client,
	      "Request Failed",None,detail,None)
	else:
	  fault = ZSI.Fault(ZSI.Fault.Server,
	      "Request Failed",None,detail,None)

	body = fault.AsSOAP()
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


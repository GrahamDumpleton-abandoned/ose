# COPYRIGHT 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc

from netrpc.common import _escape

logger = netsvc.Logger()

_servlet = {}

try:
  import external1
except:
  pass
else:
  _servlet["pywebsvcs/SOAP"] = external1._RpcServlet

try:
  import external2
except:
  pass
else:
  _servlet["PythonWare/soaplib"] = external2._RpcServlet

try:
  import external3
except:
  pass
else:
  _servlet["pywebsvcs/ZSI"] = external3._RpcServlet


_default = "pywebsvcs/ZSI"


class RpcGateway(netsvc.HttpServer,netsvc.Monitor):

  def __init__(self,group,variant=_default):
    netsvc.HttpServer.__init__(self)
    netsvc.Monitor.__init__(self)
    self._members = {}
    self._bindings = {}
    self._variant = variant
    self.subscribeServiceGroup(self.register,group)

  def register(self,binding,group,status):
    serviceName = binding.serviceName()
    agentIdentity = binding.agentIdentity()
    if status == netsvc.SERVICE_AVAILABLE:
      if not self._bindings.has_key(agentIdentity):
	self._bindings[agentIdentity] = binding
	if self._members.has_key(serviceName):
	  self._members[serviceName].append(agentIdentity)
	else:
	  self._members[serviceName] = [agentIdentity]
    else:
      if self._bindings.has_key(agentIdentity):
        del self._bindings[agentIdentity]
	members = self._members[serviceName]
	del members[members.index(agentIdentity)]
	if len(members) == 0:
	  del self._members[serviceName]

  def servlet(self,session):
    if self._members.has_key(session.servletPath()):
      agentIdentity = self._members[session.servletPath()][0]
      binding = self._bindings[agentIdentity]
      try:
	servlet = _servlet[self._variant]
      except:
	message = "SOAP gateway %s not available." % `self._variant`
	logger.notify(netsvc.LOG_WARNING,message)
	return netsvc.ErrorServlet(session,501)
      else:
	return servlet(session,binding)
    return netsvc.ErrorServlet(session,404)

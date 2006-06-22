# COPYRIGHT 2006 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc


class RpcServlet:

  def __init__(self,session,binding):
    self._HttpServlet = netsvc._ose.new_JsonRpcServlet(self,
        session.instance(),binding)

  def _destroyServlet(self):
    self._HttpServlet = None


class RpcGateway(netsvc.HttpServer):

  def __init__(self,selection,type=netsvc.SERVICE_GROUP):
    inner = netsvc._ose.new_JsonRpcGateway(self,selection,type)
    netsvc.HttpServer.__init__(self,inner)


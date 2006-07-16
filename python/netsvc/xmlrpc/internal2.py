# COPYRIGHT 2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc


class _RpcServlet:

  def __init__(self,session,binding):
    self._HttpServlet = netsvc._ose.new_XmlRpcServlet(self,
        session.instance(),binding)

  def _destroyServlet(self):
    self._HttpServlet = None

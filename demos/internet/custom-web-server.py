import netsvc
import signal
import string


class HttpServlet(netsvc.HttpServlet):

  def __init__(self,session):
    netsvc.HttpServlet.__init__(self,session)

  def processRequest(self):
    if self.requestMethod() != "GET":
      self.sendError(400)
    else:
      self.sendResponse(200)
      self.sendHeader("Content-Type","text/plain")
      self.endHeaders()
      self.sendContent("Hi there.")
      self.endContent()


class Service(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"echo")
    self.exportMethod(self.echo)

  def echo(self,*args):
    return args


class HttpServer(netsvc.HttpServer):

  def __init__(self):
    netsvc.HttpServer.__init__(self)

  def servlet(self,session):
    if session.servletPath() == "":
      return HttpServlet(session)
    elif session.servletPath() == "echo":
      return netsvc.EchoServlet(session)
    elif session.servletPath() == "file":
      return netsvc.FileServlet(session,"/etc/protocols","text/plain")
    elif session.servletPath() == "call":
      return netsvc.RpcServlet(session,service.serviceBinding())
    return netsvc.ErrorServlet(session,404)


if __name__ == "__main__":

  dispatcher = netsvc.Dispatcher()
  dispatcher.monitor(signal.SIGINT)

  service = Service()

  daemon = netsvc.HttpDaemon(8000)
  server = HttpServer()
  daemon.attach("/",server)
  daemon.start()

  dispatcher.run()

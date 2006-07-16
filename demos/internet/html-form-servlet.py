import netsvc
import signal
import string
import urllib
import cgi


class TestServlet(netsvc.FormServlet):

  def __init__(self,session):
    netsvc.FormServlet.__init__(self,session)
    self._batch = None
    self._total = None
    self._count = 0
    self._job = netsvc.Job(self.generateContent)

  def destroyServlet(self):
    netsvc.FormServlet.destroyServlet(self)
    self._job.cancel()
    self._job = None

  def handleRequest(self):
    try:
      self._batch = int(self.field("batch"))
      self._total = int(self.field("total"))
    except:
      self.sendResponse(200)
      self.sendHeader("Content-Type","text/html")
      self.endHeaders()
      servlet = urllib.quote(self.requestPath())
      values = { "servlet": servlet }
      self.sendContent(FORM%values)
      self.endContent()
    else:
      self.sendResponse(200)
      self.sendHeader("Content-Type","text/plain")
      self.endHeaders()
      self.monitorCongestion(self.clientCongestion)
      self._job.schedule(netsvc.IDLE_JOB)

  def generateContent(self):
    content = []
    for i in range(0,self._batch):
      self._count = self._count + 1
      content.append(string.zfill(self._count,60))
    content.append("")
    self.sendContent(string.join(content,"\n"))
    self._total = self._total - 1
    if self._total <= 0:
      self.ignoreCongestion()
      self.endContent()
    else:
      self.flushContent()
      self._job.schedule(netsvc.IDLE_JOB)

  def clientCongestion(self,status,pending):
    if status == netsvc.CONNECTION_CLEARED:
      self._job.reset()
      self._job.schedule(netsvc.IDLE_JOB)
    elif status == netsvc.CONNECTION_BLOCKED:
      self._job.cancel()


class HttpServer(netsvc.HttpServer):

  def __init__(self):
    netsvc.HttpServer.__init__(self)

  def servlet(self,session):
    if session.servletPath() == "":
      return TestServlet(session)
    return netsvc.ErrorServlet(session,404)


FORM = """
<html>
<body>
<form method="POST" action="%(servlet)s">
<pre>
Batch: <input type="text" name="batch" value="10"></br>
Total: <input type="text" name="total" value="10"></br>
<input type="submit" name="submit" value="Submit">
</pre>
</form>
</body>
</html>
"""


__servlet__ = TestServlet


if __name__ == "__main__":

  dispatcher = netsvc.Dispatcher()
  dispatcher.monitor(signal.SIGINT)

  daemon = netsvc.HttpDaemon(8000)
  server = HttpServer()
  daemon.attach("/",server)
  daemon.start()

  dispatcher.run()

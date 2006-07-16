import netsvc
import signal
import os

class HttpDaemon(netsvc.HttpDaemon):

  def __init__(self,port):
    netsvc.HttpDaemon.__init__(self,port)

  def authorise(self,host):
    if host != "127.0.0.1":
      return netsvc.False
    return netsvc.True

if __name__ == "__main__":

  dispatcher = netsvc.Dispatcher()
  dispatcher.monitor(signal.SIGINT)

  daemon = HttpDaemon(8000)
  filesrvr = netsvc.FileServer(os.getcwd())
  daemon.attach("/",filesrvr)
  daemon.start()

  dispatcher.run()

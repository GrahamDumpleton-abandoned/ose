import netsvc
import signal
import os

class FileServer(netsvc.FileServer):

  def __init__(self,directory):
    netsvc.FileServer.__init__(self,directory)

  def authorise(self,login,password):
    if login != "spirit" or password != "buddha":
      return netsvc.False
    return netsvc.True

if __name__ == "__main__":

  dispatcher = netsvc.Dispatcher()
  dispatcher.monitor(signal.SIGINT)

  daemon = netsvc.HttpDaemon(8000)
  filesrvr = FileServer(os.getcwd())
  daemon.attach("/",filesrvr)
  daemon.start()

  dispatcher.run()

import netsvc
import signal
import os

if __name__ == "__main__":

  dispatcher = netsvc.Dispatcher()
  dispatcher.monitor(signal.SIGINT)

  daemon = netsvc.HttpDaemon(8000)
  filesrvr = netsvc.FileServer(os.getcwd())
  daemon.attach("/",filesrvr)
  daemon.start()

  dispatcher.run()

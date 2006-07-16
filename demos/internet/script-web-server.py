import netsvc
import signal
import string
import os

# Use following URL:
#    http://localhost:8000/scripts/html-form-servlet.html
#    http://localhost:8000/scripts/html-form-servlet.py
#    http://localhost:8000/scripts/html-form-servlet

if __name__ == "__main__":

  dispatcher = netsvc.Dispatcher()
  dispatcher.monitor(signal.SIGINT)

  daemon = netsvc.HttpDaemon(8000)

  server = netsvc.FileServer(os.getcwd())
  server.plugin(".py",netsvc.PythonPlugin(),".html")
  server.plugin(".py",netsvc.PythonPlugin(),"")
  server.plugin(".py",netsvc.PythonPlugin())

  daemon.attach("/scripts",server)
  daemon.start()

  dispatcher.run()

import netsvc
import sys
import string
import socket
import signal

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: proxy-service -connect server [ -domain name ]"

try:
  i = 0
  while i < count:
    if args[i] == "-connect":
      server = args[i+1]
      i = i + 2
    elif args[i] == "-domain":
      domain = args[i+1]
      i = i + 2
    else:
      usage()
      sys.exit(1)
except:
  usage()
  sys.exit(1)

if server == None:
  usage()
  sys.exit(1)


class Proxy(netsvc.Service):

  # Note this doesn't use non blocking sockets
  # and so may block on a write. Left as an
  # exercise to add non blocking writes. If
  # done, need to think about how you notify
  # the client about congestion and get it to
  # back off if necessary.

  def __init__(self,name="proxy",audience="*"):
    netsvc.Service.__init__(self,name,audience)
    self._count = 0
    self._active = {}
    self._socket = {}
    self._client = {}
    self.exportMethod(self.connect)
    self.exportMethod(self.write)
    self.exportMethod(self.close)

  def monitor(self,binding,group,status):
    if status == netsvc.SERVICE_WITHDRAWN and group == None:
      client = binding.serviceAddress()
      for handle in self._client[client]:
        self.close(handle)

  def connect(self,address):
    request = self.currentRequest()
    if request == None:
      return ""
    client = request.sender()
    binding = self.lookupServiceAddress(client)
    if binding == None:
      self.abortResponse(1,"Client Not Visible.")
    sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    try:
      sock.connect(tuple(address))
    except:
      sock.close()
      details = netsvc.exceptionDescription()
      self.abortResponse(2,"Connection Failed.","",details)
    self.subscribeSocket(self.read,sock.fileno(),netsvc.SOCKET_POLLIN)
    self._count = self._count + 1
    handle = "(%s,%d)" % (self.agentIdentity(),sock.fileno())
    self._active[handle] = (sock,binding)
    self._socket[sock.fileno()] = handle
    if not self._client.has_key(client):
      self.subscribeServiceAddress(self.monitor,client)
      self._client[client] = []
    self._client[client].append(handle)
    return handle

  def read(self,fileno,event):
    if self._socket.has_key(fileno):
      handle = self._socket[fileno]
      sock,binding = self._active[handle]
      client = binding.serviceAddress()
      if event == netsvc.SOCKET_POLLIN:
	data = sock.recv(1024)
	self.sendReport(binding,handle,data)
	if len(data) == 0:
	  self.unsubscribeSocket(fileno)
	  del self._socket[fileno]
	  del self._active[handle]
	  index = self._client[client].index(handle)
	  del self._client[client][index]
	  if len(self._client[client]) == 0:
	    del self._client[client]
	    self.unsubscribeServiceAddress(client)
	  sock.close()

  def write(self,handle,data):
    if not self._active.has_key(handle):
      self.abortResponse(3,"No Such Connection.")
    sock,binding = self._active[handle]
    try:
      sock.send(data)
    except:
      details = netsvc.exceptionDescription()
      self.abortResponse(4,"Write Failed.","",details)
      self.close(handle)

  def close(self,handle):
    if not self._active.has_key(handle):
      self.abortResponse(3,"No Such Connection.")
    sock,binding = self._active[handle]
    client = binding.serviceAddress()
    self.unsubscribeSocket(sock.fileno())
    del self._socket[sock.fileno()]
    del self._active[handle]
    index = self._client[client].index(handle)
    del self._client[client][index]
    if len(self._client[client]) == 0:
      del self._client[client]
      self.unsubscribeServiceAddress(client)
    self.sendReport(binding,handle,"")
    sock.close()


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

proxy = Proxy()

dispatcher.run()


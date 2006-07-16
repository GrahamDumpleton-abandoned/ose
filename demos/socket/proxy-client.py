import netsvc
import sys
import string
import signal

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: proxy-client -connect server [ -domain name ]"

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


class Client(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"client")
    self._binding = None
    self._proxy = None
    self._handle = None
    self.scheduleAction(self.start,netsvc.STANDARD_JOB,"start")

  def start(self,tag):
    bindings = self.lookupServiceName("proxy")
    if len(bindings) != 0:
      print "start"
      self._binding = bindings[0]
      self._proxy = self.serviceEndPoint(self._binding)
      address = self._binding.serviceAddress()
      self.subscribeServiceAddress(self.monitor,address)
      id = self._proxy.connect(("localhost",7))
      self.processResponse(self.active,id)
      self.processFailure(self.failure,id,10)
    else:
      print "timer"
      self.startTimer(self.start,1,"start")

  def monitor(self,binding,group,status):
    print "monitor",status
    if group == None and status == netsvc.SERVICE_WITHDRAWN:
      if self._binding != None:
	address = self._binding.serviceAddress()
        self.unsubscribeServiceAddress(address)
	self.cancelAllTimers()
	self._binding = None
	self._proxy = None

  def active(self,handle):
    print "active",handle
    self._handle = handle
    self.monitorReports(self.data,"proxy",handle)
    self.startTimer(self.timeout,1,"send")
    self.startTimer(self.timeout,20,"close")

  def data(self,service,subjectName,content):
    print "report"
    sys.stdout.write(content)
    if content == "":
      if self._binding != None:
	self.unsubscribeServiceAddress(address)
	self.cancelAllTimers()
	self._binding = None
	self._proxy = None

  def timeout(self,name):
    print "timeout"
    if name == "send":
      if self._binding != None:
	id = self._proxy.write(self._handle,str(netsvc.DateTime())+"\n")
	self.processResponse(self.complete,id)
	self.processFailure(self.failure,id,2)
	self.startTimer(self.timeout,1,"send")
    else:
      print "close"
      self.cancelTimer("send")
      self._proxy.close(self._handle)
      self.ignoreReports("proxy",self._handle)
      address = self._binding.serviceAddress()
      self.unsubscribeServiceAddress(address)
      self._binding = None
      self._proxy = None
      self._handle = None

  def complete(self):
    print "write"

  def failure(self):
    print self.currentFailure()


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

client = Client()

dispatcher.run()


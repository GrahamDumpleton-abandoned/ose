import netsvc
import sys
import signal
import string

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: request-uptime -connect server [ -domain name ]"

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
    netsvc.Service.__init__(self,"")
    self.startTimer(self.call,1,"1")

  def call(self,name):
    service = self.serviceEndPoint("SERVICE")
    if service != None:
      id = service.uptime()
      self.processResponse(self.result,id)
    self.startTimer(self.call,1,"1")

  def result(self,uptime):
    print uptime

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

client = Client()

dispatcher.run()

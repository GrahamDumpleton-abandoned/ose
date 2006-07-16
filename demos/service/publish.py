import netsvc
import sys
import signal
import string
import random

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: publish -connect server [ -domain name ]"

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

class Publisher(netsvc.Service):
  
  def __init__(self):
    netsvc.Service.__init__(self,"SEED")
    self._count = 0
    time = netsvc.DateTime()
    data = { "time": time }
    self.publishReport("init",data,-1)
    self.startTimer(self.publish,1,"1")

  def publish(self,name):
    self._count = self._count + 1
    time = netsvc.DateTime()
    value = int(0xFFFF*random.random())
    data = { "time": time, "count": self._count, "value": value }
    self.publishReport("next",data)
    self.startTimer(self.publish,1,"1")

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

publisher = Publisher()

dispatcher.run()

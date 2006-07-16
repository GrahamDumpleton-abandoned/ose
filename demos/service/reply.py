import netsvc
import sys
import signal
import string
import time

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: reply -connect server [ -domain name ]"

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

class Service(netsvc.Service):
  
  def __init__(self):
    netsvc.Service.__init__(self,"SERVICE")
    self.joinGroup("PUBLIC")
    self._start = time.time()
    self.exportMethod(self.exportedMethods)
    self.exportMethod(self.uptime)
    self.exportMethod(self.multiply)
    self.exportMethod(self.wait)
    self.exportMethod(self.echo)

  def unexpectedMessage(self,sender,messageId,description,content,
      contentType,sequenceNumber):
    print sender
    print messageId
    print content
    print contentType
    print sequenceNumber

  def uptime(self):
    now = time.time()
    life = int(now - self._start)
    seconds = life % 60
    life = life / 60
    minutes = life % 60
    life = life / 60
    hours = life % 60
    life = life / 60
    days = life / 24
    life = "P%dDT%dH%dM%dS" % (days,hours,minutes,seconds)
    return netsvc.Duration(life)

  def multiply(self,x,y):
    return x*y

  def wait(self,period):
    id = self.conversationId()
    self.startTimer(self.resumeResponse,period,id)
    self.suspendResponse(self.echo)

  def echo(self,value):
    return value

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

service = Service()

dispatcher.run()

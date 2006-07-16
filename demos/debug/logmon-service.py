import netsvc
import os
import random
import sys
import signal
import string

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: logmon-service -connect server [ -domain name ]"

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


class Monitor(netsvc.Service):

  def __init__(self):
    name = "logmon@%s:%d" % (os.uname()[1],os.getpid())
    netsvc.Service.__init__(self,name)
    self._logger = netsvc.Logger()
    self._channels = {}

  def callback(self,channel,level,message):
    agent = channel[1:-1]
    report = {}
    report["agent"] = agent
    report["level"] = level
    report["message"] = message
    self.publishReport(agent,report,0)

  def handleSubscription(self,subscription):
    agent = subscription.subject()
    channel = "(%s)" % agent
    if subscription.status() == netsvc.SUBSCRIPTION_REQUESTED:
      if len(agent) != 0:
	subscriber = subscription.subscriber().agentIdentity()
	if not self._channels.has_key(channel):
	  self._channels[channel] = []
	  self._logger.monitorChannel(channel,self.callback)
	self._channels[channel].append(subscriber)
    else:
      if self._channels.has_key(channel):
        subscriber = subscription.subscriber().agentIdentity()
	if subscriber in self._channels[channel]:
	  index = self._channels[channel].index(subscriber)
	  del self._channels[channel][index]
	  if len(self._channels[channel]) == 0:
	    del self._channels[channel]
	    self._logger.monitorChannel(channel,None)


logger = netsvc.Logger()

class Publisher(netsvc.Service):
  
  def __init__(self):
    netsvc.Service.__init__(self,"SEED")
    self._count = 0
    self._channel = "(%s)" % self.agentIdentity()
    time = netsvc.DateTime()
    data = { "time": time }
    self.publishReport("init",data,-1)
    self.startTimer(self.publish,1,"1")

  def debug(self,message):
    logger.notifyChannel(self._channel,netsvc.LOG_DEBUG,message)

  def publish(self,name):
    self.debug("PUBLISH %d" % self._count)
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

monitor = Monitor()
publisher = Publisher()

dispatcher.run()


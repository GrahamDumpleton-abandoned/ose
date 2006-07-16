import netsvc
import signal
import random
import time

class Service(netsvc.Service):
  
  def __init__(self):
    netsvc.Service.__init__(self,"SERVICE")
    self.joinGroup("PUBLIC")
    self._start = time.time()
    self.exportMethod(self.uptime)
    self.exportMethod(self.multiply)

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


class Client1(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"")
    self.startTimer(self.call,1,"1")

  def call(self,name):
    service = self.serviceEndPoint("SERVICE")
    try:
      id = service.uptime()
      self.processResponse(self.result,id)
    except netsvc.ServiceUnavailable:
      pass
    self.startTimer(self.call,1,"1")

  def result(self,uptime):
    print uptime


class Client2(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"")
    self.startTimer(self.call,1,"1")

  def call(self,name):
    service = self.serviceEndPoint("SERVICE")
    x = int(1000*random.random())
    y = int(1000*random.random())
    try:
      id = service.multiply(x,y)
      self.processResponse(self.result,id)
    except netsvc.ServiceUnavailable:
      pass
    self.startTimer(self.call,1,"1")

  def result(self,result):
    print result


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

service = Service()
client1 = Client1()
client2 = Client2()

dispatcher.run()

import signal
import netsvc


class Client(netsvc.Service):

  def __init__(self,database):
    netsvc.Service.__init__(self,"","")
    self._database = database
    self._cursor = None
    self.subscribeServiceName(self.start,self._database)

  def start(self,binding,status):
    if status == netsvc.SERVICE_AVAILABLE:
      if binding.serviceName() == self._database:
	service = self.serviceEndPoint(self._database)
	if service != None:
	  id = service.cursor(20)
	  self.monitorResponse(self.sendQueries,id,10)
	else:
	  print "service %s unavailable" % `self._database`
	  self.startTimer(self.start,10,"")

  def sendQueries(self,name):
    if self.failureOccurred():
      print self.currentFailure()
    else:
      self._cursor = "%s/%s" % (self._database,name)
      service = self.serviceEndPoint(self._cursor)
      if service != None:
	service.execute("show tables")
	id = service.fetchall()
	self.monitorResponse(self.processTableList,id,10)
      else:
	print "service %s unavailable" % `self._cursor`
	self.startTimer(self.start,10,"")

  def processTableList(self,rows):
    if self.failureOccurred():
      print self.currentFailure()
    else:
      service = self.serviceEndPoint(self._cursor)
      if service != None:
	for row in rows:
	  id = service.execute("select * from " + row[0])
	  self.monitorResponse(self.processResult,id,10)
	  id = service.description()
	  self.monitorResponse(self.processResult,id,10)
	  id = service.rowcount()
	  self.monitorResponse(self.processResult,id,10)
	  id = service.fetchall()
	  self.monitorResponse(self.processResult,id,10)
	id = service.close()
	self.monitorResponse(self.processResult,id,10)
      else:
	print "service %s unavailable" % `self._cursor`

  def processResult(self,result):
    if self.failureOccurred():
      print self.currentFailure()
    else:
      print str(result)


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

client = Client("test")

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT)
exchange.connect("localhost",11111)

dispatcher.run()

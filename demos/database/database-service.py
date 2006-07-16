import MySQLdb
import signal
import netsvc


class Cursor(netsvc.Service):

  def __init__(self,name,cursor,timeout):
    netsvc.Service.__init__(self,name)
    self.joinGroup("database-services")
    self._cursor = cursor
    self._timeout = timeout
    self._restart()
    self.exportMethod(self.execute)
    self.exportMethod(self.executemany)
    self.exportMethod(self.description)
    self.exportMethod(self.rowcount)
    self.exportMethod(self.fetchone)
    self.exportMethod(self.fetchmany)
    self.exportMethod(self.fetchall)
    self.exportMethod(self.arraysize)
    self.exportMethod(self.close)

  def encodeObject(self,object):
    if hasattr(MySQLdb,"DateTime"):
      if type(object) == MySQLdb.DateTimeType:
	return ("xsd:string",object.strftime())
      elif type(object) == MySQLdb.DateTimeDeltaType:
	return ("xsd:string",str(object))
    return netsvc.Service.encodeObject(self,object)

  def executeMethod(self,name,method,params):
    try:
      return netsvc.Service.executeMethod(self,name,method,params)
    except MySQLdb.ProgrammingError,exception:
      self.abortResponse(1,"Programming Error","db",str(exception))
    except MySQLdb.Error,(error,description):
      self.abortResponse(error,description,"mysql")

  def _restart(self):
    self.cancelTimer("idle")
    self.startTimer(self._expire,self._timeout,"idle")

  def _expire(self,name):
    if name == "idle":
      self.close()

  def execute(self,query,args=None):
    result = self._cursor.execute(query,args)
    self._restart()
    return result

  def executemany(self,query,args=None):
    result = self._cursor.executemany(query,args)
    self._restart()
    return result

  def description(self):
    self._restart()
    return self._cursor.description

  def rowcount(self):
    self._restart()
    return self._cursor.rowcount

  def fetchone(self):
    result = self._cursor.fetchone()
    self._restart()
    return result

  def fetchmany(self,size=None):
    if size == None:
      size = self._cursor.arraysize
    result = self._cursor.fetchmany(size)
    self._restart()
    return result

  def fetchall(self):
    result = self._cursor.fetchall()
    self._restart()
    return result

  def arraysize(self):
    self._restart()
    return self._cursor.arraysize

  def close(self):
    self._cursor.close()
    self.cancelTimer("idle")
    self.destroyReferences()
    return 0


class Database(netsvc.Service):

  def __init__(self,name,**kw):
    netsvc.Service.__init__(self,name)
    self._name = name
    self.joinGroup("database-services")
    self._database = MySQLdb.connect(**kw)
    self._cursors = 0
    self.exportMethod(self.execute)
    self.exportMethod(self.executemany)
    self.exportMethod(self.cursor)

  def encodeObject(self,object):
    if hasattr(MySQLdb,"DateTime"):
      if type(object) == MySQLdb.DateTimeType:
	return ("xsd:string",object.strftime())
      elif type(object) == MySQLdb.DateTimeDeltaType:
	return ("xsd:string",str(object))
    return netsvc.Service.encodeObject(self,object)

  def executeMethod(self,name,method,params):
    try:
      return netsvc.Service.executeMethod(self,name,method,params)
    except MySQLdb.ProgrammingError,exception:
      self.abortResponse(1,"Programming Error","db",str(exception))
    except MySQLdb.Error,(error,description):
      self.abortResponse(error,description,"mysql")

  def execute(self,query,args=None):
    cursor = self._database.cursor()
    cursor.execute(query,args)
    if cursor.description == None:
      return cursor.rowcount
    return cursor.fetchall()

  def executemany(self,query,args=None):
    cursor = self._database.cursor()
    cursor.executemany(query,args)
    if cursor.description == None:
      return cursor.rowcount
    return cursor.fetchall()

  def cursor(self,timeout):
    self._cursors = self._cursors + 1
    name = "%s/%d" % (self._name,self._cursors)
    cursor = self._database.cursor()
    Cursor(name,cursor,timeout)
    child = "%d" % self._cursors
    return child


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

server = Database("test",db="test")

httpd = netsvc.HttpDaemon(8000)
rpcgw = netsvc.RpcGateway("database-services")
httpd.attach("/database",rpcgw)
httpd.start()

exchange = netsvc.Exchange(netsvc.EXCHANGE_SERVER)
exchange.listen(11111)

dispatcher.run()

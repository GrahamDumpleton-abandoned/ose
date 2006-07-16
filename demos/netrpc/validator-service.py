import signal
import netsvc
import netsvc.xmlrpc

# Use following URL:
#    NET-RPC --> http://localhost:8000/netrpc/validator
#    XML-RPC --> http://localhost:8000/xmlrpc/validator

class Validator(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"validator")
    self.joinGroup("test")
    self.exportMethod(self.arrayOfStructsTest,
        "validator1.arrayOfStructsTest")
    self.exportMethod(self.countTheEntities,
        "validator1.countTheEntities")
    self.exportMethod(self.easyStructTest,
        "validator1.easyStructTest")
    self.exportMethod(self.echoStructTest,
        "validator1.echoStructTest")
    self.exportMethod(self.manyTypesTest,
        "validator1.manyTypesTest")
    self.exportMethod(self.moderateSizeArrayCheck,
        "validator1.moderateSizeArrayCheck")
    self.exportMethod(self.nestedStructTest,
        "validator1.nestedStructTest")
    self.exportMethod(self.simpleStructReturnTest,
        "validator1.simpleStructReturnTest")

  def arrayOfStructsTest(self,array):
    total = 0
    for struct in array:
      total = total + struct["curly"]
    return total

  def countTheEntities(self,string):
    totals = { '<':0, '>':0, '&':0, "'": 0, '"':0 }
    for c in string:
      if totals.has_key(c):
        totals[c] = totals[c] + 1
    result = {
        "ctLeftAngleBrackets":totals['<'],
        "ctRightAngleBrackets":totals['>'],
        "ctAmpersands":totals['&'],
        "ctApostrophes":totals["'"],
        "ctQuotes":totals['"']
    }
    return result

  def easyStructTest(self,struct):
    total = struct["moe"] + struct["larry"] + struct["curly"]
    return total

  def echoStructTest(self,struct):
    return struct

  def manyTypesTest(self,a1,a2,a3,a4,a5,a6):
    return [a1,a2,a3,a4,a5,a6]

  def moderateSizeArrayCheck(self,array):
    return array[0]+array[-1]

  def nestedStructTest(self,struct):
    entry = struct["2000"]["04"]["01"]
    total = entry["moe"] + entry["larry"] + entry["curly"]
    return total

  def simpleStructReturnTest(self,value):
    struct = {}
    struct["times10"] = 10*value
    struct["times100"] = 100*value
    struct["times1000"] = 1000*value
    return struct


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

validator = Validator()

httpd = netsvc.HttpDaemon(8000)

rpcgw1 = netsvc.RpcGateway("test")
httpd.attach("/netrpc",rpcgw1)

rpcgw2 = netsvc.xmlrpc.RpcGateway("test")
httpd.attach("/xmlrpc",rpcgw2)

httpd.start()

dispatcher.run()

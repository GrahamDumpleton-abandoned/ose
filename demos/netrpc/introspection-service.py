import signal
import netsvc
import netsvc.xmlrpc
import netsvc.soap

# Use following URLs:
#    NET-RPC --> http://localhost:8000/netrpc/service-1
#    NET-RPC --> http://localhost:8000/netrpc/service-2
#    XML-RPC --> http://localhost:8000/xmlrpc/service-1
#    XML-RPC --> http://localhost:8000/xmlrpc/service-2
#    SOAP --> http://localhost:8000/soap/service-1
#    SOAP --> http://localhost:8000/soap/service-2

class IntrospectionService1(netsvc.Service):

  def __init__(self,name):
    netsvc.Service.__init__(self,name)
    self.joinGroup("introspect")
    self.exportMethod(self._listMethods,"system.listMethods")
    self.exportMethod(self._methodHelp,"system.methodHelp")
    self.exportMethod(self._methodSignature,"system.methodSignature")

  def _listMethods(self):
    "Returns a list containing the names of all the methods which " \
    "have been exported by this service agent and are thus callable."
    return self.exportedMethods()

  def _methodHelp(self,name):
    "Returns a string containing any documentation which has been " \
    "defined for the method identified by the argument 'name'. If no " \
    "documentation has been defined, an empty string is returned."
    callback = self.lookupMethod(name)
    if callback != None:
      if callback.__doc__ == None:
        return ""
      return callback.__doc__
    return ""

  def _methodSignature(self,name):
    "Note that this implementation does not strictly adhere to the " \
    "introspection API as it doesn't return types but the Python " \
    "function parameter names. It also shows default arguments and " \
    "any vararg style argument. The benefit of this implementation " \
    "however is that the user doesn't need to define the signature " \
    "manually. The function returns a description of the signature " \
    "for the method called 'name'. The returned value will consist of " \
    "a list containing another list. The first element of the inner list " \
    "describes the result, but in this implementation will always be " \
    "'value'. The subsequent elements of the inner list will be the " \
    "names of the method arguments. If any of the method arguments have " \
    "default values, the default value will be shown as 'arg=value'. " \
    "Any varargs type argument will be shown as '*arg'."
    callback = self.lookupMethod(name)
    if callback != None:
      argcount = callback.im_func.func_code.co_argcount
      argnames = list(callback.im_func.func_code.co_varnames[1:argcount])
      argdflts = callback.im_func.func_defaults
      if argdflts != None and len(argdflts) > 0:
        off = argcount - len(argdflts) - 1
        for i in range(0,len(argdflts)):
          argnames[off+i] = "%s=%s" % (argnames[off+i],`argdflts[i]`)
      argnames.insert(0,"value")
      if callback.im_func.func_code.co_flags & 4:
        argnames.append("*"+callback.im_func.func_code.co_varnames[argcount])
      return [argnames]
    return "undef"


class IntrospectionService2(netsvc.Service):

  def __init__(self,name):
    netsvc.Service.__init__(self,name)
    self.joinGroup("introspect")
    self._methodSignatures = {}
    self.exportMethod(self._listMethods,"system.listMethods")
    self.exportMethod(self._methodHelp,"system.methodHelp")
    self.exportMethod(self._methodSignature,"system.methodSignature")

  def exportMethod(self,callback,name=None,signatures=None):
    netsvc.Service.exportMethod(self,callback,name)
    self._methodSignatures[name] = signatures

  def _listMethods(self):
    "Returns a list containing the names of all the methods which " \
    "have been exported by this service agent and are thus callable."
    return self.exportedMethods()

  def _methodHelp(self,name):
    "Returns a string containing any documentation which has been " \
    "defined for the method identified by the argument 'name'. If no " \
    "documentation has been defined, an empty string is returned."
    callback = self.lookupMethod(name)
    if callback != None:
      if callback.__doc__ == None:
        return ""
      return callback.__doc__
    return ""

  def _methodSignature(self,name):
    "Returns a description of the signature of the method identified " \
    "by the argument 'name'. The returned value will consist of a list " \
    "containing another list. The first element of the inner list " \
    "describes the result. The subsequent elements of the inner list " \
    "will be the types of the method arguments."
    callback = self.lookupMethod(name)
    if callback != None:
      if self._methodSignatures[name] == None:
        return "undef"
      return self._methodSignatures[name]
    return "undef"

class Service1(IntrospectionService1):

  def __init__(self,name="service-1"):
    IntrospectionService1.__init__(self,name)
    self.exportMethod(self.multiply)
    self.exportMethod(self.echo)

  def multiply(self,x,y):
    "Multiplies two values together and returns the result. Note that " \
    "in Python, if one argument is a string, it returns a new string " \
    "which has been duplicated by the number of times described by the " \
    "other argument."
    return x*y

  def echo(self,*args):
    "Returns an array containing the arguments supplied to the function."
    return args


class Service2(IntrospectionService2):

  def __init__(self,name="service-2"):
    IntrospectionService2.__init__(self,name)
    self.exportMethod(self.multiply,"multiply",
        [['int','int','int'],['double','double','int'],
	 ['double','int','double'],['double','double','double'],
	 ['string','int','string'],['string','string','int']])
    self.exportMethod(self.echo,"echo",
        [['array','...']])

  def multiply(self,x,y):
    "Multiplies two values together and returns the result. Note that " \
    "in Python, if one argument is a string, it returns a new string " \
    "which has been duplicated by the number of times described by the " \
    "other argument."
    return x*y

  def echo(self,*args):
    "Returns an array containing the arguments supplied to the function. " \
    "This function can actually take a variable number of arguments, but " \
    "the introspection API doesn't define how you are mean't to handle " \
    "that so return '...'."
    return args


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

service = Service1()
service = Service2()

daemon = netsvc.HttpDaemon(8000)

rpcgw1 = netsvc.RpcGateway("introspect")
daemon.attach("/netrpc",rpcgw1)
rpcgw2 = netsvc.xmlrpc.RpcGateway("introspect")
daemon.attach("/xmlrpc",rpcgw2)
rpcgw3 = netsvc.soap.RpcGateway("introspect")
daemon.attach("/soap",rpcgw3)

daemon.start()

dispatcher.run()

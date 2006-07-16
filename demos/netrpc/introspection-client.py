from sys import argv,exit,stdin,exc_info
from string import strip

import netrpc

if len(argv) not in range(2,4):
  print "Usage: introspection-client url [protocol]"
  exit()

url = argv[1]
protocol = "NET-RPC"
if len(argv) >= 3:
  protocol = argv[2]

if protocol == "XML-RPC":
  import netrpc.xmlrpc
  service = netrpc.xmlrpc.RemoteService(url)
elif protocol == "SOAP":
  import netrpc.soap
  service = netrpc.soap.RemoteService(url)
else:
  service = netrpc.RemoteService(url)

try:
  result = service.system.listMethods()
except:
  print "Introspection Failed"
  print "--------------------"
  print ""
  print netrpc.exceptionDetails()
  exit(1)

info = {}
try:
  for name in result:
    info[name] = {}
    info[name]["signatures"] = service.system.methodSignature(name)
    info[name]["help"] = service.system.methodHelp(name)
except:
  print "Introspection Failed"
  print "--------------------"
  print ""
  print netrpc.exceptionDetails()
  exit(1)

if info.keys() == []:
  print "No Methods Described"
  print "--------------------"

for name in info.keys():
  print name
  print len(name) * "-"
  print
  signatures = info[name]["signatures"]
  if signatures == "undef" or signatures == []:
    print "Signature Unknown"
  else:
    print "Signatures:\n"
    for prototype in signatures:
      string = "  %s %s(" % (prototype[0],name)
      params = prototype[1:]
      if params != []:
	string = string + params[0]
	params = params[1:]
	if params != []:
	  for param in params:
	    string = string + ", " + param
      string = string + ")"
      print string
  print
  help = info[name]["help"]
  if help == None or help == "":
    print "No Documentation Available"
  else:
    print "Documentation:\n"
    print help
  print

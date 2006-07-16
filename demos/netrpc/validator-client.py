import sys
import random
import string
import time

import netrpc
import netrpc.xmlrpc

if len(sys.argv) not in range(2,4):
  print "Usage: validator-client url [protocol]"
  sys.exit()

url = sys.argv[1]
protocol = "NET-RPC"
if len(sys.argv) >= 3:
  protocol = sys.argv[2]

if protocol == "XML-RPC":
  service = netrpc.xmlrpc.RemoteService(url)
else:
  service = netrpc.RemoteService(url)

def test1(service):
  array = []
  for i in range(0,20):
    struct = {}
    struct["moe"] = int(1000*random.random())
    struct["larry"] = int(1000*random.random())
    struct["curly"] = int(1000*random.random())
    array.append(struct)
    total = 0
    for struct in array:
      total = total + struct["curly"]
  try:
    result = service.validator1.arrayOfStructsTest(array)
    if result != total:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test2(service):
  ascii = ""
  chars = "0123456789<>&'\""
  for i in range(0,100):
    ascii = string.join((ascii,chars[int(len(chars)*random.random())]),"")
  totals = { '<':0, '>':0, '&':0, "'": 0, '"':0 }
  for c in ascii:
    if totals.has_key(c):
      totals[c] = totals[c] + 1
  try:
    result = service.validator1.countTheEntities(ascii)
    if result["ctLeftAngleBrackets"] != totals['<']:
      return "FAILED"
    if result["ctRightAngleBrackets"] != totals['>']:
      return "FAILED"
    if result["ctAmpersands"] != totals['&']:
      return "FAILED"
    if result["ctApostrophes"] != totals["'"]:
      return "FAILED"
    if result["ctQuotes"] != totals['"']:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test3(service):
  struct = {}
  struct["moe"] = int(1000*random.random())
  struct["larry"] = int(1000*random.random())
  struct["curly"] = int(1000*random.random())
  total = struct["moe"] + struct["larry"] + struct["curly"]
  try:
    result = service.validator1.easyStructTest(struct)
    if result != total:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    print "FAILED"

def test4(service):
  struct = {}
  struct["moe"] = int(1000*random.random())
  struct["larry"] = int(1000*random.random())
  struct["curly"] = int(1000*random.random())
  try:
    result = service.validator1.echoStructTest(struct)
    if result != struct:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test5(service):
  array = []
  array.append(int(1000*random.random()))
  array.append(netrpc.Boolean(int(1000*random.random())%2))
  array.append(str(netrpc.DateTime()))
  array.append(float(int(1000*random.random()))/2)
  array.append(netrpc.DateTime())
  array.append(netrpc.Binary(str(netrpc.DateTime())))
  try:
    result = service.validator1.manyTypesTest(
        array[0],array[1],array[2],array[3],array[4],array[5])
    if result != array:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test6(service):
  array = []
  for i in range(1,100+int(100*random.random())):
    array.append(string.zfill(str(i),int(20*random.random())))
  try:
    result = service.validator1.moderateSizeArrayCheck(array)
    if result != array[0]+array[-1]:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test7(service):
  struct = {}
  struct["moe"] = int(1000*random.random())
  struct["larry"] = int(1000*random.random())
  struct["curly"] = int(1000*random.random())
  total = struct["moe"] + struct["larry"] + struct["curly"]
  struct = { "2000": { "04" : { "01" : struct } } }
  try:
    result = service.validator1.nestedStructTest(struct)
    if result != total:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test8(service):
  number = int(1000*random.random())
  struct = {}
  struct["times10"] = 10*number
  struct["times100"] = 100*number
  struct["times1000"] = 1000*number
  try:
    result = service.validator1.simpleStructReturnTest(number)
    if result != struct:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

print test1(service)
print test2(service)
print test3(service)
print test4(service)
print test5(service)
print test6(service)
print test7(service)
print test8(service)

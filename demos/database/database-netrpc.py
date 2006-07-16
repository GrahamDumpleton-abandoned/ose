import netrpc

url = "http://localhost:8000/database/test"

service = netrpc.RemoteService(url)

tables = service.execute("show tables")

timout = 30
for entry in tables:
  table = entry[0]
  print "table: " + table
  name = service.cursor(30)
  print "cursor: " + url + "/" + name
  cursor = netrpc.RemoteService(url+"/"+name)
  cursor.execute("select * from "+table)
  desc = cursor.description()
  print "desc: " + str(desc)
  data = cursor.fetchall()
  print "data: " + str(data)
  cursor.close()

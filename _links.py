import vampire
import os

# Following determines if this is being run within the
# context of the parent web site, and if it is use the
# links from the parent web site as well as any which
# have been added here.

current = os.path.dirname(__file__)
parent = os.path.join(current,"../..")

_links = []

try:
  module = vampire.importModule("_links",parent)
except OSError:
  pass
else:
  _links = list(module._links)

_package = []

if _links == []:
  _package.append(("Home","%(ose_home)s/index.html"))

#_package.append(("Articles","%(ose_home)s/articles.html"))
_package.append(("License","%(ose_home)s/license.html"))
#_package.append(("Examples","%(ose_home)s/examples/index.html"))
_package.append(("Installation","%(ose_home)s/install.html"))
#_package.append(("Porting","%(ose_home)s/porting.html"))
#_package.append(("Changes","%(ose_home)s/changes.html"))
#_package.append(("TODO","%(ose_home)s/todo.html"))

_links.append(("OSE",_package))

def links(req):
  return _links

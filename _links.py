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
#_package.append(("Changes","%(ose_home)s/changes.html"))
#_package.append(("TODO","%(ose_home)s/todo.html"))

_package.append(("News Archive","%(sf_news)s"))
_package.append(("Bug Reporting","%(sf_bugs)s"))
_package.append(("Support Requests","%(sf_support)s"))
_package.append(("Feature Requests","%(sf_features)s"))
_package.append(("Submit Patches","%(sf_patches)s"))
_package.append(("Web Forums","%(sf_forum)s"))
_package.append(("Mailing Lists","%(sf_mail)s"))
_package.append(("Downloads","%(sf_files)s"))

_links.append(("OSE",_package))

def links(req):
  return _links

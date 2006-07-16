#!/usr/bin/env python

from distutils.core import setup

setup(name="pyose",
      version="7.0pl12",
      description="Python Wrappers for OSE",
      author="Graham Dumpleton",
      author_email="grahamd@dscpl.com.au",
      url="http://ose.sourceforge.net",
      packages=['netrpc', 'netsvc', 'netsvc.xmlrpc', 'netsvc.soap'],
     )

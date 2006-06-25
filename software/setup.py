#!/usr/bin/env python

# If you are not interested in those parts of the Python modules which
# are dependent upon the C++ class library, specifically you don't
# require the "netsvc" module, but do want the "netrpc" and "zsirpc"
# modules, set the "WITH_NETSVC" variable to "False".

WITH_NETSVC = True

# If you do not want thread support compiled into the C++ class library
# which is used by the Python wrappers, set the "WITH_THREADS" variable
# to "False".

WITH_THREADS = True

# If thread support is being compiled into the C++ class library and you
# are using Microsoft C++, you must have access to an implementation of
# the POSIX thread library for the Win32 platform. Depending on where
# this library is installed on your system and which variant of the
# library you use, you will need to modify the following definitions as
# appropriate.

WIN32_PTHREAD_INCDIRS = ["c:/posix/include"]
WIN32_PTHREAD_LIBDIRS = ["c:/posix/lib"]
WIN32_PTHREAD_LDLIBS = ["pthreadVC"]



import sys, os, glob, string

from distutils import core
from distutils import sysconfig

file = open("PACKAGE","r")
value = file.readline()
file.close()

package = ""
version = ""

i = string.index(value,"-")

package = value[:i]
version = value[i+1:-1]

# This file does double duty. That is, the same file is used for the
# installation of the full Python wrappers for OSE, as well as the
# case where the "netsvc" module is not required or not present.

if not WITH_NETSVC or not os.access("netsvc",os.F_OK):

  # The first part file deals with installing just the "netrpc" module

  core.setup(
   name="netrpc",
   version=version,
   packages=[ "netrpc", "zsirpc" ],
   description="Python Modules For RPC Over HTTP Protocols.",
   author="Graham Dumpleton",
   author_email="grahamd@dscpl.com.au",
   url="http://ose.sourceforge.net"
  )

else:

  # This second part of the module deals with building the full wrappers.
  # Note that on UNIX like platforms the "configure" script needs to
  # have been run before trying to use "setup.py" and Python to build
  # the package. Note that if using native Win32 Microsoft C++ compiler,
  # it is assumed that "include/OSE/version.h" has been shipped with
  # the source code in generated form appropriate for version.

  sources = []

  sources.extend(glob.glob("python/*.cc"))

  sources.extend(glob.glob("library/OTC/collctn/otc*.cc"))
  sources.extend(glob.glob("library/OTC/debug/otc*.cc"))
  sources.extend(glob.glob("library/OTC/dispatch/otc*.cc"))
  sources.extend(glob.glob("library/OTC/internet/otc*.cc"))
  sources.extend(glob.glob("library/OTC/memory/otc*.cc"))
  sources.extend(glob.glob("library/OTC/message/otc*.cc"))
  sources.extend(glob.glob("library/OTC/misc/otc*.cc"))
  sources.extend(glob.glob("library/OTC/refcnt/otc*.cc"))
  sources.extend(glob.glob("library/OTC/system/otc*.cc"))
  sources.extend(glob.glob("library/OTC/text/otc*.cc"))
  sources.extend(glob.glob("library/OTC/thread/otc*.cc"))
  sources.extend(glob.glob("library/OTC/types/otc*.cc"))

  sources.extend(glob.glob("library/OTC/hash/hash_*.c"))
  sources.extend(glob.glob("library/OTC/regex/reg*.c"))

  if WITH_THREADS:
    define_macros=[("OSE_WITH_THREADS",1)]

  #define_macros.extend([("OTCLIB_TRACE",1)])

  include_dirs=["include"]

  library_dirs=[]
  libraries=[]

  # In older versions of Python it didn't use the C++
  # compiler to perform the final link so fiddle it
  # so that it does.

  if sys.version_info[0:2] < (2,2):
    compiler = sysconfig.get_config_var("CC")
    compiler_basename = os.path.basename(compiler)
    if compiler_basename == "gcc":
      sysconfig._config_vars["LDSHARED"] = string.replace( \
       sysconfig._config_vars["LDSHARED"],"gcc","g++")
      libraries.extend(["stdc++"])

  # Don't compile with debugging as final shared object
  # is simply way to big otherwise.

  sysconfig._config_vars["OPT"] = string.replace( \
   sysconfig.get_config_var("OPT")," -g "," ")

  # For native Win32 compiler, need to explicitly list the
  # POSIX threads library. Also link in Winsock 2 libraries
  # in case they are not otherwise being linked in.

  if sys.platform == "win32":
    if WITH_THREADS:
      include_dirs.extend(WIN32_PTHREAD_INCDIRS)
      library_dirs.extend(WIN32_PTHREAD_LIBDIRS)
      libraries.extend(WIN32_PTHREAD_LDLIBS)
    libraries.extend(["ws2_32"])

  module = core.Extension(
   name="_ose",
   sources=sources,
   define_macros=define_macros,
   include_dirs=include_dirs,
   library_dirs=library_dirs,
   libraries=libraries
  )

  packages=['netrpc', 'zsirpc', 'netsvc', 'netsvc.xmlrpc',
      'netsvc.json', 'netsvc.client', 'netsvc.cache' ]

  core.setup(
   name="netsvc",
   version=version,
   description="Python Wrappers For OSE C++ Class Library",
   author="Graham Dumpleton",
   author_email="grahamd@dscpl.com.au",
   url="http://ose.sourceforge.net",
   packages=packages,
   ext_modules=[module]
  )

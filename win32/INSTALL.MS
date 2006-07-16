To be able to compile and use just the C++ libraries from OSE on the Win32
platform with Microsoft C++, you should follow the steps below.

Note that you may want to make minor changes to the makefile so as to
make options used when compiling the library compatable with how you are
going to use the library.

Move the source code to the directory \ose\src. Change to the
directory \ose\src\ose-7.0\win32. This should now be the directory in
which this file is located. There should also be a file `MS.MAK' in this
directory.

Check the file `MS.MAK' to see if the options are as you desire. The
options that can be turned on are:

  DEBUG		- Will compile debug versions of library instead of
		  optimised.

  OSTORE	- Will compile in support for ObjectStore OODBMS. If
		  the location of the ObjectStore header files are not
		  listed in your INCLUDE environment variable so that the
		  compiler can find them, you will need to add a -I flag
		  in CPPFLAGS1 giving the location of the header files.

  THREADS	- Will make the library thread safe so that it can be
		  properly used in multithreaded applications. Note that
		  even if this isn't done, handling of signals in the
		  event system still relies on threads since signal
		  handlers are executed as separated threads. The library
		  therefore must always be built using -MD option.

  EXCEPTIONS	- Will compile in support for C++ exceptions.

  BOOL		- Will enable use of bool type instead of int in OSE.
		  Must have Microsoft C++ 5.0 or later.

  DLL		- Will create a DLL instead of static library. Note that
		  this will only work if you also have THREADS enabled.
		  Also, you can't use DLL in conjunction with OSTORE at
		  present.

  STDLIB	- Will use standard C++ library. Ie., <iostream> instead
		  of <iostream.h>. Your compiler must support it or you
		  must have a third party implementation.

  WINSOCK2	- Will compile library against Winsock 2 library instead
		  of Winsock 1.

To enable a particular option, make sure the variable is being set to `YES'.

Make sure that the Microsoft C++ compiler bin directory is in your path.
If the location of the Microsoft C++ header files are not listed in your
INCLUDE environment variable so that the compiler can find them, you will need
to add a -I flag in CPPFLAGS0 giving the location of the header files.

You can then run Microsoft NMAKE to build everything.

  nmake -f MS.MAK lib

If everything went okay, remove all the obj files from the directory.

To use the library, you will need to set/add the following in Microsoft
Developer Studio.

Note, on Visual Studio 98 and latter versions, the location of some menu
subitems has changed. In particular:

  Build -> Settings ====> Project -> Settings

Note that some options should only be defined if you had enabled certain
options.

  1: Tools -> Options -> Directories -> Include Files

       \ose\src\ose-7.0\config\X86_WIN32
       \ose\src\ose-7.0\include

     Equivalent if you are using a makefile and not the IDE, is:

       -I\ose\src\ose-7.0\config\X86_WIN32
       -I\ose\src\ose-7.0\include

  2: Build -> Settings -> C/C++ -> General -> Preprocessor Definitions

       EXPAND_TEMPLATES
       HAVE_WIN32_THREADS		# Only if THREADS was set to YES.
       HAVE_EXCEPTIONS			# Only if EXCEPTIONS was set to YES.
       ENV_OSTORE			# Only if OSTORE was set to YES.
       HAVE_BOOL			# Only if BOOL was set to YES.
       DLL_IMPORT_OSE			# Only if DLL was set to YES.
       ENV_CXXSTDLIB			# Only if STDLIB was set to YES.
       HAVE_WINSOCK2			# Only if WINSOCK2 was set to YES.

     Equivalent if you are using a makefile and not the IDE, is:

       -DEXPAND_TEMPLATES
       -DHAVE_WIN32_THREADS		# Only if THREADS was set to YES.
       -DHAVE_EXCEPTIONS		# Only if EXCEPTIONS was set to YES.
       -DENV_OSTORE			# Only if OSTORE was set to YES.
       -DHAVE_BOOL			# Only if BOOL was set to YES.
       -DDLL_IMPORT_OSE			# Only if DLL was set to YES.
       -DENV_CXXSTDLIB			# Only if STDLIB was set to YES.
       -DHAVE_WINSOCK2			# Only if WINSOCK2 was set to YES.

  3: Build -> Settings -> C/C++ -> Code Generation -> Use run-time library

       Multithreaded DLL		# Only if THREADS was set to YES.
       Debug Multithreaded DLL		# Only if THREADS & DEBUG set to YES.

     Equivalent if you are using a makefile and not the IDE, is:

       -MD				# Only if THREADS was set to YES.
       -MDd				# Only if THREADS & DEBUG set to YES.

  4: Insert -> Files into project

     For optimised library, use:

       \ose\src\ose-7.0\win32\OSEM70M.LIB

     For library with debug, use:

       \ose\src\ose-7.0\win32\OSEM70MD.LIB

  5: Build -> Settings -> Link -> Input -> Object/library modules

       WS2_32.LIB			# Only if WINSOCK2 was set to YES.
       WSOCK32.LIB			# Only if WINSOCK2 was set to NO.

     All libraries should appear after -link flag when not using IDE.

If you are using ObjectStore OODBMS, there exists a schema file for the
library. This is:

  \ose\src\ose-7.0\include\OTC\SCHEMA.cc

You should incorporate this file into your schema database.

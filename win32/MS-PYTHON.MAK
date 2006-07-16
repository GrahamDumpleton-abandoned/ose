# This makefile is only suitable for Microsoft NMAKE and Microsoft C++.

# You must use the same options here as in MS.MK. You must have built
# the main OSE library using MS.MAK first as this makefile picks up the
# same objects rather than recompile them.

DEBUG = NO

THREADS = YES
EXCEPTIONS = YES
BOOL = YES
DLL = YES
STDLIB = YES
WINSOCK2 = YES

CC = CL
AR = LIB

LIBNAME = _ose

PYTHON_VERSION = 15
PYTHON_ROOT = c:/Progra~1/Python

CPPFLAGS0 = \
 -DWIN32 \
 -DEXPAND_TEMPLATES \
 -I../config/X86_WIN32 \
 -I../include \
 -I$(PYTHON_ROOT)/include

CPPFLAGS8 = -DFD_SETSIZE=128

!IF "$(DEBUG)" == "YES"
CFLAGS0 = -Zi -Od
!ELSE
CFLAGS0 = -O
!ENDIF

CFLAGS1 = -GX

!IF "$(THREADS)" == "YES"

CPPFLAGS2 = -DHAVE_WIN32_THREADS

!ENDIF

!IF "$(DEBUG)" == "YES"
CFLAGS2 = -MDd
!ELSE
CFLAGS2 = -MD
!ENDIF


!IF "$(EXCEPTIONS)" == "YES"

CPPFLAGS3 = -DHAVE_EXCEPTIONS

!ENDIF

!IF "$(BOOL)" == "YES"

CPPFLAGS4 = -DHAVE_BOOL

!ENDIF

!IF "$(THREADS)" == "YES"
!IF "$(DLL)" == "YES"
CPPFLAGS5 = -DPIC -DDLL_EXPORT_OSE
!ENDIF
!ENDIF

!IF "$(STDLIB)" == "YES"

CPPFLAGS6 = -DENV_CXXSTDLIB

!ENDIF

!IF "$(WINSOCK2)" == "YES"

CPPFLAGS7 = -DHAVE_WINSOCK2

!ENDIF

OBJECTS = \
 otcpyagent.obj \
 otcpydsptch.obj \
 otcpyechosl.obj \
 otcpyexchnc.obj \
 otcpyexchng.obj \
 otcpyfailre.obj \
 otcpyfilesl.obj \
 otcpyfilesr.obj \
 otcpyhttpdm.obj \
 otcpyhttpsl.obj \
 otcpyhttpsn.obj \
 otcpyhttpsr.obj \
 otcpyjob.obj \
 otcpylogger.obj \
 otcpymnitor.obj \
 otcpymodule.obj \
 otcpyopaque.obj \
 otcpypbntfn.obj \
 otcpyprogrm.obj \
 otcpyreport.obj \
 otcpyrpcgwy.obj \
 otcpyrpcslt.obj \
 otcpyrquest.obj \
 otcpyrspnse.obj \
 otcpysbscrp.obj \
 otcpyservce.obj \
 otcpysrvbnd.obj \
 otcpyxmlrsl.obj \
 otcpyloader.obj

CPPFLAGS = $(CPPFLAGS0) $(CPPFLAGS1) $(CPPFLAGS2) \
 $(CPPFLAGS3) $(CPPFLAGS4) $(CPPFLAGS5) $(CPPFLAGS6) \
 $(CPPFLAGS7) $(CPPFLAGS8)

CFLAGS = $(CFLAGS0) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3)

!IF "$(CPPFLAGS5)" != ""
LIB : $(LIBNAME).dll
!ELSE
LIB : $(LIBNAME).lib
!ENDIF

$(LIBNAME).lib : $(OBJECTS)
	$(AR) -nologo -out:$@ *.obj

$(LIBNAME).dll : $(OBJECTS)
	$(CC) /LD -o $@ *.obj $(LDLIBS1) \
!IF "$(WINSOCK2)" == "YES"
         $(PYTHON_ROOT)/libs/python$(PYTHON_VERSION).lib \
         WS2_32.LIB USER32.LIB
!ELSE
         $(PYTHON_ROOT)/libs/python$(PYTHON_VERSION).lib \
         WSOCK32.LIB USER32.LIB
!ENDIF

otcpyagent.obj : ../lib/OTC-PY/python/otcpyagent.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyagent.cc

otcpydsptch.obj : ../lib/OTC-PY/python/otcpydsptch.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpydsptch.cc

otcpyechosl.obj : ../lib/OTC-PY/python/otcpyechosl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyechosl.cc

otcpyexchnc.obj : ../lib/OTC-PY/python/otcpyexchnc.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyexchnc.cc

otcpyexchng.obj : ../lib/OTC-PY/python/otcpyexchng.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyexchng.cc

otcpyfailre.obj : ../lib/OTC-PY/python/otcpyfailre.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyfailre.cc

otcpyfilesl.obj : ../lib/OTC-PY/python/otcpyfilesl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyfilesl.cc

otcpyfilesr.obj : ../lib/OTC-PY/python/otcpyfilesr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyfilesr.cc

otcpyhttpdm.obj : ../lib/OTC-PY/python/otcpyhttpdm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyhttpdm.cc

otcpyhttpsl.obj : ../lib/OTC-PY/python/otcpyhttpsl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyhttpsl.cc

otcpyhttpsn.obj : ../lib/OTC-PY/python/otcpyhttpsn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyhttpsn.cc

otcpyhttpsr.obj : ../lib/OTC-PY/python/otcpyhttpsr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyhttpsr.cc

otcpyjob.obj : ../lib/OTC-PY/python/otcpyjob.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyjob.cc

otcpylogger.obj : ../lib/OTC-PY/python/otcpylogger.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpylogger.cc

otcpymnitor.obj : ../lib/OTC-PY/python/otcpymnitor.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpymnitor.cc

otcpymodule.obj : ../lib/OTC-PY/python/otcpymodule.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpymodule.cc

otcpypbntfn.obj : ../lib/OTC-PY/python/otcpypbntfn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpypbntfn.cc

otcpyopaque.obj : ../lib/OTC-PY/python/otcpyopaque.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyopaque.cc

otcpyprogrm.obj : ../lib/OTC-PY/python/otcpyprogrm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyprogrm.cc

otcpyreport.obj : ../lib/OTC-PY/python/otcpyreport.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyreport.cc

otcpyrpcgwy.obj : ../lib/OTC-PY/python/otcpyrpcgwy.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyrpcgwy.cc

otcpyrpcslt.obj : ../lib/OTC-PY/python/otcpyrpcslt.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyrpcslt.cc

otcpyrquest.obj : ../lib/OTC-PY/python/otcpyrquest.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyrquest.cc

otcpyrspnse.obj : ../lib/OTC-PY/python/otcpyrspnse.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyrspnse.cc

otcpysbscrp.obj : ../lib/OTC-PY/python/otcpysbscrp.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpysbscrp.cc

otcpyservce.obj : ../lib/OTC-PY/python/otcpyservce.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyservce.cc

otcpysrvbnd.obj : ../lib/OTC-PY/python/otcpysrvbnd.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpysrvbnd.cc

otcpyxmlrsl.obj : ../lib/OTC-PY/python/otcpyxmlrsl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC-PY/python/otcpyxmlrsl.cc

otcpyloader.obj : ../python/otcpyloader.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../python/otcpyloader.cc


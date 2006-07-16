# This makefile is only suitable for Watcom WMAKE (/MS mode) and Watcom C++.

DEBUG = NO

THREADS = YES
EXCEPTIONS = YES

CC = CL
AR = LIB

!IF "$(DEBUG)" == "YES"
!IF "$(THREADS)" == "YES"
LIBNAME = OSEW70MD
!ELSE
LIBNAME = OSEW70SD
!ENDIF
!ELSE
!IF "$(THREADS)" == "YES"
LIBNAME = OSEW70M
!ELSE
LIBNAME = OSEW70S
!ENDIF
!ENDIF

CPPFLAGS0 = \
 -DWIN32 \
 -DEXPAND_TEMPLATES \
 -I../config/X86_WIN32 \
 -I../include

!IF "$(DEBUG)" == "YES"
CFLAGS0 = -d2
!ELSE
CFLAGS0 = -otexan
!ENDIF

CFLAGS1 = -GX

!IF "$(THREADS)" == "YES"

CPPFLAGS2 = -DHAVE_WIN32_THREADS

!IF "$(DEBUG)" == "YES"
CFLAGS2 = -MDd
!ELSE
CFLAGS2 = -MD
!ENDIF

!ELSE

!IF "$(DEBUG)" == "YES"
CFLAGS2 = -MLd
!ELSE
CFLAGS2 = -ML
!ENDIF

!ENDIF

!IF "$(EXCEPTIONS)" == "YES"

CPPFLAGS3 = -DHAVE_EXCEPTIONS
CFLAGS3 = -xs

!ENDIF

OBJECTS2 = \
 oseversion.obj \
 otcanchor.obj \
 otcapplcatr.obj \
 otcavlhnode.obj \
 otcavllnode.obj \
 otcavlnode.obj \
 otcavltnode.obj \
 otcavltree.obj \
 otcbitset.obj \
 otccbitset.obj \
 otcdequei.obj \
 otchashntry.obj \
 otchashtble.obj \
 otciilink.obj \
 otcilink.obj \
 otcivlink.obj \
 otclink.obj \
 otclinkable.obj \
 otclinkcurs.obj \
 otclinkiter.obj \
 otclinklist.obj \
 otclisti.obj \
 otcrange.obj \
 otcrlink.obj \
 otcrngecurs.obj \
 otcvlink.obj \
 otcasrtfail.obj \
 otcassert.obj \
 otcensure.obj \
 otcerrors.obj \
 otcexcption.obj \
 otclogbuf.obj \
 otclogger.obj \
 otclogstrm.obj \
 otcnewhndlr.obj \
 otcoutofmem.obj \
 otcprecfail.obj \
 otcstophere.obj \
 otctermfunc.obj \
 otctobject.obj \
 otctracer.obj \
 otctracetag.obj \
 otctrcebuf.obj \
 otctrcestrm.obj \
 otctrcswtch.obj \
 otcwarning.obj \
 otcaction.obj \
 otcalarm.obj \
 otccondtion.obj \
 otcdispatch.obj \
 otcenvelope.obj \
 otcevagent.obj \
 otcevcllbck.obj \
 otcevent.obj \
 otceventjob.obj \
 otcevhandle.obj \
 otcevthread.obj \
 otcfailure.obj \
 otcfnagent.obj \
 otcgrpannc.obj \
 otcioevent.obj \
 otcjob.obj \
 otcjoblist.obj \
 otcjobqueue.obj \
 otcmessage.obj \
 otcopaque.obj \
 otcpbntftcn.obj \
 otcplreader.obj \
 otcreceiver.obj \
 otcreport.obj \
 otcrequest.obj \
 otcresponse.obj \
 otcropyload.obj \
 otcrwpyload.obj \
 otcsbscrptn.obj \
 otcservannc.obj \
 otcservbind.obj \
 otcshutdown.obj \
 otcsignal.obj \
 otcstartup.obj \
 otcsubject.obj \
 otcsvbroker.obj \
 otcsvpyload.obj \
 otcsvregsub.obj \
 otcsvrgstry.obj \
 otctimeout.obj \
 otcechosrvl.obj \
 otcfilesrvl.obj \
 otcfilesrvr.obj \
 otchttpdaem.obj \
 otchttplstn.obj \
 otchttpsrvl.obj \
 otchttpsrvr.obj \
 otchttpsssn.obj \
 otcrpcgtway.obj \
 otcrpcsrvlt.obj \
 otcxmlrpcgw.obj \
 otcxmlrpcsl.obj \
 otcalign.obj \
 otcarena.obj \
 otccluster.obj \
 otccmmnpool.obj \
 otcheap.obj \
 otcmcobject.obj \
 otcmpobject.obj \
 otcpool.obj \
 otccongest.obj \
 otcconnect.obj \
 otcendpoint.obj \
 otcepclient.obj \
 otcepintrfc.obj \
 otcepisockt.obj \
 otcepismesg.obj \
 otcepisstrm.obj \
 otceplisten.obj \
 otceprgstry.obj \
 otcepserver.obj \
 otcexchannc.obj \
 otcexchange.obj \
 otcexrgstry.obj \
 otcinetaddr.obj \
 otcinetclnt.obj \
 otcinetlstn.obj \
 otcinetsrvr.obj \
 otccopy.obj \
 otcfuncbuf.obj \
 otcfuncstrm.obj \
 otchash.obj \
 otcrank.obj \
 otctermnate.obj \
 otcvfuncbuf.obj \
 otccount.obj \
 otcrcount.obj \
 otcresource.obj \
 otcdirectry.obj \
 otcpathname.obj \
 otcprogram.obj \
 otcsigblock.obj \
 otcsighndle.obj \
 otcstat.obj \
 otcstopwtch.obj \
 otcclckinfo.obj \
 otccstring.obj \
 otcdomnode.obj \
 otcdomprpty.obj \
 otcdomstrng.obj \
 otcglobex.obj \
 otclstring.obj \
 otcoptions.obj \
 otcosstream.obj \
 otcpattern.obj \
 otcprprties.obj \
 otcrecord.obj \
 otcregex.obj \
 otcregexp.obj \
 otcrstring.obj \
 otcsilink.obj \
 otcslink.obj \
 otcslogger.obj \
 otcstring.obj \
 otcstrngbuf.obj \
 otcsvlink.obj \
 otcsymbol.obj \
 otctracepat.obj \
 otcvrecord.obj \
 otcmutex.obj \
 otccvmutex.obj \
 otcnrmutex.obj \
 otcrwmutex.obj \
 otcdate.obj \
 otcduration.obj \
 otctime.obj

OBJECTS = $(OBJECTS1) $(OBJECTS2)

CPPFLAGS = $(CPPFLAGS0) $(CPPFLAGS1) $(CPPFLAGS2) $(CPPFLAGS3)

CFLAGS = $(CFLAGS0) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3)

LIB : $(LIBNAME).lib

$(LIBNAME).lib : $(OBJECTS)
	$(AR) -nologo -out:$@ $(OBJECTS)

oseversion.obj : ../lib/OSE/oseversion.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OSE/oseversion.cc

otcanchor.obj : ../lib/OTC/collctn/otcanchor.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcanchor.cc

otcapplcatr.obj : ../lib/OTC/collctn/otcapplcatr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcapplcatr.cc

otcavlhnode.obj : ../lib/OTC/collctn/otcavlhnode.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcavlhnode.cc

otcavllnode.obj : ../lib/OTC/collctn/otcavllnode.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcavllnode.cc

otcavlnode.obj : ../lib/OTC/collctn/otcavlnode.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcavlnode.cc

otcavltnode.obj : ../lib/OTC/collctn/otcavltnode.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcavltnode.cc

otcavltree.obj : ../lib/OTC/collctn/otcavltree.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcavltree.cc

otcbitset.obj : ../lib/OTC/collctn/otcbitset.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcbitset.cc

otccbitset.obj : ../lib/OTC/collctn/otccbitset.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otccbitset.cc

otcdequei.obj : ../lib/OTC/collctn/otcdequei.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcdequei.cc

otchashntry.obj : ../lib/OTC/collctn/otchashntry.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otchashntry.cc

otchashtble.obj : ../lib/OTC/collctn/otchashtble.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otchashtble.cc

otciilink.obj : ../lib/OTC/collctn/otciilink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otciilink.cc

otcilink.obj : ../lib/OTC/collctn/otcilink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcilink.cc

otcivlink.obj : ../lib/OTC/collctn/otcivlink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcivlink.cc

otclink.obj : ../lib/OTC/collctn/otclink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otclink.cc

otclinkable.obj : ../lib/OTC/collctn/otclinkable.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otclinkable.cc

otclinkcurs.obj : ../lib/OTC/collctn/otclinkcurs.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otclinkcurs.cc

otclinkiter.obj : ../lib/OTC/collctn/otclinkiter.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otclinkiter.cc

otclinklist.obj : ../lib/OTC/collctn/otclinklist.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otclinklist.cc

otclisti.obj : ../lib/OTC/collctn/otclisti.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otclisti.cc

otcrange.obj : ../lib/OTC/collctn/otcrange.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcrange.cc

otcrlink.obj : ../lib/OTC/collctn/otcrlink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcrlink.cc

otcrngecurs.obj : ../lib/OTC/collctn/otcrngecurs.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcrngecurs.cc

otcvlink.obj : ../lib/OTC/collctn/otcvlink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/collctn/otcvlink.cc

otcasrtfail.obj : ../lib/OTC/debug/otcasrtfail.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcasrtfail.cc

otcassert.obj : ../lib/OTC/debug/otcassert.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcassert.cc

otcensure.obj : ../lib/OTC/debug/otcensure.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcensure.cc

otcerrors.obj : ../lib/OTC/debug/otcerrors.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcerrors.cc

otcexcption.obj : ../lib/OTC/debug/otcexcption.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcexcption.cc

otclogbuf.obj : ../lib/OTC/debug/otclogbuf.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otclogbuf.cc

otclogger.obj : ../lib/OTC/debug/otclogger.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otclogger.cc

otclogstrm.obj : ../lib/OTC/debug/otclogstrm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otclogstrm.cc

otcnewhndlr.obj : ../lib/OTC/debug/otcnewhndlr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcnewhndlr.cc

otcoutofmem.obj : ../lib/OTC/debug/otcoutofmem.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcoutofmem.cc

otcprecfail.obj : ../lib/OTC/debug/otcprecfail.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcprecfail.cc

otcstophere.obj : ../lib/OTC/debug/otcstophere.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcstophere.cc

otctermfunc.obj : ../lib/OTC/debug/otctermfunc.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctermfunc.cc

otctobject.obj : ../lib/OTC/debug/otctobject.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctobject.cc

otctracer.obj : ../lib/OTC/debug/otctracer.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctracer.cc

otctracetag.obj : ../lib/OTC/debug/otctracetag.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctracetag.cc

otctrcebuf.obj : ../lib/OTC/debug/otctrcebuf.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctrcebuf.cc

otctrcestrm.obj : ../lib/OTC/debug/otctrcestrm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctrcestrm.cc

otctrcswtch.obj : ../lib/OTC/debug/otctrcswtch.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otctrcswtch.cc

otcwarning.obj : ../lib/OTC/debug/otcwarning.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/debug/otcwarning.cc

otcaction.obj : ../lib/OTC/dispatch/otcaction.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcaction.cc

otcalarm.obj : ../lib/OTC/dispatch/otcalarm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcalarm.cc

otccondtion.obj : ../lib/OTC/dispatch/otccondtion.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otccondtion.cc

otcdispatch.obj : ../lib/OTC/dispatch/otcdispatch.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcdispatch.cc

otcenvelope.obj : ../lib/OTC/dispatch/otcenvelope.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcenvelope.cc

otcevagent.obj : ../lib/OTC/dispatch/otcevagent.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcevagent.cc

otcevcllbck.obj : ../lib/OTC/dispatch/otcevcllbck.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcevcllbck.cc

otcevent.obj : ../lib/OTC/dispatch/otcevent.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcevent.cc

otceventjob.obj : ../lib/OTC/dispatch/otceventjob.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otceventjob.cc

otcevhandle.obj : ../lib/OTC/dispatch/otcevhandle.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcevhandle.cc

otcevthread.obj : ../lib/OTC/dispatch/otcevthread.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcevthread.cc

otcfailure.obj : ../lib/OTC/dispatch/otcfailure.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcfailure.cc

otcfnagent.obj : ../lib/OTC/dispatch/otcfnagent.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcfnagent.cc

otcgrpannc.obj : ../lib/OTC/dispatch/otcgrpannc.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcgrpannc.cc

otcioevent.obj : ../lib/OTC/dispatch/otcioevent.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcioevent.cc

otcjob.obj : ../lib/OTC/dispatch/otcjob.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcjob.cc

otcjoblist.obj : ../lib/OTC/dispatch/otcjoblist.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcjoblist.cc

otcjobqueue.obj : ../lib/OTC/dispatch/otcjobqueue.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcjobqueue.cc

otcmessage.obj : ../lib/OTC/dispatch/otcmessage.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcmessage.cc

otcopaque.obj : ../lib/OTC/dispatch/otcopaque.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcopaque.cc

otcpbntftcn.obj : ../lib/OTC/dispatch/otcpbntftcn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcpbntftcn.cc

otcplreader.obj : ../lib/OTC/dispatch/otcplreader.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcplreader.cc

otcreceiver.obj : ../lib/OTC/dispatch/otcreceiver.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcreceiver.cc

otcreport.obj : ../lib/OTC/dispatch/otcreport.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcreport.cc

otcrequest.obj : ../lib/OTC/dispatch/otcrequest.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcrequest.cc

otcresponse.obj : ../lib/OTC/dispatch/otcresponse.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcresponse.cc

otcropyload.obj : ../lib/OTC/dispatch/otcropyload.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcropyload.cc

otcrwpyload.obj : ../lib/OTC/dispatch/otcrwpyload.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcrwpyload.cc

otcsbscrptn.obj : ../lib/OTC/dispatch/otcsbscrptn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsbscrptn.cc

otcservannc.obj : ../lib/OTC/dispatch/otcservannc.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcservannc.cc

otcservbind.obj : ../lib/OTC/dispatch/otcservbind.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcservbind.cc

otcshutdown.obj : ../lib/OTC/dispatch/otcshutdown.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcshutdown.cc

otcsignal.obj : ../lib/OTC/dispatch/otcsignal.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsignal.cc

otcstartup.obj : ../lib/OTC/dispatch/otcstartup.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcstartup.cc

otcsubject.obj : ../lib/OTC/dispatch/otcsubject.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsubject.cc

otcsvbroker.obj : ../lib/OTC/dispatch/otcsvbroker.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsvbroker.cc

otcsvpyload.obj : ../lib/OTC/dispatch/otcsvpyload.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsvpyload.cc

otcsvregsub.obj : ../lib/OTC/dispatch/otcsvregsub.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsvregsub.cc

otcsvrgstry.obj : ../lib/OTC/dispatch/otcsvrgstry.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otcsvrgstry.cc

otctimeout.obj : ../lib/OTC/dispatch/otctimeout.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/dispatch/otctimeout.cc

otcechosrvl.obj : ../lib/OTC/internet/otcechosrvl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcechosrvl.cc

otcfilesrvl.obj : ../lib/OTC/internet/otcfilesrvl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcfilesrvl.cc

otcfilesrvr.obj : ../lib/OTC/internet/otcfilesrvr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcfilesrvr.cc

otchttpdaem.obj : ../lib/OTC/internet/otchttpdaem.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otchttpdaem.cc

otchttplstn.obj : ../lib/OTC/internet/otchttplstn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otchttplstn.cc

otchttpsrvl.obj : ../lib/OTC/internet/otchttpsrvl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otchttpsrvl.cc

otchttpsrvr.obj : ../lib/OTC/internet/otchttpsrvr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otchttpsrvr.cc

otchttpsssn.obj : ../lib/OTC/internet/otchttpsssn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otchttpsssn.cc

otcrpcgtway.obj : ../lib/OTC/internet/otcrpcgtway.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcrpcgtway.cc

otcrpcsrvlt.obj : ../lib/OTC/internet/otcrpcsrvlt.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcrpcsrvlt.cc

otcxmlrpcgw.obj : ../lib/OTC/internet/otcxmlrpcgw.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcxmlrpcgw.cc

otcxmlrpcsl.obj : ../lib/OTC/internet/otcxmlrpcsl.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/internet/otcxmlrpcsl.cc

otcalign.obj : ../lib/OTC/memory/otcalign.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otcalign.cc

otcarena.obj : ../lib/OTC/memory/otcarena.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otcarena.cc

otccluster.obj : ../lib/OTC/memory/otccluster.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otccluster.cc

otccmmnpool.obj : ../lib/OTC/memory/otccmmnpool.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otccmmnpool.cc

otcheap.obj : ../lib/OTC/memory/otcheap.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otcheap.cc

otcmcobject.obj : ../lib/OTC/memory/otcmcobject.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otcmcobject.cc

otcmpobject.obj : ../lib/OTC/memory/otcmpobject.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otcmpobject.cc

otcpool.obj : ../lib/OTC/memory/otcpool.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/memory/otcpool.cc

otccongest.obj : ../lib/OTC/message/otccongest.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otccongest.cc

otcconnect.obj : ../lib/OTC/message/otcconnect.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcconnect.cc

otcendpoint.obj : ../lib/OTC/message/otcendpoint.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcendpoint.cc

otcepclient.obj : ../lib/OTC/message/otcepclient.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcepclient.cc

otcepintrfc.obj : ../lib/OTC/message/otcepintrfc.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcepintrfc.cc

otcepisockt.obj : ../lib/OTC/message/otcepisockt.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcepisockt.cc

otcepismesg.obj : ../lib/OTC/message/otcepismesg.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcepismesg.cc

otcepisstrm.obj : ../lib/OTC/message/otcepisstrm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcepisstrm.cc

otceplisten.obj : ../lib/OTC/message/otceplisten.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otceplisten.cc

otceprgstry.obj : ../lib/OTC/message/otceprgstry.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otceprgstry.cc

otcepserver.obj : ../lib/OTC/message/otcepserver.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcepserver.cc

otcexchannc.obj : ../lib/OTC/message/otcexchannc.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcexchannc.cc

otcexchange.obj : ../lib/OTC/message/otcexchange.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcexchange.cc

otcexrgstry.obj : ../lib/OTC/message/otcexrgstry.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcexrgstry.cc

otcinetaddr.obj : ../lib/OTC/message/otcinetaddr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcinetaddr.cc

otcinetclnt.obj : ../lib/OTC/message/otcinetclnt.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcinetclnt.cc

otcinetlstn.obj : ../lib/OTC/message/otcinetlstn.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcinetlstn.cc

otcinetsrvr.obj : ../lib/OTC/message/otcinetsrvr.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/message/otcinetsrvr.cc

otccopy.obj : ../lib/OTC/misc/otccopy.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otccopy.cc

otcfuncbuf.obj : ../lib/OTC/misc/otcfuncbuf.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otcfuncbuf.cc

otcfuncstrm.obj : ../lib/OTC/misc/otcfuncstrm.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otcfuncstrm.cc

otchash.obj : ../lib/OTC/misc/otchash.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otchash.cc

otcrank.obj : ../lib/OTC/misc/otcrank.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otcrank.cc

otctermnate.obj : ../lib/OTC/misc/otctermnate.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otctermnate.cc

otcvfuncbuf.obj : ../lib/OTC/misc/otcvfuncbuf.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/misc/otcvfuncbuf.cc

otccount.obj : ../lib/OTC/refcnt/otccount.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/refcnt/otccount.cc

otcrcount.obj : ../lib/OTC/refcnt/otcrcount.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/refcnt/otcrcount.cc

otcresource.obj : ../lib/OTC/refcnt/otcresource.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/refcnt/otcresource.cc

otcdirectry.obj : ../lib/OTC/system/otcdirectry.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcdirectry.cc

otcpathname.obj : ../lib/OTC/system/otcpathname.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcpathname.cc

otcprogram.obj : ../lib/OTC/system/otcprogram.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcprogram.cc

otcsigblock.obj : ../lib/OTC/system/otcsigblock.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcsigblock.cc

otcsighndle.obj : ../lib/OTC/system/otcsighndle.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcsighndle.cc

otcstat.obj : ../lib/OTC/system/otcstat.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcstat.cc

otcstopwtch.obj : ../lib/OTC/system/otcstopwtch.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcstopwtch.cc

otcclckinfo.obj : ../lib/OTC/system/otcclckinfo.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/system/otcclckinfo.cc

otccstring.obj : ../lib/OTC/text/otccstring.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otccstring.cc

otcdomnode.obj : ../lib/OTC/text/otcdomnode.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcdomnode.cc

otcdomprpty.obj : ../lib/OTC/text/otcdomprpty.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcdomprpty.cc

otcdomstrng.obj : ../lib/OTC/text/otcdomstrng.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcdomstrng.cc

otcglobex.obj : ../lib/OTC/text/otcglobex.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcglobex.cc

otclstring.obj : ../lib/OTC/text/otclstring.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otclstring.cc

otcoptions.obj : ../lib/OTC/text/otcoptions.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcoptions.cc

otcosstream.obj : ../lib/OTC/text/otcosstream.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcosstream.cc

otcpattern.obj : ../lib/OTC/text/otcpattern.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcpattern.cc

otcprprties.obj : ../lib/OTC/text/otcprprties.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcprprties.cc

otcrecord.obj : ../lib/OTC/text/otcrecord.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcrecord.cc

otcregex.obj : ../lib/OTC/text/otcregex.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcregex.cc

otcregexp.obj : ../lib/OTC/text/otcregexp.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcregexp.cc

otcrstring.obj : ../lib/OTC/text/otcrstring.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcrstring.cc

otcsilink.obj : ../lib/OTC/text/otcsilink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcsilink.cc

otcslink.obj : ../lib/OTC/text/otcslink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcslink.cc

otcslogger.obj : ../lib/OTC/text/otcslogger.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcslogger.cc

otcstring.obj : ../lib/OTC/text/otcstring.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcstring.cc

otcstrngbuf.obj : ../lib/OTC/text/otcstrngbuf.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcstrngbuf.cc

otcsvlink.obj : ../lib/OTC/text/otcsvlink.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcsvlink.cc

otcsymbol.obj : ../lib/OTC/text/otcsymbol.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcsymbol.cc

otctracepat.obj : ../lib/OTC/text/otctracepat.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otctracepat.cc

otcvrecord.obj : ../lib/OTC/text/otcvrecord.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/text/otcvrecord.cc

otcmutex.obj : ../lib/OTC/thread/otcmutex.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/thread/otcmutex.cc

otccvmutex.obj : ../lib/OTC/thread/otccvmutex.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/thread/otccvmutex.cc

otcnrmutex.obj : ../lib/OTC/thread/otcnrmutex.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/thread/otcnrmutex.cc

otcrwmutex.obj : ../lib/OTC/thread/otcrwmutex.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/thread/otcrwmutex.cc

otcdate.obj : ../lib/OTC/types/otcdate.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/types/otcdate.cc

otcduration.obj : ../lib/OTC/types/otcduration.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/types/otcduration.cc

otctime.obj : ../lib/OTC/types/otctime.cc
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -Tp ../lib/OTC/types/otctime.cc

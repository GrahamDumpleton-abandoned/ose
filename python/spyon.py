#! /usr/bin/env python
#
# Copyright 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import sys
import signal
import string
import Tkinter
import os
import time
import fnmatch
import socket
import netsvc

try:
  import Pmw
except:
  print "This program requires the Python Megawidgets (Pmw) module."
  print "Pmw can be obtained free from 'http://pmw.sourceforge.net'."
  sys.exit()

from netsvc.spyon.StartupDialog import StartupDialog
from netsvc.spyon.AgentBrowser import AgentBrowser
from netsvc.spyon.AgentWindow import AgentWindow
from netsvc.spyon.ServiceBrowser import ServiceBrowser
from netsvc.spyon.ServiceWindow import ServiceWindow
from netsvc.spyon.GroupBrowser import GroupBrowser

try:
  import threading
except:
  print "Your version of Python does not include support for threading."
  sys.exit()

if not netsvc.threadingEnabled():
  print "Your version of OSE does not include support for threading."
  sys.exit()

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()


class ReportWindowAgent(netsvc.Service):

  def __init__(self,name,service,subject):
    netsvc.Service.__init__(self,name)

    self._service = service
    self._subject = subject

    self._services = {}
    self._groups = {}
    self._messages = []
    self._reports = []

    self._version = 0

    self.subscribeReports(service,subject)

  def modelVersion(self):
    return self._version

  def availableServices(self):
    return self._services

  def availableGroups(self):
    return self._groups

  def queuedMessages(self):
    result = self._messages
    self._messages = []
    return result

  def queuedReports(self):
    result = self._reports
    self._reports = []
    return result

  def handlePublisherNotification(self,notification):
    name = notification.publisher().serviceName()
    agent = notification.publisher().agentIdentity()

    self._messages.append("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
    self._messages.append("EVENT     = 'ANNOUNCEMENT'\n")
    self._messages.append("SERVICE   = %s\n" % `name`)
    self._messages.append("AGENT     = %s\n" % `agent`)

    if notification.status() == netsvc.SERVICE_AVAILABLE:
      self._messages.append("STATUS    = 'AVAILABLE'\n")
    else:
      self._messages.append("STATUS    = 'WITHDRAWN'\n")

    self._messages.append("\n")

    self._version = self._version + 1

  def handleReport(self,report):
    self._reports.append(report)

    name = report.publisher().serviceName()
    agent = report.publisher().agentIdentity()
    subject = report.subjectName()
    content = str(repr(report.content()))

    self._messages.append("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
    self._messages.append("EVENT     = 'REPORT'\n")
    self._messages.append("SERVICE   = %s\n" % `name`)
    self._messages.append("AGENT     = %s\n" % `agent`)
    self._messages.append("SUBJECT   = %s\n" % `subject`)
    self._messages.append("CONTENT   = %s\n" % content)
    self._messages.append("\n")

    self._version = self._version + 1


class ReportView:

  def __init__(self,service,subject):
    name = "spy@%s" % netsvc.processIdentity()

    self._subjects = {}

    self._logState = "running"
    self._eventLog = ""

    self._version = 0

    self._root = Pmw.MegaToplevel()
    self._root.withdraw()

    self._root.title("SPYON:SUBJECT - %s / %s" % (service,subject))
    self._root.protocol("WM_DELETE_WINDOW",self._destroyWindow)

    g3a = Pmw.Group(self._root.interior(),tag_text="Service")
    g3a.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    #p3 = Pmw.PanedWidget(self._root.interior(),orient="vertical",
    #	hull_width=500,hull_height=600)
    p3 = Pmw.PanedWidget(g3a.interior(),orient="vertical",
	hull_width=500,hull_height=600)
    p3.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    p3.add("top")
    p3.add("bottom")

    #g3 = Pmw.Group(p3.pane("top"),tag_text="Service")
    #g3.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    #f3 = Tkinter.Frame(g3.interior())
    f3 = Tkinter.Frame(p3.pane("top"))
    f3.pack(side="top")

    e1 = Pmw.EntryField(f3,labelpos='w',entry_width=25,
        label_text="Name:",validate=None,value=service,
	entry_state="disabled")
    e1.pack(side="top",padx=8,pady=8)

    e2 = Pmw.EntryField(f3,labelpos='w',entry_width=25,
        label_text="Subject:",validate=None,value=subject,
	entry_state="disabled")
    e2.pack(side="top",padx=8,pady=8)

    Pmw.alignlabels((e1,e2))

    self._subjectView = Pmw.ScrolledListBox(g3.interior(),listbox_height=12)
    self._subjectView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    #g4 = Pmw.Group(p3.pane("bottom"),tag_text="Events")
    #g4.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    #f4 = Tkinter.Frame(g4.interior())
    f4 = Tkinter.Frame(p3.pane("bottom"))
    f4.pack(side="top")

    self._eventButtons = Pmw.ButtonBox(f4,orient="horizontal")
    self._eventButtons.pack(side="left",padx=8,pady=8)
    self._eventButtons.add("Stop",width=7,command=self._startLog)
    self._eventButtons.add("Suspend",width=7,command=self._suspendLog)
    self._eventButtons.add("Clear",width=7,command=self._clearLog)

    self._eventView = Pmw.ScrolledText(g4.interior(),text_padx=4,
        text_pady=4,text_wrap="char",text_state="disabled",
	text_height=10,borderframe=1)
    #	text_height=10,vscrollmode="static",borderframe=1)
    self._eventView.pack(side="top",padx=8,pady=8,fill="both",expand="true")
    self._eventView.yview("moveto",1.0)

    self._root.update()
    self._root.deiconify()
    self._root.update_idletasks()

    self._agent = ReportWindowAgent(name,service,subject).synchronisedObject()

    self._root.after(100,self._wakeup)

  def _wakeup(self):
    version = self._agent.modelVersion()
    if version != self._version:
      self._version = version
      self._updateReports()
      self._updateMessages()
    self._root.after(100,self._wakeup)

  def _destroyWindow(self):
    self._agent.destroyReferences()
    self._agent = None
    self._root.destroy()

  def _clearLog(self):
    self._eventView.configure(text_state="normal")
    self._eventView.clear()
    self._eventView.configure(text_state="disabled")
    self._eventView.yview("moveto",1.0)

  def _clearLog(self):
    self._eventView.configure(text_state="normal")
    self._eventView.clear()
    self._eventView.configure(text_state="disabled")
    self._eventView.yview("moveto",1.0)

  def _appendLog(self,message):
    if self._logState == "running":
      p1,p2 = self._eventView.yview()
      self._eventView.configure(text_state="normal")
      self._eventView.insert("end",message)
      self._eventView.configure(text_state="disabled")
      if (p1 == 0.0 and p2 == 0.0) or p2 == 1.0:
	self._eventView.yview("moveto",1.0)
    elif self._logState == "paused":
      self._eventLog = self._eventLog + message

  def _startLog(self):
    if self._logState == "stopped":
      self._logState = "running"
      self._eventButtons.button(0).configure(text="Stop")
      self._eventButtons.button(1).configure(state="normal")
    elif self._logState == "running":
      self._logState = "stopped"
      self._eventButtons.button(0).configure(text="Start")
      self._eventButtons.button(1).configure(state="disabled")
    elif self._logState == "paused":
      self._logState = "stopped"
      self._eventButtons.button(0).configure(text="Start")
      self._eventButtons.button(1).configure(text="Suspend")
      self._eventButtons.button(1).configure(state="disabled")

  def _suspendLog(self):
    if self._logState == "running":
      self._logState = "paused"
      self._eventButtons.button(1).configure(text="Resume")
    elif self._logState == "paused":
      self._logState = "running"
      self._eventButtons.button(1).configure(text="Suspend")
      if self._eventLog != "":
	p1,p2 = self._eventView.yview()
	self._eventView.configure(text_state="normal")
	self._eventView.insert("end",self._eventLog)
	self._eventView.configure(text_state="disabled")
	if (p1 == 0.0 and p2 == 0.0) or p2 == 1.0:
	  self._eventView.yview("moveto",1.0)
	self._eventLog = ""

  def _updateReports(self):
    for report in self._agent.queuedReports():
      subject = report.subjectName()
      content = str(report.content())
      self._subjects[subject] = content

    subjects = self._subjects.keys()
    subjects.sort()

    view = []
    for subject in subjects:
      view.append(subject + " = " + self._subjects[subject])
    self._subjectView.setlist(view)

  def _updateMessages(self):
    for message in self._agent.queuedMessages():
      self._appendLog(message)


class MainWindowAgent(netsvc.Service):

  def __init__(self,name):
    netsvc.Service.__init__(self,name)

    self._services = {}
    self._groups = {}
    self._messages = []
    self._reports = []

    self._version = 0

    self.joinGroup("spies")
    self.subscribeRegistry(self._handleAnnouncements)

  def modelVersion(self):
    return self._version

  def availableServices(self):
    return self._services

  def availableGroups(self):
    return self._groups

  def queuedMessages(self):
    result = self._messages
    self._messages = []
    return result

  def queuedReports(self):
    result = self._reports
    self._reports = []
    return result

  def _handleAnnouncements(self,binding,group,status):
    update = 0
    service = binding.serviceName()

    if group == None:
      if status == netsvc.SERVICE_AVAILABLE:
        if self._services.has_key(service):
	  self._services[service] = self._services[service] + 1
	else:
	  self._services[service] = 1
	  update = 1
      else:
        if self._services.has_key(service):
	  count = self._services[service] - 1
	  if count == 0:
	    del self._services[service]
	    update = 1
	  else:
	    self._services[service] = count
    else:
      if status == netsvc.SERVICE_AVAILABLE:
        if self._groups.has_key(group):
	  services = self._groups[group]
	  if services.has_key(service):
	    self._groups[group][service] = services[service] + 1
	  else:
	    self._groups[group][service] = 1
	    update = 1
	else:
	  self._groups[group] = { service: 1 }
	  update = 1
      else:
        if self._groups.has_key(group):
	  services = self._groups[group]
	  if services.has_key(service):
	    count = services[service] - 1
	    if count == 0:
	      del self._groups[group][service]
	      if len(self._groups[group].keys()) == 0:
	        del self._groups[group]
	      update = 1
	    else:
	      self._groups[group][service] = count

    if update != 0:
      self._version = self._version + 1

  def handlePublisherNotification(self,notification):
    name = notification.publisher().serviceName()
    agent = notification.publisher().agentIdentity()

    self._messages.append("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
    self._messages.append("EVENT     = 'ANNOUNCEMENT'\n")
    self._messages.append("SERVICE   = %s\n" % `name`)
    self._messages.append("AGENT     = %s\n" % `agent`)

    if notification.status() == netsvc.SERVICE_AVAILABLE:
      self._messages.append("STATUS    = 'AVAILABLE'\n")
    else:
      self._messages.append("STATUS    = 'WITHDRAWN'\n")

    self._messages.append("\n")

    self._version = self._version + 1

  def handleReport(self,report):
    self._reports.append(report)

    name = report.publisher().serviceName()
    agent = report.publisher().agentIdentity()
    subject = report.subjectName()
    content = str(repr(report.content()))

    self._messages.append("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
    self._messages.append("EVENT     = 'REPORT'\n")
    self._messages.append("SERVICE   = %s\n" % `name`)
    self._messages.append("AGENT     = %s\n" % `agent`)
    self._messages.append("SUBJECT   = %s\n" % `subject`)
    self._messages.append("CONTENT   = %s\n" % content)
    self._messages.append("\n")

    self._version = self._version + 1


class MainWindow:

  def __init__(self,root):
    name = "spy@%s" % netsvc.processIdentity()

    self._root = root
    self._root.title("SPYON")
    self._root.protocol("WM_DELETE_WINDOW",self._destroyWindow)

    self._serviceFilter = "*"
    self._groupFilter = "*"

    self._publisher = ""
    self._subjects = {}

    self._logState = "stopped"
    self._eventLog = ""

    self._version = 0

    p1 = Pmw.PanedWidget(self._root,orient="horizontal",
        hull_width=1000,hull_height=700)
    p1.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    p1.add("left",size=400)
    p1.add("right")

    p2 = Pmw.PanedWidget(p1.pane("left"),orient="vertical")
    p2.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    p2.add("top",size=200)
    p2.add("bottom")

    g1 = Pmw.Group(p2.pane("top"),tag_text="Groups")
    g1.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    self._groupView = GroupBrowser(g1.interior(),
     catalog_dblclickcommand=self._filterGroup)
    self._groupView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    g2 = Pmw.Group(p2.pane("bottom"),tag_text="Services")
    g2.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    self._serviceView = ServiceBrowser(g2.interior(),
     catalog_dblclickcommand=self._browseService)
    self._serviceView.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    g3 = Pmw.Group(p1.pane("right"),tag_text="Service")
    g3.pack(side="top",padx=8,pady=8,fill="both",expand="true")

    #p3 = Pmw.PanedWidget(p1.pane("right"),orient="vertical")
    p3 = Pmw.PanedWidget(g3.interior(),orient="vertical")
    p3.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    p3.add("top",size=500)
    p3.add("bottom")

    #g3 = Pmw.Group(p3.pane("top"),tag_text="Service")
    #g3.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    f3a = Tkinter.Frame(p3.pane("top"))
    f3a.pack(side="top",fill="both",expand="true")

    #f3 = Tkinter.Frame(g3.interior())
    #f3 = Tkinter.Frame(p3.pane("top"))
    f3 = Tkinter.Frame(f3a)
    f3.pack(side="top")

    self._subscribeField = Pmw.EntryField(f3,labelpos='w',entry_width=25,
        label_text="Name:",validate=None,value="",
	command=self._subscribeService)
    self._subscribeField.pack(side="left",padx=4,pady=4)

    b2 = Pmw.ButtonBox(f3,orient="horizontal")
    b2.pack(side="left",padx=4,pady=4)
    b2.add("Subscribe",command=self._subscribeService)

    #self._subjectView = Pmw.ScrolledListBox(g3.interior(),listbox_height=12)
    #self._subjectView = Pmw.ScrolledListBox(p3.pane("top"),listbox_height=12)
    self._subjectView = Pmw.ScrolledListBox(f3a,listbox_height=15)
    self._subjectView.pack(side="top",padx=4,pady=8,fill="both",expand="true")

    #g4 = Pmw.Group(p3.pane("bottom"),tag_text="Events")
    #g4.pack(side="top",padx=4,pady=4,fill="both",expand="true")

    #f4 = Tkinter.Frame(g4.interior())
    f4 = Tkinter.Frame(p3.pane("bottom"))
    f4.pack(side="top",fill="both",expand="true")

    #self._eventView = Pmw.ScrolledText(g4.interior(),text_padx=4,
    #self._eventView = Pmw.ScrolledText(p3.pane("bottom"),text_padx=4,
    self._eventView = Pmw.ScrolledText(f4,text_padx=4,
        text_pady=4,text_wrap="char",text_state="disabled",
	text_height=10,borderframe=1)
    #	text_height=10,vscrollmode="static",borderframe=1)
    self._eventView.yview("moveto",1.0)
    self._eventView.pack(side="top",padx=4,pady=8,fill="both",expand="true")

    self._eventButtons = Pmw.ButtonBox(f4,orient="horizontal")
    self._eventButtons.pack(side="bottom",padx=8,pady=8)
    self._eventButtons.add("Start",width=7,command=self._startLog)
    self._eventButtons.add("Suspend",width=7,state="disabled",
        command=self._suspendLog)
    self._eventButtons.add("Clear",width=7,command=self._clearLog)

    self._agent = MainWindowAgent(name).synchronisedObject()

    self._timer = self._root.after(100,self._wakeup)

  def _wakeup(self):
    if self._agent is not None:
      version = self._agent.modelVersion()
      if version != self._version:
	self._version = version
	self._updateReports()
	self._updateMessages()
      self._timer = self._root.after(100,self._wakeup)

  def _destroyWindow(self):
    if self._timer:
      self._root.after_cancel(self._timer)
      self._timer = None
    if self._agent is not None:
      self._agent.destroyReferences()
      self._agent = None
    self._root.destroy()

  def _browseService(self):
    services = self._serviceView.getcurselection()
    if len(services) != 0:
      child = AgentWindow(servicename=services[0])

  def _filterGroup(self):
    groups = self._groupView.getcurselection()
    if len(groups) != 0:
      self._serviceView.setgroup(groups[0])

  def _browseGroup(self):
    groups = self._groupView.getcurselection()
    if len(groups) != 0:
      child = ServiceWindow(groupfilter=groups[0])

  def _subscribeService(self):
    if self._publisher != "":
      self._subjectView.clear()
      self._subjects = {}
      self._agent.unsubscribeReports(self._publisher,"*")
      self._appendLog("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
      self._appendLog("EVENT     = 'UNSUBSCRIBE'\n")
      self._appendLog("SERVICE   = %s\n" % `self._publisher`)
      self._appendLog("\n")
      self._publisher = self._subscribeField.get()
      if self._publisher != "":
	self._appendLog("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
	self._appendLog("EVENT     = 'SUBSCRIBE'\n")
	self._appendLog("SERVICE   = %s\n" % `self._publisher`)
	self._appendLog("\n")
	self._agent.subscribeReports(self._publisher,"*")
    else:
      self._publisher = self._subscribeField.get()
      if self._publisher != "":
	self._appendLog("TIMESTAMP = %s\n" % `str(netsvc.DateTime())`)
	self._appendLog("EVENT     = 'SUBSCRIBE'\n")
	self._appendLog("SERVICE   = %s\n" % `self._publisher`)
	self._appendLog("\n")
	self._agent.subscribeReports(self._publisher,"*")

  def _clearLog(self):
    self._eventView.configure(text_state="normal")
    self._eventView.clear()
    self._eventView.configure(text_state="disabled")
    self._eventView.yview("moveto",1.0)

  def _appendLog(self,message):
    if self._logState == "running":
      p1,p2 = self._eventView.yview()
      self._eventView.configure(text_state="normal")
      self._eventView.insert("end",message)
      self._eventView.configure(text_state="disabled")
      if (p1 == 0.0 and p2 == 0.0) or p2 == 1.0:
	self._eventView.yview("moveto",1.0)
    elif self._logState == "paused":
      self._eventLog = self._eventLog + message

  def _startLog(self):
    if self._logState == "stopped":
      self._logState = "running"
      self._eventButtons.button(0).configure(text="Stop")
      self._eventButtons.button(1).configure(state="normal")
    elif self._logState == "running":
      self._logState = "stopped"
      self._eventButtons.button(0).configure(text="Start")
      self._eventButtons.button(1).configure(state="disabled")
    elif self._logState == "paused":
      self._logState = "stopped"
      self._eventButtons.button(0).configure(text="Start")
      self._eventButtons.button(1).configure(text="Suspend")
      self._eventButtons.button(1).configure(state="disabled")

  def _suspendLog(self):
    if self._logState == "running":
      self._logState = "paused"
      self._eventButtons.button(1).configure(text="Resume")
    elif self._logState == "paused":
      self._logState = "running"
      self._eventButtons.button(1).configure(text="Suspend")
      if self._eventLog != "":
	p1,p2 = self._eventView.yview()
	self._eventView.configure(text_state="normal")
	self._eventView.insert("end",self._eventLog)
	self._eventView.configure(text_state="disabled")
	if (p1 == 0.0 and p2 == 0.0) or p2 == 1.0:
	  self._eventView.yview("moveto",1.0)
	self._eventLog = ""

  def _updateReports(self):
    for report in self._agent.queuedReports():
      subject = report.subjectName()
      content = str(report.content())
      self._subjects[subject] = content

    subjects = self._subjects.keys()
    subjects.sort()

    view = []
    for subject in subjects:
      view.append(subject + " = " + self._subjects[subject])
    self._subjectView.setlist(view)

  def _updateMessages(self):
    for message in self._agent.queuedMessages():
      self._appendLog(message)

if __name__ == "__main__":
  dispatcher.monitor(signal.SIGINT)

  root = Pmw.initialise()
  root.withdraw()

  main = MainWindow(root)

  root.update()

  target = None
  dialog = None

  exchange = None

  def startup(target):
    if target != None:
      if dialog != None:
	dialog.deactivate()

      try:
	group = ""
	group,address = string.splitfields(target,'@')
      except:
	host,port = string.splitfields(target,':')
      else:
	host,port = string.splitfields(address,':')

      global exchange
      exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,group)
      exchange.connect(host,int(port),5)

      root.deiconify()
      root.update_idletasks()
    else:
      if dialog != None:
	dialog.deactivate()
	dispatcher.shutdown(0.2)

  if len(sys.argv) == 2:
    target = sys.argv[1]
  else:
    dialog = StartupDialog(root,command=startup)
    dialog.withdraw()

  def run():
    dispatcher.run()
    global root
    root.quit()

  thread = threading.Thread(target=run)
  thread.start()

  if dialog != None:
    dialog.activate()
  else:
    startup(target)

  root.mainloop()
  dispatcher.shutdown(0.2)

import Tkinter
import Pmw
import fnmatch

from ServiceTracker import ServiceTracker

class ServiceBrowser(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
      ("namefilter","*",Pmw.INITOPT),
      ("groupfilter","*",Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._version = 0

    self._nameFilter = self["namefilter"]
    self._groupFilter = self["groupfilter"]

    f1 = Tkinter.Frame(self.interior())
    f1.pack(side="top")

    f2 = Tkinter.Frame(f1)
    f2.pack(side="left")

    self._groupField = self.createcomponent("group",(),None,
        Pmw.EntryField,(f2,),labelpos="w",entry_width=25,
	label_text="Group:",validate=None,value=self["groupfilter"],
	command=self._applyFilters)
    self._groupField.pack(side="top",padx=8,pady=8)

    self._serviceField = self.createcomponent("service",(),None,
        Pmw.EntryField,(f2,),labelpos="w",entry_width=25,
        label_text="Name:",validate=None,value=self["namefilter"],
        command=self._applyFilters)
    self._serviceField.pack(side="top",padx=8,pady=8)

    Pmw.alignlabels((self._groupField,self._serviceField))

    b1 = Pmw.ButtonBox(f1,orient="vertical")
    b1.pack(side="left",padx=8,pady=8)
    b1.add("Filter",command=self._applyFilters)   
    b1.add("Reset",command=self._resetFilters)

    #self._servicesView = self.createcomponent("catalog",(),None,
    #    Pmw.ScrolledListBox,(self.interior(),),listbox_height=18)
    self._servicesView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),))
    self._servicesView.pack(side="top",fill="both",expand="true")

    self.bind("<Destroy>", self._widgetDestroyed)

    Pmw.forwardmethods(ServiceBrowser,Pmw.ScrolledListBox,"_servicesView")

    self._monitor = ServiceTracker()
    self._monitor = self._monitor.synchronisedObject()

    self.after(100,self._wakeup)

  def setgroup(self,name):
    self._groupField.setvalue(name)
    self._groupField.invoke()

  def _wakeup(self):
    try:
      self._monitor.acquireObject()
      version = self._monitor.snapshotVersion()
      if version != self._version:
	self._version = version
	self._updateServices()
      self._monitor.releaseObject()
      self.after(100,self._wakeup)
    except:
      self._monitor.releaseObject()
      raise

  def _widgetDestroyed(self,*args):    
    self._monitor.destroyReferences()
    self._monitor = None

  def _applyFilters(self):
    self._nameFilter = self._serviceField.get()
    self._groupFilter = self._groupField.get()              
    self._updateServices()     

  def _resetFilters(self):
    self._serviceField.setentry("*")
    self._groupField.setentry("*")
    self._applyFilters()

  def _updateServices(self):
    if self._groupFilter == "*":      
      services = []
      for service in self._monitor.registeredServices().keys():
        if fnmatch.fnmatch(service,self._nameFilter):      
          services.append(service)  
      services.sort()
      self._servicesView.setlist(services)
    else:
      services = []
      for group in self._monitor.registeredGroups():
        if fnmatch.fnmatch(group,self._groupFilter):
          for service in self._monitor.registeredServices(group).keys():
            if fnmatch.fnmatch(service,self._nameFilter):
              services.append(service)
      services.sort()
      self._servicesView.setlist(services)

    self.update_idletasks()


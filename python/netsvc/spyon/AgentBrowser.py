import Tkinter
import Pmw
import fnmatch

from AgentTracker import AgentTracker

class AgentBrowser(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
      ("servicename","",Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._version = 0

    #self._nameFilter = self["namefilter"]
    #self._groupFilter = self["groupfilter"]

    f1 = Tkinter.Frame(self.interior())
    f1.pack(side="top")

    #f2 = Tkinter.Frame(f1)
    #f2.pack(side="left")

    e1 = Pmw.EntryField(f1,labelpos='w',entry_width=25,
        label_text="Name:",validate=None,value=self["servicename"],
        entry_state="disabled")
    e1.pack(side="top",padx=8,pady=8)

    self._agentsView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),),listbox_height=12)
    self._agentsView.pack(side="top",fill="both",expand="true")

    """
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

    self._servicesView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),),listbox_height=18)
    self._servicesView.pack(side="top",fill="both",expand="true")
    """

    self.bind("<Destroy>", self._widgetDestroyed)

    Pmw.forwardmethods(AgentBrowser,Pmw.ScrolledListBox,"_servicesView")

    self._monitor = AgentTracker(self["servicename"])
    self._monitor = self._monitor.synchronisedObject()

    self.after(100,self._wakeup)

  def _wakeup(self):
    try:
      self._monitor.acquireObject()
      version = self._monitor.snapshotVersion()
      if version != self._version:
	self._version = version
	self._updateAgents()
      self._monitor.releaseObject()
      self.after(100,self._wakeup)
    except:
      self._monitor.releaseObject()
      raise

  def _widgetDestroyed(self,*args):    
    self._monitor.destroyReferences()
    self._monitor = None

  def _updateAgents(self):
    agents = list(self._monitor.registeredAgents().keys())
    agents.sort()
    self._agentsView.setlist(agents)

    self.update_idletasks()


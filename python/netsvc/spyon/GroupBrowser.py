import Tkinter
import Pmw

from ServiceTracker import ServiceTracker

class GroupBrowser(Pmw.MegaWidget):

  def __init__(self,parent=None,**kw):
    optiondefs = (
    )

    self.defineoptions(kw,optiondefs)

    Pmw.MegaWidget.__init__(self,parent)

    self._version = 0

    #self._groupsView = self.createcomponent("catalog",(),None,
    #    Pmw.ScrolledListBox,(self.interior(),),listbox_height=6)
    self._groupsView = self.createcomponent("catalog",(),None,
        Pmw.ScrolledListBox,(self.interior(),))
    self._groupsView.pack(side="top",fill="both",expand="true")

    self.bind("<Destroy>", self._widgetDestroyed)

    Pmw.forwardmethods(GroupBrowser,Pmw.ScrolledListBox,"_groupsView")

    self._monitor = ServiceTracker()
    self._monitor = self._monitor.synchronisedObject()

    self.after(100,self._wakeup)

  def _wakeup(self):
    try:
      self._monitor.acquireObject()
      version = self._monitor.snapshotVersion()
      if version != self._version:
	self._version = version
	self._updateGroups()
      self._monitor.releaseObject()
      self.after(100,self._wakeup)
    except:
      self._monitor.releaseObject()
      raise

  def _widgetDestroyed(self,*args):    
    self._monitor.destroyReferences()
    self._monitor = None

  def _updateGroups(self):
    groups = self._monitor.registeredGroups()
    groups.sort()
    self._groupsView.setlist(groups)

    self.update_idletasks()


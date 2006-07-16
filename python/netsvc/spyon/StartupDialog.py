import Tkinter
import Pmw

class StartupDialog(Pmw.Dialog):

  def __init__(self,parent=None,**kw):
    optiondefs = (
      ("title","SPYON",Pmw.INITOPT),
      ("address","localhost:11111",Pmw.INITOPT),
      ("command",None,Pmw.INITOPT),
    )

    self.defineoptions(kw,optiondefs)

    Pmw.Dialog.__init__(self,parent)

    f1 = Tkinter.Frame(self.interior())
    f1.pack(side="top",padx=4,pady=4)

    self._address = Pmw.EntryField(f1,labelpos='w',entry_width=25,
        label_text="Address:",validate=None,value=self["address"],
	command=self._ok)
    self._address.pack(side="top",padx=8,pady=8)

    b1 = Pmw.ButtonBox(f1)
    b1.pack(side="top",padx=8,pady=8)
    b1.add("OK",command=self._ok)
    b1.add("Cancel",command=self._cancel)

    self.bind("<Destroy>", self._cancel)

  def _ok(self,*args):
    self["command"](self._address.getvalue())

  def _cancel(self,*args):
    self["command"](None)

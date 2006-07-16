import netsvc
import signal

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()

class Agent(netsvc.Agent):

  def __init__(self):

    netsvc.Agent.__init__(self)

    self.count = 0

    self.scheduleAction(self.callback1,netsvc.IDLE_JOB,"1")
    self.scheduleAction(self.callback1,netsvc.STANDARD_JOB,"2")
    self.scheduleAction(self.callback1,netsvc.PRIORITY_JOB,"3")

    self.scheduleAction(self.callback1,netsvc.IDLE_JOB,"4")
    self.scheduleAction(self.callback3,netsvc.IDLE_JOB,"5")

    self.scheduleAction(self.callback4,"* * * * *")

  def callback1(self,name):

    if name == "1":
      self.cancelAction("4")

    logger.notify(netsvc.LOG_DEBUG,'self.callback1 ' + str(name))

    self.startTimer(self.callback2,4,"5")

  def callback2(self,name):

    logger.notify(netsvc.LOG_DEBUG,'self.callback2 ' + str(name))

    self.scheduleAction(self.callback1,netsvc.IDLE_JOB,"6")

  def callback3(self):

    logger.notify(netsvc.LOG_DEBUG,'self.callback3')

  def callback4(self,name):

    logger.notify(netsvc.LOG_DEBUG,'self.callback4 ' + str(name))



dispatcher.monitor(signal.SIGINT)

object = Agent()

dispatcher.run()

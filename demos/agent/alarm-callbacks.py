import netsvc
import signal
import time

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()

class Agent(netsvc.Agent):

  def __init__(self):
    netsvc.Agent.__init__(self)
    clock = int(time.time())
    self.setAlarm(self.callback1,clock+2,"1")
    self.setAlarm(self.callback2,clock+10,"2")
    self.setAlarm(self.callback3,clock+20,"3")
    self.setAlarm(self.callback4,clock+5,"4")
    self.subscribeShutdown(self.callback5)
    logger.notify(netsvc.LOG_DEBUG,'start alarms')

  def callback1(self,name):
    logger.notify(netsvc.LOG_DEBUG,'alarm1 triggered')
    clock = int(time.time())
    self.setAlarm(self.callback1,clock+2,"1")

  def callback2(self,name):
    logger.notify(netsvc.LOG_DEBUG,'alarm2 triggered')
    self.cancelAlarm("1")

  def callback3(self,name):
    logger.notify(netsvc.LOG_DEBUG,'alarm3 triggered')
    self.scheduleShutdown()

  def callback4(self):
    logger.notify(netsvc.LOG_DEBUG,'alarm4 triggered')

  def callback5(self,category):
    logger.notify(netsvc.LOG_DEBUG,'shutdown triggered')
    logger.notify(netsvc.LOG_DEBUG,'category=%d' % category)
    if category == netsvc.SHUTDOWN_PENDING:
      self.suspendShutdown()
      self.startTimer(self.callback5,5,"resume")

  def callback5(self,name):
    logger.notify(netsvc.LOG_DEBUG,'timer5 triggered')
    self.resumeShutdown()


dispatcher.monitor(signal.SIGINT)

object = Agent()

netsvc.Dispatcher().run()

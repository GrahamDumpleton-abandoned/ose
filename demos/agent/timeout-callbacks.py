import netsvc
import signal

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()

class Agent(netsvc.Agent):

  def __init__(self):

    netsvc.Agent.__init__(self)

    self.startTimer(self.callback1,2,"1")
    self.startTimer(self.callback2,10,"2")
    self.startTimer(self.callback3,20,"3")
    self.startTimer(self.callback4,5,"4")

    self.subscribeShutdown(self.callback5)
    self.unsubscribeShutdown()
    self.subscribeShutdown(self.callback5)

    logger.notify(netsvc.LOG_DEBUG,'start timers')

  def callback1(self,name):

    logger.notify(netsvc.LOG_DEBUG,'timer1 triggered')

    self.startTimer(self.callback1,2,"1")

  def callback2(self,name):

    logger.notify(netsvc.LOG_DEBUG,'timer2 triggered')

    self.cancelTimer("1")

  def callback3(self,name):

    logger.notify(netsvc.LOG_DEBUG,'timer3 triggered')

    dispatcher.shutdown()

  def callback4(self):

    logger.notify(netsvc.LOG_DEBUG,'timer4 triggered')

  def callback5(self,category):

    logger.notify(netsvc.LOG_DEBUG,'shutdown ' + str(category))


dispatcher.monitor(signal.SIGINT)

object = Agent()

dispatcher.run()

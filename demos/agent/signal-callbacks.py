import netsvc
import signal

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()

class Agent(netsvc.Agent):

  def __init__(self):

    netsvc.Agent.__init__(self)

    self.subscribeSignal(self.callback1,signal.SIGINT)

    if hasattr(signal,'SIGHUP'):
      self.subscribeSignal(self.callback1,signal.SIGHUP)

    if hasattr(signal,'SIGUSR1'):
      self.subscribeSignal(self.callback2,signal.SIGUSR1)

    if hasattr(signal,'SIGUSR2'):
      self.subscribeSignal(self.callback3,signal.SIGUSR2)

  def callback1(self,num):

    logger.notify(netsvc.LOG_DEBUG,'self.callback1 ' + str(num))

  def callback2(self,num):

    logger.notify(netsvc.LOG_DEBUG,'self.callback2 ' + str(num))

  def callback3(self,num):

    logger.notify(netsvc.LOG_DEBUG,'self.callback3 ' + str(num))

    self.unsubscribeSignal(signal.SIGUSR1)


dispatcher.monitor(signal.SIGINT)

object = Agent()

dispatcher.run()

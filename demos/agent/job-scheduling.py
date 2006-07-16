import netsvc
import signal

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()


class Object:

  def __init__(self):
    self.count = 0
    dispatcher.schedule(self.callback1,netsvc.PRIORITY_JOB)

  def callback1(self):
    self.count = self.count + 1
    logger.notify(netsvc.LOG_DEBUG,'callback1 ' + str(self.count))
    if self.count != 5:
      dispatcher.schedule(self.callback1,netsvc.STANDARD_JOB)


def callback2(count):
  count = count + 1
  logger.notify(netsvc.LOG_DEBUG,'callback2 ' + str(count))
  if count != 5:
    job = netsvc.Job(callback2,(count,))
    dispatcher.schedule(job,netsvc.STANDARD_JOB)


def callback3(count):
  count = count + 1
  logger.notify(netsvc.LOG_DEBUG,'callback3 ' + str(count))
  if count != 5:
    job = netsvc.Job(callback3,(count,))
    job.schedule(netsvc.STANDARD_JOB)


def callback4(count):
  count = count + 1
  logger.notify(netsvc.LOG_DEBUG,'callback4 ' + str(count))
  if count != 5:
    job = netsvc.Job(callback4)
    job.schedule(netsvc.STANDARD_JOB,(count,))


count = 0

def callback5():
  global count
  count = count + 1
  logger.notify(netsvc.LOG_DEBUG,'callback5 ' + str(count))
  if count == 10:
    dispatcher.shutdown()
  else:
    dispatcher.schedule(callback5,netsvc.IDLE_JOB)


dispatcher.monitor(signal.SIGINT)

object = Object()
job = netsvc.Job(callback2,(0,))
dispatcher.schedule(job,netsvc.PRIORITY_JOB)
job = netsvc.Job(callback3,(0,))
job.schedule(netsvc.PRIORITY_JOB)
job = netsvc.Job(callback4)
job.schedule(netsvc.PRIORITY_JOB,(0,))
dispatcher.schedule(callback5,netsvc.PRIORITY_JOB)

dispatcher.run()

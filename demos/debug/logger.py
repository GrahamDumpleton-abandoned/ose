import netsvc

logger = netsvc.Logger()
logger.enableLongFormat()

#logger.setLogFile('python.log')

def callback(channel,level,message):
  if channel == '@HIDDEN':
    logger.notify(level,message)


enabled = logger.stderrOutputEnabled()
logger.notify(netsvc.LOG_DEBUG,'enabled1 = %d' % enabled)

logger.disableStderrOutput()

enabled = logger.stderrOutputEnabled()
logger.notify(netsvc.LOG_DEBUG,'enabled2 = %d' % enabled)

logger.enableStderrOutput()

enabled = logger.stderrOutputEnabled()
logger.notify(netsvc.LOG_DEBUG,'enabled3 = %d' % enabled)

logfile = logger.logFile()
logger.notify(netsvc.LOG_DEBUG,'logfile = %s' % logfile)

logger.notify(netsvc.LOG_DEBUG,'NO-CHANNEL')

logger.notifyChannel('VISIBLE',netsvc.LOG_DEBUG,'VISIBLE')
logger.notifyChannel('@HIDDEN',netsvc.LOG_DEBUG,'@HIDDEN')

logger.monitorChannel('@HIDDEN',callback)
logger.notifyChannel('@HIDDEN',netsvc.LOG_DEBUG,'@HIDDEN')


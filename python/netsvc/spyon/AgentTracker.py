import netsvc


class AgentTracker(netsvc.Monitor):

  def __init__(self,name):
    netsvc.Monitor.__init__(self)
    self._agents = {}
    self._version = 0
    self.subscribeServiceName(self._processAnnouncements,name)

  def snapshotVersion(self):
    return self._version

  def registeredAgents(self):
    return self._agents

  def _processAnnouncements(self,binding,status):
    agent = binding.agentIdentity()

    update = 0
    if status == netsvc.SERVICE_AVAILABLE:
      self._agents[agent] = binding
      update = 1
    else:
      if self._agents.has_key(agent):
	del self._agents[agent]
	update = 1

    if update != 0:
      self._version = self._version + 1

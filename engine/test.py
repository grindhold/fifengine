import fife, time

print "Creating engine"
engine = fife.Engine()

print "Creating settings manager"
settings = fife.SettingsManager()

print "Testing log"
log = fife.Log.initialize(fife.Log.LEVEL_MAX, True, True)
fife.Log("Testing log...")


print "Testing timers"
class MyTimeEvent(fife.TimeEvent):
	def __init__(self, period):
		fife.TimeEvent.__init__(self, period)
		self.counter = 0
	
	def updateEvent(self, curtime):
		print "testing timer event... %d, %d" % (curtime, self.counter)
		self.counter += 1

m = fife.TimeManager()
e = MyTimeEvent(100)
m.registerEvent(e)

for i in xrange(10):
	time.sleep(0.1)
	m.update()

print "finishing"
m.unregisterEvent(e)
del e

m = fife.EventManager()
class MyEventListener(fife.ICommandListener):
	def __init__(self):
		fife.ICommandListener.__init__(self)
	
	def onCommand(self, command):
		print "received command with code %d" % command.getCode()		

l = MyEventListener()
m.addCommandListener(l)
cmd = fife.Command()
cmd.setCode(0)
print "Sending command..."
for i in xrange(20):
	m.dispatchCommand(cmd)
	cmd.setCode(i)
m.removeCommandListener(l)
del l


vfs = fife.VFSUtility()
print vfs.listFiles('.')
print vfs.listDirectories('.')
print vfs.readLines('test.py')
print vfs.readBytes('test.py')

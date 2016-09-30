import multiprocessing.managers

class ProcessGlobal(object):
    """This class is a singleton whose values are assigned in each subprocess.
    """

    def start(self, send, receive, memory):
        self.send = send
        self.receive = receive
        self.memory = memory

        self.allocated = 0
        self.address = ctypes.addressof(memory)

        self.objects = {}
        self.token = 0
        threading.Thread(target=self._run, daemon=True).start()

    def allocate(self, size):
        result = self.allocated
        if self.allocated + size > len(self.memory):
            raise ValueError('Out of memory: %s', (
                result, size, len(self.memory)))
        self.allocated += size
        return result

    def add_object(self, o):
       self.token += 1
       self.objects[self.token] = o
       return self.token

    def remove_object(self, o):
        for k, v in self.objects:
            if v is o:
                del self.objects[k]
                return
        else:
            raise ValueError('%s is not an saved object' % o)

    def _run(self):
        while True:
            #print('ProcessGlobal::about to get')
            token, method, args, kwds = self.send.get()
            #print('ProcessGlobal::got')
            try:
                result = method(token and self.objects[token], *args, **kwds)
            except Exception as e:
                result = 'Exception %s for "%s"' % (
                    e, (token, method, args, kwds))
            #print('ProcessGlobal::about to put')
            self.receive.put(result)
            #print('ProcessGlobal::put done')


_PROCESS = ProcessGlobal()


cdef void _send_from_juce(string s) with gil:
    _PROCESS.receive.put(s)
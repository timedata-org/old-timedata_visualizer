class ProcessGlobal(object):
    """This class is a singleton whose values are assigned in each subprocess.
    """

    def start(self, send, receive, events, memory):
        self.send = send
        self.receive = receive
        self.events = events
        self.memory = memory

        self.allocated = 0
        self.address = ctypes.addressof(memory)

        self.objects = {}
        self.token = 0
        threading.Thread(target=self._handle_incoming, daemon=True).start()

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
       o.set_token(self.token)
       return self.token

    def remove_object(self, o):
        for k, v in self.objects.items():
            if v is o:
                del self.objects[k]
                return
        else:
            raise ValueError('%s is not an saved object' % o)

    def event(self, event, data, token=None):
        self.events.put((event, data, token))

    def _handle_incoming(self):
        while True:
            token, method, args, kwds = self.send.get()
            try:
                if token:
                    msg = method(self.objects[token], *args, **kwds)
                else:
                    assert method is _add_proxy
                    msg = method(*args, **kwds)
            except Exception as e:
                msg = ('Exception %s for "%s"' % (
                    e, (token, method, args, kwds)))
                if not has_return(token, method):
                    # TODO: report errors back at the source.
                    print(msg)
            if has_return(token, method):
                self.receive.put(msg)


_PROCESS = ProcessGlobal()

def _add_proxy(proxy_class, *args, **kwds):
    # Must be top-level so it can be passed to another process.
    return _PROCESS.add_object(proxy_class(*args, **kwds))

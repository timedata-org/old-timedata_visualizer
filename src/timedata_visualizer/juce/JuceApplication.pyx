import atexit, ctypes, functools, multiprocessing, multiprocessing.sharedctypes
import queue, time, threading, traceback, weakref

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JuceApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil


cpdef void start_juce_application():
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    assert threading.current_thread() is threading.main_thread(), (
        'JuceApplication must run on main thread')

    with nogil:
        startJuceApplication(_send_from_juce)


def _juce_process(*args):
    _PROCESS.start(*args)
    start_juce_application()


class JuceApplication(object):
    INSTANCES = weakref.WeakSet()

    def __init__(self, size):
        self.INSTANCES.add(self)
        self.running = True
        self.proxies = weakref.WeakValueDictionary()
        ctx = multiprocessing.get_context('spawn')

        send = ctx.Queue()
        receive = ctx.Queue()
        events = ctx.Queue()

        memory = multiprocessing.sharedctypes.RawArray(ctypes.c_uint8, size)

        self._send, self._receive, self._events, self.memory = (
            send, receive, events, memory)
        threading.Thread(target=self._handle_events, daemon=True).start()

        self.process = ctx.Process(target=_juce_process,
                                   args=(send, receive, events, memory))
        self.process.start()
        result = receive.get()
        assert result == b'{"event":"start"}', str(result)


    def _handle_events(self):
        while True:
            event, data, token = self._events.get()
            if not token:
                call_callbacks(self, event)
            else:
                item = self.proxies.get(token)
                if not item:
                    print('Got an event for a missing token', token)
                elif event == 'desc':
                    item._desc = data
                else:
                    call_callbacks(item, (event, data))

    def __del__(self):
        try:
            self.quit()
        except:
            pass

    @classmethod
    def quit_all(cls):
        for i in cls.INSTANCES:
            i.__del__()

    def send(self, token, method, *args, **kwds):
        self._send.put((token, method, args, kwds))
        if has_return(token, method):
            return self._receive.get()

    def quit(self):
        if self.running:
            self.running = False
            try:
                self.send(True, quit_juce_application)
            except:
                pass
            self.process.terminate()

    @classmethod
    def register(cls, proxy_class):
        def method(self, *args, **kwds):
            proxy = Proxy(self, proxy_class, *args, **kwds)
            self.proxies[proxy.token] = proxy
            return proxy

        name = proxy_class.__name__
        if name.startswith('_'):
            name = name[1:]
        setattr(cls, name, method)


JuceApplication.register(_LightWindow)
atexit.register(JuceApplication.quit_all)

import functools, multiprocessing, queue, time, threading, traceback

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil

_TIMEOUT = 0.1


cpdef start_juce_application():
    print('About to start JUCE application.')
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    assert threading.current_thread() is threading.main_thread(), (
        'JuceApplication must run on main thread')

    with nogil:
        startJuceApplication(_send_from_juce)
    print('JUCE application has shut down.')


cpdef _start_process(from_juce, to_juce):
    _start_communication(from_juce, to_juce)
    start_juce_application()


_MP_CONTEXT = multiprocessing.get_context('spawn')
_Queue = _MP_CONTEXT.Queue
_Process = _MP_CONTEXT.Process


class JuceApplication(object):
    def __init__(self):
        send, receive = _Queue(), _Queue()
        _Process(target=_start_process, args=(send, receive)).start()
        result = receive.get()
        assert result == b'{"event":"start"}', str(result)

        self._send, self._receive = send, receive

    def send(self, x):
        print('JuceApplication:send')
        self._send.put(x)
        print('JuceApplication:receive')
        result = self._receive.get()
        print('JuceApplication:result', result)
        return result

    def quit(self):
        self.send('quit')

    def proxy(self, cls):
        return Proxy(self, cls)

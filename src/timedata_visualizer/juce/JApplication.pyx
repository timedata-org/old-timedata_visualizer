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

def _make_queues():
    # This can probably run more efficiently using mp.Pipe but we can switch
    # that when we're all done - queues are a well-understood way to handle
    # concurrency.
    return _MP_CONTEXT.Queue(), _MP_CONTEXT.Queue()


class JuceApplication(object):
    def __init__(self):
        send_receive = _make_queues()
        self.send, self.receive = send_receive
        self.process = _MP_CONTEXT.Process(
            target=_start_process, args=send_receive)
        self.start = self.process.start
        self.running = True

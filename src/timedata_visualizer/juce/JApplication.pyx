import functools, multiprocessing, queue, time, threading, traceback

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil

_TIMEOUT = 0.1


cpdef _start_juce_application():
    print('About to start JUCE application.')
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    assert threading.current_thread() is threading.main_thread(), (
        'JuceApplication must run on main thread')

    with nogil:
        startJuceApplication(_send_from_juce)
    print('JUCE application has shut down.')


cpdef _start(from_juce, to_juce):
    _start_communication(from_juce, to_juce)
    _start_juce_application()


class JuceApplication(object):
    def __init__(self):
        ctx = multiprocessing.get_context('spawn')

        send_receive = ctx.Queue(), ctx.Queue()
        self.send = send_receive[0].put
        self.receive = send_receive[1].get
        self.process = ctx.Process(target=_start, args=send_receive)
        self.start = self.process.start
        self.running = True

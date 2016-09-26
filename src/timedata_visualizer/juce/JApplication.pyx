import functools, multiprocessing, queue, time, threading, traceback

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil
    void receiveMessageToJuce(string);

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
        args = ctx.Queue(), ctx.Queue()
        self.from_juce, self.to_juce = args
        self.process = ctx.Process(target=_start, args=args)
        self.start = self.process.start
        self.running = True

    def __del__(self):
        quit()

    def quit(self):
        if self.running:
            self.running = False
            self.to_juce.put('quit')

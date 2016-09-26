import atexit, multiprocessing, queue, time, threading, traceback

from libcpp cimport bool
from libcpp.string cimport string

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil
    void quitJuceApplication() nogil
    void receiveMessageToJuce(string);

_TIMEOUT = 0.1

# _FROM_JUCE has to be a global variable so that perform_string_callback can be
# a pure C function.  This only gets assigned in a Juce process, not in the
# master process.
_FROM_JUCE = None


cdef void _send_from_juce(string s) with gil:
    _FROM_JUCE.put(s)


cpdef _forward_to_juce(q):
    while True:
        try:
            receiveMessageToJuce(q.get(timeout=_TIMEOUT))
        except queue.Empty:
            continue


def juce_application(to_juce, from_juce):
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    assert threading.current_thread() is threading.main_thread(), (
        'JApplication must run on main thread')

    global _FROM_JUCE
    assert not _FROM_JUCE
    _FROM_JUCE = from_juce

    threading.Thread(
        target=_forward_to_juce,
        args=(to_juce,),
        daemon=True).start()
    time.sleep(_TIMEOUT)  # Just to make sure. :-D
    start_juce_application()


def start_juce_application():
    print('About to start JUCE application.')
    with nogil:
        startJuceApplication(_send_from_juce)
    print('JUCE application has shut down.')


class JuceApplication(object):
    def __init__(self):
        ctx = multiprocessing.get_context('spawn')
        args = ctx.Queue(), ctx.Queue()
        self.to_juce, self.from_juce = args
        self.process = ctx.Process(target=juce_application, args=args)
        self.start = self.process.start
        self.running = True

    def __del__(self):
        quit()

    def quit(self):
        if self.running:
            self.running = False
            self.to_juce.put('quit')

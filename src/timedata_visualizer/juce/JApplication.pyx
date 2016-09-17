import atexit, multiprocessing, queue, time, threading, traceback

from libcpp cimport bool
from libcpp.string cimport string

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil
    void quitJuceApplication() nogil

_CALLBACKS = []
_QUEUE = queue.Queue()
_STARTED = False


cdef void perform_string_callback(string s) with gil:
    _QUEUE.put(s)


def start_application(pipe):
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    assert threading.current_thread() is threading.main_thread(), (
        'JApplication must run on main thread')

    def target():
        print('Waiting for JUCE queue')
        s = _QUEUE.get()
        print('JUCE application started (%s), calling back' % s)
        callback(s, _QUEUE)

    threading.Thread(target=target).start()
    time.sleep(0.1)

    print('About to start JUCE application.')
    with nogil:
        startJuceApplication(perform_string_callback)
    print('JUCE application has shut down.')


def quit_application():
    print('Quitting JUCE application')
    quitJuceApplication()

atexit.register(quit_application)


def start_test():
    def callback(s, q):
        print('callback...')
        time.sleep(3)
        print('...callback done')
        quit_application()
    start_application(callback)


def start_application_in_process():
    ctx = multiprocessing.get_context('spawn')
    pipe = ctx.Pipe()
    process = ctx.Process(target=start_application, args=(pipe,)).start()
    return process, pipe

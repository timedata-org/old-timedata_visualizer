import atexit, queue, time, threading, traceback

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


def start_application(callback):
    assert threading.current_thread() is threading.main_thread(), (
        'JApplication must run on main thread')

    def target():
        print('Waiting for JUCE queue')
        s = _QUEUE.get()
        print('JUCE application started (%s), calling back' % s)
        callback()

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

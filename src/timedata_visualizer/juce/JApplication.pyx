import atexit, queue, time, threading, traceback

from libcpp cimport bool
from libcpp.string cimport string

ctypedef void (*StringCaller)(string)

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb) nogil
    void quitJuceApplication() nogil

_CALLBACKS = []

cpdef add_callback(cb):
    if cb not in _CALLBACKS:
        _CALLBACKS.append(cb)

cpdef remove_callback(cb):
    _CALLBACKS.remove(cb)

cpdef clear_callbacks():
    _CALLBACKS.clear()

cdef void perform_string_callback(string s) with gil:
    bad_callbacks = []

    for cb in _CALLBACKS:
        try:
            cb(s)
        except:
            traceback.print_exc(limit=100)
            bad_callbacks.append(cb)
    for cb in bad_callbacks:
        remove_callback(cb)


def _sleep():
    time.sleep(0.5) # HACK to avoid waiting for the message queue.


def start_application(callback):
    q = queue.Queue()
    def target():
        print('waiting for queue')
        q.get()
        print('application started, calling back')
        callback()

    threading.Thread(target=target).start()
    _sleep()

    print('about to startApplication')
    with nogil:
        startJuceApplication(perform_string_callback)
    print('startApplication done')


def quit_application():
    print('quitting JUCE')
    quitJuceApplication()

def register_quit():
    print('registering quit')
    atexit.register(quit_application)


def start_application_in_thread():
    def target():
        print('about to startApplication')
        with nogil:
            startJuceApplication(perform_string_callback)
        print('startApplication done')
    threading.Thread(target=target, daemon=True).start()
    print('start_application done')

# _start_application()

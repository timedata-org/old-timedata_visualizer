import atexit, time, traceback

from libcpp cimport bool
from libcpp.string cimport string

ctypedef void (*StringCaller)(void *user_data, string)

cdef extern from "<timedata/juce/JApplication.h>" namespace "timedata":
    void startJuceApplication(StringCaller cb, void* user_data) nogil
    void quit() nogil

_CALLBACKS = []

cpdef add_callback(cb):
    if cb not in _CALLBACKS:
        _CALLBACKS.append(cb)

cpdef remove_callback(cb):
    _CALLBACKS.remove(cb)

cpdef clear_callbacks():
    _CALLBACKS.clear()

cdef void perform_string_callback(void* f, string s) with gil:
    bad_callbacks = []

    for cb in _CALLBACKS:
        try:
            cb(s)
        except:
            traceback.print_exc(limit=100)
            bad_callbacks.append(cb)
    for cb in bad_callbacks:
        remove_callback(cb)


def start_application():
    def sleep():
        time.sleep(0.5) # HACK to avoid waiting for the message queue.

    def quit_juce():
        quit()
        sleep()
    print('registering quit')
    atexit.register(quit_juce)

    print('about to startApplication')
    with nogil:
        startApplication(perform_string_callback, callback)
    print('startApplication done')
    sleep()
    print('start_application done')

# _start_application()

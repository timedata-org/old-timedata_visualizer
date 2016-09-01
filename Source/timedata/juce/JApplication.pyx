# include "timedata/util/AppCallback.pyx"

from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "timedata/util/JApplication.h" namespace "timedata":
    void startApplication(StringCaller cb, void* user_data) nogil
    bool isStarted()

cdef extern from "timedata/util/Quit.h" namespace "timedata":
    void quit() nogil

def start_application(callback):
    with nogil:
        startApplication(perform_string_callback, callback)

def stop_application():
    quit()

def is_started():
    return isStarted()

import multiprocessing.managers

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()


# _RECEIVE has to be a global variable so that _send_from_juce can be a pure C++
# function.  It gets assigned in each Juce process, but not in the master
# process.
_RECEIVE = None

cdef void _send_from_juce(string s) with gil:
    _RECEIVE.put(s)


def _route_message_to_juce(object s):
    if s == b'quit' or s == 'quit':
        quitJuceApplication()
        return
    try:
        # We're creating a new class instance!
        cls, send, receive = s
        juce_object = cls()

        def route():
            while True:
                method, args, kwds = send.get()
                receive.put(method(juce_object, *args, **kwds))

        threading.Thread(target=route, daemon=True).start()

    except Exception as e:
        print('Don\'t understand message %s (%s)' % (s, e))


def _start_communication(send, receive):
    global _RECEIVE
    assert not _RECEIVE
    _RECEIVE = receive

    def route():
        while True:
            receive.put(_route_message_to_juce(send.get()))

    threading.Thread(target=route, daemon=True).start()

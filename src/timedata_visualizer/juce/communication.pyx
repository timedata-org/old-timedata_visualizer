import functools, queue

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void receiveMessageToJuce(string);


# _FROM_JUCE has to be a global variable so that perform_string_callback can be
# a pure C function.  This only gets assigned in a Juce process, not in the
# master process.
_FROM_JUCE = None

cdef void _send_from_juce(string s) with gil:
    _FROM_JUCE.put(s)


@functools.singledispatch
def _route_message_to_juce(msg):
    raise TypeError('Don\'t understand %s' % msg)


@_route_message_to_juce.register(str)
def _(string msg):
    receiveMessageToJuce(msg)


@_route_message_to_juce.register(_LightWindowDesc)
def _(_LightWindowDesc desc):
    cdef _LightWindow lw = _LightWindow()
    lw.set_desc(desc)


cpdef _handle_juce_queue(q):
    while True:
        try:
            item = q.get(timeout=_TIMEOUT)
        except queue.Empty:
            continue
        _route_message_to_juce(item)


def _start_communication(from_juce, to_juce):
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    global _FROM_JUCE
    assert not _FROM_JUCE
    _FROM_JUCE = from_juce

    thread = threading.Thread(
        target=_handle_juce_queue,
        args=(to_juce,),
        daemon=True)
    thread.start()
    return thread

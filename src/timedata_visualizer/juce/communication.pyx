import functools, queue, multiprocessing.managers

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()


class JuceManager(multiprocessing.managers.BaseManager):
    pass

JuceManager.register('LightWindowProxy', _LightWindow)

# _FROM_JUCE has to be a global variable so that _send_from_juce can be
# a pure C function.  This gets assigned in each Juce process, but not in the
# master process.
_RECEIVE = None


cdef void _send_from_juce(string s) with gil:
    _RECEIVE(s)


@functools.singledispatch
def _route_message_to_juce(object msg, object manager):
    raise TypeError('Don\'t understand %s' % msg)


@_route_message_to_juce.register(_LightWindowDesc)
def _(_LightWindowDesc desc, object manager):
    _RECEIVE(manager.LightWindowProxy(desc))


@_route_message_to_juce.register(str)
def _(string s, object manager):
    if s == b'quit':
        quitJuceApplication()
    else:
        raise ValueError('Don\'t understand message %s' % s)


cpdef _handle_juce_queue(send):
    with JuceManager() as manager:
        while True:
            try:
                item = send.get(timeout=_TIMEOUT)
            except queue.Empty:
                continue
            _route_message_to_juce(item, manager)


def _start_communication(send, receive):
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    global _RECEIVE
    assert not _RECEIVE
    _RECEIVE = receive.put

    thread = threading.Thread(
        target=_handle_juce_queue,
        args=(send,),
        daemon=True)
    thread.start()
    return thread

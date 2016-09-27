import functools, queue, multiprocessing.managers

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()


class JuceManager(multiprocessing.managers.BaseManager):
    pass

JuceManager.register('LightWindowProxy', _LightWindow)

# _FROM_JUCE has to be a global variable so that _send_from_juce can be
# a pure C function.  This gets assigned in each Juce process, but not in the
# master process.
_FROM_JUCE = None


cdef void _send_from_juce(string s) with gil:
    _FROM_JUCE(s)


@functools.singledispatch
def _route_message_to_juce(object msg, object manager):
    raise TypeError('Don\'t understand %s' % msg)


@_route_message_to_juce.register(_LightWindowDesc)
def _(_LightWindowDesc desc, object manager):
    _FROM_JUCE(manager.LightWindowProxy(desc))


@_route_message_to_juce.register(str)
def _(string s, object manager):
    if s == b'quit':
        quitJuceApplication()
    else:
        raise ValueError('Don\'t understand message %s' % s)


cpdef _handle_juce_queue(q):
    with JuceManager() as manager:
        while True:
            try:
                item = q.get(timeout=_TIMEOUT)
            except queue.Empty:
                continue
            _route_message_to_juce(item, manager)


def _start_communication(from_juce, to_juce):
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    global _FROM_JUCE
    assert not _FROM_JUCE
    _FROM_JUCE = from_juce.put

    thread = threading.Thread(
        target=_handle_juce_queue,
        args=(to_juce,),
        daemon=True)
    thread.start()
    return thread

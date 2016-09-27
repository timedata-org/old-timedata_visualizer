import functools, queue, multiprocessing.managers

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()


# _FROM_JUCE has to be a global variable so that _send_from_juce can be
# a pure C function.  This gets assigned in each Juce process, but not in the
# master process.
_RECEIVE = None


cdef void _send_from_juce(string s) with gil:
    _RECEIVE(s)


@functools.singledispatch
def _route_message_to_juce(object msg):
    raise TypeError('Don\'t understand %s' % msg)


@_route_message_to_juce.register(_LightWindowDesc)
def _(_LightWindowDesc desc):
    # TODO:  if there's nothing here, the window fails to display!
    print('_LightWindowDesc!')


@_route_message_to_juce.register(str)
def _(string s):
    if s == b'quit':
        quitJuceApplication()
    else:
        raise ValueError('Don\'t understand message %s' % s)


cpdef _handle_juce_queue(send):
    while True:
        try:
            item = send.get(timeout=_TIMEOUT)
        except queue.Empty:
            continue
        _route_message_to_juce(item)


def _start_communication(send, receive):
    # This must be run on the main thread.  You can run it in a multiprocess,
    # but you must have started it with 'spawn'.
    global _RECEIVE
    assert not _RECEIVE
    _RECEIVE = receive.put

    def start_window():
        time.sleep(1)
        lw = _LightWindow(_LightWindowDesc())
        array = lw.set_dimensions(2, 2)
        for i in range(12):
            array[i] = 20 * i
        time.sleep(10)

    threading.Thread(target=start_window).start()

    thread = threading.Thread(
        target=_handle_juce_queue,
        args=(send,),
        daemon=True)
    thread.start()
    return thread

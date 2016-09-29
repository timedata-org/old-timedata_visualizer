import multiprocessing.managers

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()


# _RECEIVE has to be a global variable so that _send_from_juce can be a pure C++
# function.  It gets assigned in each Juce process, but not in the master
# process.
_RECEIVE = None

cdef void _send_from_juce(string s) with gil:
    _RECEIVE.put(s)


def _route_message_to_juce(msg, objects):
    try:
        if msg == b'quit' or msg == 'quit':
            print('! quit')
            quitJuceApplication()

        elif callable(msg):
            print('! new object')
            # We're creating a new class instance!
            token = len(objects)
            while token in objects:
                token += 1
            print('! new object', token)
            objects[token] = msg()

            print('! new object DONE')
            return token

        else:
            print('!method call')
            token, method, args, kwds = msg
            return method(objects[token], *args, **kwds)

    except Exception as e:
        return 'Don\'t understand message %s (%s)' % (msg, e)


def _start_communication(send, receive):
    global _RECEIVE
    _RECEIVE = receive

    def run():
        objects = {}
        while True:
            receive.put(_route_message_to_juce(send.get(), objects))

    threading.Thread(target=run, daemon=True).start()

import multiprocessing.managers

cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()


# _RECEIVE has to be a global variable so that _send_from_juce can be a pure C++
# function.  It gets assigned in each Juce process, but not in the master
# process.
_RECEIVE = None

cdef void _send_from_juce(string s) with gil:
    _RECEIVE.put(s)


def _start_communication(send, receive):
    global _RECEIVE
    _RECEIVE = receive
    objects = {}

    def run():
        while True:
            receive.put(route(send.get()))

    def route(msg):
        try:
            if msg == b'quit' or msg == 'quit':
                print('! quit')
                quitJuceApplication()
                return

            token, method, args, kwds = msg
            if token is not None:
                print('!method call')
                result = method(objects[token], *args, **kwds)
                print('!method call done', result)
                return result

            print('! new object')
            # We're creating a new class instance!
            token = len(objects)
            while token in objects:
                token += 1
            objects[token] = method()
            return token

        except Exception as e:
            if True:
                raise
            return 'Exception %s for "%s"' % (e, msg)


    threading.Thread(target=run, daemon=True).start()

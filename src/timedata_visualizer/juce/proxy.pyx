class Proxy(object):
    def __init__(self, app, cls):
        # Make a pair of queues just for this proxy.
        send, receive = _make_queues()

        # Send the class and queues to the remote app and wait for the result.
        app.send.put((cls, send, receive))
        result = app.receive.get()
        assert result is None

        # Now add a proxy method for each method in the class.
        def proxy(method):
            def forward(*args, **kwds):
                send((method, args, kwds))
                return receive()
            return forward

        for k, v in cls.__dict__.items():
            if not k.startswith('_') and callable(v):
                setattr(self, k, proxy(v))

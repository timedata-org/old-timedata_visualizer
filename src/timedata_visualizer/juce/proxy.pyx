class Proxy(object):
    def __init__(self, app, cls, *args, **kwds):
        token = app.send((None, cls, args, kwds))

        # Add a proxy method for each method in the class.
        def proxy(method):
            def forward(*args, **kwds):
                return app.send((token, method, args, kwds))
            return forward

        for k, v in cls.__dict__.items():
            if not k.startswith('_') and callable(v):
                setattr(self, k, proxy(v))

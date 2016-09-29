class Proxy(object):
    def __init__(self, app, cls):
        token = app.send(cls)

        # Add a proxy method for each method in the class.
        def proxy(method):
            def forward(*args, **kwds):
                return app.send((token, method, args, kwds))
            return forward

        for k, v in cls.__dict__.items():
            if not k.startswith('_') and callable(v):
                setattr(self, k, proxy(v))

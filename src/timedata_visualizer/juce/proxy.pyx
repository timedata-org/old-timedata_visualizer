def _add_proxy(_, cls, *args, **kwds):
    return _PROCESS.add_object(cls(*args, **kwds))


class Proxy(object):
    def __init__(self, app, cls, *args, **kwds):
        # Get a token from the other side.
        token = app.send(None, _add_proxy, cls, *args, **kwds)

        # Now add a proxy method for each method in the class.
        def proxy(method):
            return lambda *a, **k: app.send(token, method, *a, **k)

        for name, method in cls.__dict__.items():
            if callable(method) and not name.startswith('_'):
                setattr(self, name, proxy(method))

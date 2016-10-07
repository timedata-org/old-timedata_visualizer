import copy, functools

def _add_proxy(_, cls, *args, **kwds):
    return _PROCESS.add_object(cls(*args, **kwds))


class Proxy(object):
    def __init__(self, app, proxy_class, *args, **kwds):
        # Get a token from the other side.
        self.token = app.send(None, _add_proxy, proxy_class, *args, **kwds)
        if isinstance(self.token, str):
            raise ValueError(self.token)

        def proxy(method):
            @functools.wraps(method)
            def remote(*a, **k):
                return app.send(self.token, method, *a, **k)
            return remote

        for name, method in proxy_class.__dict__.items():
            if callable(method) and not name.startswith('_'):
                setattr(self, name, proxy(method))

        # We pretty up the interface of self.set_desc so we don't have to set
        # all the values in the description class, every time.
        self._desc = proxy_class.DESC()
        proxy_set_desc = self.set_desc

        @functools.wraps(proxy_set_desc)
        def set_desc(**kwds):
            if kwds:
                setattrs(self._desc, **kwds)
                proxy_set_desc(self._desc)
            return self._desc

        self.set_desc = set_desc

    def get_desc(self):
        return self._desc

    def __del__(self):
        try:
            self.reset()
        except:
            pass

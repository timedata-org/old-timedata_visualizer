import copy, functools, weakref

def _add_proxy(_, cls, *args, **kwds):
    return _PROCESS.add_object(cls(*args, **kwds))


class Proxy(object):
    def __init__(self, app, proxy_class, *args, **kwds):
        # Get a token from the other side.
        token = app.send(None, _add_proxy, proxy_class, *args, **kwds)

        # Now add a proxy method for each method in the original class.
        app_ref = weakref.ref(app)

        def proxy(method):
            @functools.wraps(method)
            def remote(*a, **k):
                app = app_ref()
                if not app:
                    raise ValueError('Main application has already quit')
                app.send(token, method, *a, **k)
            return remote

        for name, method in proxy_class.__dict__.items():
            if callable(method) and not name.startswith('_'):
                setattr(self, name, proxy(method))

        # We pretty up the interface of self.set_desc so we don't have to set
        # all the values in the description class, every time.
        desc = proxy_class.DESC()
        proxy_set_desc = self.set_desc

        @functools.wraps(proxy_set_desc)
        def set_desc(**kwds):
            if kwds:
                setattrs(desc, **kwds)
                proxy_set_desc(desc)
            return desc

        self.set_desc = set_desc

    def __del__(self):
        print('starting to reset proxy...')
        try:
            self.reset()
        except:
            pass
        print('reset proxy done.')

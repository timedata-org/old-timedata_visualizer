import weakref

_CALLBACK_TABLE = weakref.WeakKeyDictionary()


def add_callback(item, callback):
    _CALLBACK_TABLE.setdefault(item, weakref.WeakSet()).add(callback)


def call_callbacks(item, event):
    for cb in _CALLBACK_TABLE.get(item, ()):
        cb(event)

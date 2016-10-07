_METHODS_THAT_RETURN = set()


def returns(method):
    _METHODS_THAT_RETURN.add(method)
    return method


def has_return(token, method):
    return not token or method in _METHODS_THAT_RETURN

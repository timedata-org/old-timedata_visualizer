from functools import singledispatch

@singledispatch
def foo(x):
    print('generic', x)

@foo.register(str)
def _(x):
    print('str', x)

if __name__ == '__main__':
    foo(1)
    foo('x')

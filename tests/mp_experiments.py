import multiprocessing, time
from multiprocessing.managers import BaseManager

class MathsClass:
    def __init__(self, i):
        self.i = i

    def add(self, x, y):
        return x + y + self.i

    def mul(self, x, y):
        return x * y + self.i

    def set_i(self, i):
        self.i = i

    def get_i(self):
        return self.i

    def __call__(self, x):
        return self.i + x


class MyManager(BaseManager):
    pass

MyManager.register('Maths', MathsClass)

def target(maths=None):
    if not maths:
        return
    print('target', maths.get_i())
    maths.set_i(5)

    time.sleep(0.5)
    print('target', maths.get_i())
    maths.set_i(23)

    time.sleep(1)
    print('target', maths.get_i())


if __name__ == '__main__':
    with MyManager() as manager:
        ctx = multiprocessing.get_context('spawn')
        maths = manager.Maths(1)

        #process = ctx.Process(target=target, args=()).start()
        process = ctx.Process(target=target, args=(maths,)).start()
        time.sleep(0.1)
        print('source', maths.get_i())
        maths.set_i(2)

        time.sleep(1)
        print('source', maths.get_i())
        maths.set_i(17)

        time.sleep(1)

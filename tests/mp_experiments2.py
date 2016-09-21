import ctypes, multiprocessing, multiprocessing.sharedctypes, threading, time
from multiprocessing.managers import BaseManager

class MathsClass:
    def __init__(self, i):
        self.i = i

    def set(self, i):
        self.i = i

    def get(self):
        return self.i


class MyManager(BaseManager):
    pass

MyManager.register('Maths', MathsClass)

def call_wait(callback, *items, sleep=1.0):
    for i in items:
        callback(i)
        time.sleep(sleep)

RUNNING = True

def make_callback(maths, name):
    def callback(i):
        print('%s: get %s, set %s' % (name, maths.get(), i))
        maths.set(i)
    return callback

def other_process_manager(queue):
    with MyManager() as manager:
        maths = manager.Maths(1)
        queue.put(maths)
        time.sleep(0.3)
        call_wait(make_callback(maths, 'other'), 2, 4, 8, 10)

def other_process(q):
    # Start process manager I hope?
    t = threading.Thread(target=other_process_manager, args=(q,), daemon=True)
    t.start()

    # main thread blocks on Juce.
    while RUNNING:
        time.sleep(1.0)


def do_it():
#    with MyManager() as manager:
#        maths = manager.Maths(1)
    ctx = multiprocessing.get_context('spawn')
    q = ctx.Queue()
    process = None
    def target():
        while True:
            maths = q.get()
            print('gotit!')
            call_wait(make_callback(maths, 'primary'), 2, 4, 8, 10)

    process = ctx.Process(target=other_process, args=(q,), daemon=not True)
    process.start()
    time.sleep(10)
    global RUNNING
    RUNNING = False
    process.join()


if __name__ == '__main__':
    do_it()

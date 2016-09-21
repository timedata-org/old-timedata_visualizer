import multiprocessing, threading, time
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

def process_manager_thread(queue):
    with MyManager() as manager:
        maths = manager.Maths(1)
        queue.put(maths)

        time.sleep(0.5)
        print('process', maths.get())
        maths.set(5)

        time.sleep(1)
        print('process', maths.get())
        maths.set(100)

        time.sleep(1)
        print('process', maths.get())

def other_process(q):
    threading.Thread(target=process_manager_thread,
                     args=(q,), daemon=True).start()

    # In the real program, we start a GUI here, which blocks
    # indefinitely, but for the test I just sleep for 10 seconds.
    time.sleep(4)

def start_process():
    # 'spawn' is the only possible choice, given the GUI and the fact we
    # run on Windows and *nix.
    ctx = multiprocessing.get_context('spawn')
    queue = ctx.Queue()
    process = ctx.Process(target=other_process, args=(queue,), daemon=not True)
    process.start()

    maths = queue.get()

    print('main', maths.get())
    maths.set(2)
    time.sleep(1)

    print('main', maths.get())
    maths.set(23)
    time.sleep(1)
    process.join()


if __name__ == '__main__':
    start_process()

import ctypes, multiprocessing, multiprocessing.sharedctypes, time

def target(array):
    print('target address', hex(ctypes.addressof(array)))

def spawn():
    ctx = multiprocessing.get_context('spawn')
    array = multiprocessing.sharedctypes.RawArray(ctypes.c_uint8, 3 * 10240)
    print('source address', hex(ctypes.addressof(array)))
    process = ctx.Process(target=target, args=(array,)).start()
    time.sleep(0.5)

if __name__ == '__main__':
    spawn()

import random, time, unittest

import timedata_visualizer

LOOPS = 20
WIDTH = 128
HEIGHT = 96
SIZE = (WIDTH * HEIGHT) * 3

FRAME_RATE = not False

def start_and_stop():
    print('SIZE=', SIZE)
    app = timedata_visualizer.JuceApplication(SIZE)
    proxy = app.proxy(timedata_visualizer.LightWindow, WIDTH, HEIGHT)
    proxy.set_desc(timedata_visualizer.LightWindowDesc(
        instrumentPadding=1,
        # shape='ellipse',
        ))
    print('LENGTH=', len(app.memory))

    base = 0
    i = 0
    last_time = time.time()
    FRAMES = [bytearray(random.randrange(256) for i in range(SIZE)) for j in range(8)]
    while True:
        app.memory[:] = FRAMES[base]
        base = (base + 1) % len(FRAMES)
        proxy.repaint()
        # print('iteration', base)
        i += 1
        if not (i % LOOPS):
            next_time = time.time()
            if FRAME_RATE:
                print('frame rate:', LOOPS / (next_time - last_time))
            last_time = next_time


    time.sleep(100)
    # works fine without this, but want to make sure it stays up.
    app.quit()


class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        start_and_stop()


if __name__ == '__main__':
    start_and_stop()

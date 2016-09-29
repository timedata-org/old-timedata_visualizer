import random, time, unittest

import timedata_visualizer

frames = (
    [0, 0, 0, 255, 255, 255] * 2,
    [0] * 12,
    [255, 0, 0] * 2 + [0, 0, 255] * 2,
    [255, 0, 0] * 2 + [0, 255, 0] * 2,
    [255, 0, 0] * 2 + [0, 255, 255] * 2,
    )

LOOPS = 100

def start_and_stop():
    app = timedata_visualizer.JuceApplication(size=12)
    proxy = app.proxy(timedata_visualizer.LightWindow, 2, 2)
    print('LENGTH=', len(app.memory))

    base = 0
    i = 0
    last_time = time.time()
    while True:
        app.memory[:] = frames[base]
        base = (base + 1) % len(frames)
        proxy.repaint()
        # print('iteration', base)
        i += 1
        if not (i % LOOPS):
            next_time = time.time()
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

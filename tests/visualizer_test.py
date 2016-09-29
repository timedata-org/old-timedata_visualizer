import random, time, unittest

import timedata_visualizer

def start_and_stop():
    app = timedata_visualizer.JuceApplication()
    window = app.proxy(timedata_visualizer.LightWindow)
    array = window.set_dimensions(2, 2)

    for i in range(12):
        array[i] = random.randrange(0, 256)

    time.sleep(3)
    # works fine without this, but want to make sure it stays up.
    app.send.put('quit')


class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        start_and_stop()


if __name__ == '__main__':
    start_and_stop()

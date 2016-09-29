import random, time, unittest

import timedata_visualizer

def start_and_stop():
    app = timedata_visualizer.JuceApplication()
    proxy = app.proxy(timedata_visualizer.LightWindow, 2, 2)

    while True:
        for i in range(len(app.memory)):
            app.memory[i] = random.randrange(0, 256)

        print('iteration')
        proxy.repaint()

    time.sleep(100)
    # works fine without this, but want to make sure it stays up.
    app.quit()


class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        start_and_stop()


if __name__ == '__main__':
    start_and_stop()

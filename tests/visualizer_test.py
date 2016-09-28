import time, unittest

import timedata_visualizer

def start_and_stop():
    app = timedata_visualizer.JuceApplication()
    app.start()
    time.sleep(3)
    # works fine without this, but want to make sure it stays up.
    app.send.put('quit')


class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        start_and_stop()

if __name__ == '__main__':
    start_and_stop()

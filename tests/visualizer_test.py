import time, unittest

import timedata_visualizer

def start_and_stop():
    app = timedata_visualizer.JuceApplication()
    app.start()
    time.sleep(10)
    app.quit()


class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        start_and_stop()

if __name__ == '__main__':
    start_and_stop()

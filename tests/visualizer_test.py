import unittest

import timedata_visualizer

class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        callback = lambda s, q: timedata_visualizer.quit_application()
        timedata_visualizer.start_application(callback)

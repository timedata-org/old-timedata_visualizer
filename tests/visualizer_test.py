import unittest

import timedata_visualizer

class TestVisualizer(unittest.TestCase):
    def test_nothing(self):
        timedata_visualizer.start_application(
            timedata_visualizer.quit_application)

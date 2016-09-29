# cython: embedsignature=True
# cython: infer_types=True
#
# See http://docs.cython.org/en/latest/src/reference/compilation.html

include "src/timedata_visualizer/base/stl.pyx"
include "src/timedata_visualizer/base/base.pyx"
include "src/timedata_visualizer/base/timestamp.pyx"
include "src/timedata_visualizer/base/wrapper.pyx"
include "src/timedata_visualizer/juce/juce_process.pyx"
include "src/timedata_visualizer/juce/commands.pyx"

include "build/genfiles/timedata_visualizer/genfiles.pyx"

include "src/timedata_visualizer/component/LightWindow.pyx"
include "src/timedata_visualizer/juce/JApplication.pyx"
include "src/timedata_visualizer/juce/proxy.pyx"

print_startup_message()

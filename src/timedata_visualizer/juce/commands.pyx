cdef extern from "<timedata_visualizer/juce/JApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()

cpdef quit_juce_application():
    quitJuceApplication()

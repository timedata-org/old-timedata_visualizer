cdef extern from "<timedata_visualizer/juce/JuceApplication_inl.h>" namespace "timedata":
    void quitJuceApplication()

cpdef quit_juce_application(_):
    quitJuceApplication()

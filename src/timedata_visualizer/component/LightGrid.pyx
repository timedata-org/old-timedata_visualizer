cdef extern from "timedata/component/InstrumentGrid.h" namespace "timedata":
    cdef cppclass InstrumentGrid:
        InstrumentGrid()

        void setLights(FColorList)
        void setPaintingIsUnclipped(bool)
        void setLightCount(int)
        void setLabelStartsAtZero(bool)
        void setLayout(
          Point layout, Point size, Point padding,
          Point instrumentPadding, Point labelPadding)
        void setShape(bool isRect)
        void setShowLabel(bool)
        int getLightCount()
        float gamma()
        void setGamma(float gamma)

import ctypes

cdef extern from "timedata_visualizer/component/LightWindow_inl.h" namespace "timedata":
    cdef cppclass LightWindow:
        LightWindow()
        void setDesc(LightWindowDesc)
        void setLights(size_t width, size_t height, uint8_t* memory);
        void writeSnapshotToFile(string filename)


cdef class _LightWindow:
    cdef LightWindow cdata

    def __init__(self, _LightWindowDesc desc):
        self.cdata.setDesc(desc.cdata)

    cpdef void set_desc(self, _LightWindowDesc desc):
        self.cdata.setDesc(desc.cdata)

    cpdef void set_lights(self, size_t width, size_t height, object data):
        buffer = ctypes.addressof(data)
        self.cdata.setLights(width, height, <uint8_t*>(buffer))

    cpdef void write_snapshot_to_file(self, string name):
        self.cdata.writeSnapshotToFile(name)

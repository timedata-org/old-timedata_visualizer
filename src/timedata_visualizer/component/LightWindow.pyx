import ctypes, multiprocessing.sharedctypes

cdef extern from "timedata_visualizer/component/LightWindow_inl.h" namespace "timedata":
    cdef cppclass LightWindow:
        LightWindow()
        void setDesc(LightWindowDesc)
        void setLights(size_t width, size_t height, uint64_t memory);
        void writeSnapshotToFile(string filename)


cdef class _LightWindow:
    cdef LightWindow cdata
    cdef object array

    def __init__(self, _LightWindowDesc desc):
        self.cdata.setDesc(desc.cdata)

    cpdef void set_desc(self, _LightWindowDesc desc):
        self.cdata.setDesc(desc.cdata)

    cpdef object set_dimensions(self, size_t width, size_t height):
        cdef size_t size = 3 * width * height
        self.array = multiprocessing.sharedctypes.RawArray(ctypes.c_uint8, size)
        self.cdata.setLights(width, height, ctypes.addressof(self.array))
        return self.array

    cpdef void write_snapshot_to_file(self, string name):
        self.cdata.writeSnapshotToFile(name)

import ctypes, multiprocessing.sharedctypes

cdef extern from "timedata_visualizer/component/LightWindow_inl.h" namespace "timedata":
    cdef cppclass LightWindow:
        LightWindow()
        void setDesc(LightWindowDesc)
        void setLights(size_t width, size_t height, uint64_t memory);
        void writeSnapshotToFile(string filename)
        void repaint()


cdef class _LightWindow:
    cdef LightWindow cdata
    cdef size_t _offset

    def __init__(self, size_t width, size_t height):
        self.reallocate(width, height)

    cpdef void reallocate(self, size_t width, size_t height):
        cdef size_t size = 3 * width * height
        self._offset = _PROCESS.allocate(size)
        self.cdata.setLights(width, height, self._offset + _PROCESS.address)

    cpdef void repaint(self):
        self.cdata.repaint()

    cpdef void set_desc(self, _LightWindowDesc desc):
        self.cdata.setDesc(desc.cdata)

    cpdef void write_snapshot_to_file(self, string name):
        self.cdata.writeSnapshotToFile(name)

    cpdef size_t offset(self):
        return self._offset


locals()['LightWindow'] = _LightWindow

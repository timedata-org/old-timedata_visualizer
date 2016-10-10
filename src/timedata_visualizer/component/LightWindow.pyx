import ctypes, multiprocessing.sharedctypes


cdef extern from "timedata_visualizer/component/LightWindow_inl.h" namespace "timedata":
    cdef cppclass LightWindow:
        LightWindow()

        void reset()
        void setDesc(LightWindowDesc)
        LightWindowDesc getDesc()
        void setLights(size_t width, size_t height, uint64_t memory);
        void writeSnapshotToFile(string filename)
        void repaint()
        void setCallback(Function stringCallback)


cdef class _LightWindow:
    DESC = _LightWindowDesc

    cdef LightWindow cdata
    cdef _LightWindowDesc desc
    cdef size_t _offset
    cdef size_t token

    def __init__(self, size_t width, size_t height):
        self.reallocate(width, height)
        #self.cdata.setCallback(<uint64_t> self._receive_from_cpp)
        self.token = 0

    cpdef void set_token(self, size_t token):
        # print('set_token', token)
        self.token = token

    def _receive_from_cpp(self, event):
        print('_receive_from_cpp', event)
        _PROCESS.event(event, self.desc, self.token)

    cpdef void reallocate(self, size_t width, size_t height):
        cdef size_t size = 3 * width * height
        self._offset = _PROCESS.allocate(size)
        self.cdata.setLights(width, height, self._offset + _PROCESS.address)

    cpdef void repaint(self):
        self.cdata.repaint()

    cpdef void reset(self):
        self.cdata.reset()
        _PROCESS.remove_object(self)

    cpdef void set_desc(self, _LightWindowDesc desc):
        self.desc.cdata = desc.cdata
        self.cdata.setDesc(desc.cdata)

    cpdef void write_snapshot_to_file(self, string name):
        self.cdata.writeSnapshotToFile(name)

    @returns
    def offset(self):
        return self._offset

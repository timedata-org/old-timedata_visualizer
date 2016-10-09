cdef void _function_caller(void* f) with gil:
    try:
        (<object>f)()
    except:
        traceback.print_exc(limit=100)


cdef void _string_function_caller(void* f, string s) with gil:
    try:
        (<object>f)(s)
    except:
        traceback.print_exc(limit=100)


cdef extern from "<timedata_visualizer/juce/CallPython_inl.h>" namespace "timedata":
    void setPythonCaller[Caller](Caller)


ctypedef void* Function;

ctypedef void (*FunctionCaller)(Function)
ctypedef void (*StringFunctionCaller)(Function, string)


setPythonCaller(<FunctionCaller> _function_caller)
setPythonCaller(<StringFunctionCaller> _string_function_caller)

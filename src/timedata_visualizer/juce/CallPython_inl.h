#pragma once

#include <timedata_visualizer/juce/CallPython.h>

namespace timedata {

template <typename Caller>
Caller& pythonCaller() {
    static Caller c;
    return c;
}

template <typename Caller>
void setPythonCaller(Caller c) {
    pythonCaller<Caller>() = c;
}

inline void callPython(Function f) {
    if (f)
        pythonCaller<FunctionCaller>()(f);
}

inline void callPython(Function f, std::string s) {
    if (f)
        pythonCaller<StringFunctionCaller>()(f, s);
}

} // timedata

#pragma once

#include <string>

namespace timedata {

/** We have to pass Python function pointers in and out of C++ as void*. */
using Function = uint64_t;

typedef void (*FunctionCaller) (Function);
typedef void (*StringFunctionCaller) (Function, std::string);

template <typename Caller>
void setPythonCaller(Caller);

/** Call a Python function. */
void callPython(Function);

/** Call a Python function that takes a string. */
void callPython(Function, std::string);

} // timedata

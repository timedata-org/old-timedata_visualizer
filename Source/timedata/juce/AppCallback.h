#pragma once

namespace timedata {

typedef void (*VoidCaller)(void *callback);
typedef void (*StringCaller)(void *callback, string data);

} // namespace

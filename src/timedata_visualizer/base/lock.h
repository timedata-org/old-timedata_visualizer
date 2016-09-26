#include <mutex>

namespace timedata {

using Mutex = std::mutex;
using Lock = std::unique_lock<Mutex>;

} // timedata

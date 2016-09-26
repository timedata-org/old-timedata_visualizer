#include <algorithm>
#include <string>

namespace timedata {

inline std::string toCharString(size_t n) {
    std::string result;
    while (n) {
        result += ('a' + n % 26);
        n /= 26;
    }

    if (result.empty())
        result += 'a';
    else
        std::reverse(result.begin(), result.end());
    return result;
}

} // timedata

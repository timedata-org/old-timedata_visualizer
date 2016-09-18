#pragma once

namespace timedata {

template <typename Struct>
void clearStruct(Struct& s) {
    s = Struct{};
}

}  // timedata

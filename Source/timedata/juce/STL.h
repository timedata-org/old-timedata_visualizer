#pragma once

#include "timedata/base/Timedata.h"

namespace timedata {

template <typename Value, typename Container>
void erasePointer(Container* container, Value* v) {
    auto isCallback = [&](unique_ptr<Value> const& p) { return p.get() == v; };
    auto remover = remove_if(container->begin(), container->end(), isCallback);
    container->erase(remover, container->end());
}

}  // namespace timedata

#pragma once

#include <functional>

#include "timedata/base/Timedata.h"

namespace timedata {

typedef std::function<void()> VoidFunction;

template <class Callback>
class RunOnMessageThread : public CallbackMessage {
  public:
    RunOnMessageThread(Callback f) : callback_(f) {}
    virtual void messageCallback() { callback_(); }
    Callback callback_;
};

template <class Callback>
void runOnMessageThread(Callback cb) {
    (new RunOnMessageThread<Callback>(cb))->post();
}

}  // namespace timedata

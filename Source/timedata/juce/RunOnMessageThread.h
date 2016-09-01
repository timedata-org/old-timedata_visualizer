#pragma once

namespace timedata {

template <class Callback>
void runOnMessageThread(Callback cb) {
    struct RunOnMessageThread : CallbackMessage {
        void messageCallback() override { callback(); }
        Callback callback;
    };

    (new RunOnMessageThread{cb})->post();
}

}  // namespace timedata

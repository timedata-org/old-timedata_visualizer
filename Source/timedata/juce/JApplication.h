#pragma once

namespace timedata {

typedef void (*StringCaller)(void *callback, string data);

void startJuceApplication(StringCaller, void*);
void callTimedata(std::string const&);
void quit();


template <class Callback>
void runOnMessageThread(Callback cb) {
    struct RunOnMessageThread : CallbackMessage {
        void messageCallback() override { callback(); }
        Callback callback;
    };

    (new RunOnMessageThread{cb})->post();
}

}  // namespace timedata

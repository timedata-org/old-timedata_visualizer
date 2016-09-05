#pragma once

#include <JuceHeader.h>

namespace timedata {

using namespace juce;

typedef void (*StringCaller)(std::string data);

void startJuceApplication(StringCaller);
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

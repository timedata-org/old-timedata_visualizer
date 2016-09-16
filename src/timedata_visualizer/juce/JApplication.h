#pragma once

#include <JuceHeader.h>

namespace timedata {

using namespace juce;

typedef void (*StringCaller)(std::string data);

void startJuceApplication(StringCaller);
void callTimedata(std::string const&);
void quitJuceApplication();


template <class Callback>
void runOnMessageThread(Callback cb, std::string message = "") {
    struct RunOnMessageThread : CallbackMessage {
        void messageCallback() override {
            if (!message.empty())
                std::cerr << "starting " << message << "\n";
            callback();
            if (!message.empty())
                std::cerr << "finished " << message << "\n";
        }
        RunOnMessageThread(Callback cb, std::string m)
                : callback(cb), message(m) {
        }

        Callback callback;
        std::string message;
    };

    (new RunOnMessageThread(cb, message))->post();
}

}  // namespace timedata

#pragma once

#include <JuceHeader.h>

namespace timedata {

using namespace juce;

typedef void (*StringCaller)(std::string data);

void startJuceApplication(StringCaller);
void callTimedata(std::string const&);
void quitJuceApplication();

template <class Callback>
void runOnMessageThread(Callback cb, std::string message = "");

}  // namespace timedata

#pragma once

#include <JuceHeader.h>

namespace timedata {

using namespace juce;

typedef void (*StringCaller)(std::string data);
typedef void (*MethodCaller)(void* object);
typedef void (*StringMethodCaller)(void* object, std::string data);

void startJuceApplication(StringCaller);
void callTimedata(std::string const&);
void quitJuceApplication();

}  // namespace timedata

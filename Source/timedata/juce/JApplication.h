#pragma once

#include "timedata/util/AppCallback.h"

namespace timedata {

void startApplication(StringCaller, void*);
void callTimedata(string const&);
bool isStarted();

string timestamp();
string datestamp();

}  // namespace timedata

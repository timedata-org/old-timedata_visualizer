#include <timedata/juce/JApplication.h>

namespace timedata {

auto const LIBRARY_NAME = "timedata_visualizer";
auto const VERSION_NUMBER = "0.0";

struct VisualizerContext {
    StringCaller callback;
    void* userData;

    void call(char const* s) {
        callback(userData, s);
    }

    static VisualizerContext& global() {
        VisualizerContext context;
        return context;
    }
};

inline void quit() {
    runOnMessageThread(JUCEApplicationBase::quit);
}

class ApplicationBase : public juce::JUCEApplicationBase {
  public:
    String getApplicationName() override { return LIBRARY_NAME; }
    String getApplicationVersion() override { return VERSION_NUMBER; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override {
        VisualizerContext::global().call("{\"event\":\"start\"}");
    }

    void systemRequestedQuit() override {
        quit();
    }

    void shutdown() override {}
    void anotherInstanceStarted(const String&) override {}
    void suspended() override {}
    void resumed() override {}
    void unhandledException(const std::exception*, const String&, int) override
    {}
};

juce::JUCEApplicationBase* juce_CreateApplication() {
    return new ApplicationBase();
}

inline void startJuceApplication(StringCaller cb, void* userData) {
    VisualizerContext::global() = {cd, userData};
    juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
    static const char* argv[] = {LIBRARY_NAME};
    juce::JUCEApplicationBase::main(1, argv);
}

void callTimedata(std::string const& s) {
    VisualizerContext::global().call(s.c_str());
}

} // timedata

#include <timedata_visualizer/juce/JApplication.h>

namespace timedata {

struct VisualizerContext {
    StringCaller callback;
    void* userData;

    static VisualizerContext& global() {
        static VisualizerContext context;
        return context;
    }
};

inline void quit() {
    runOnMessageThread(JUCEApplicationBase::quit);
}

class ApplicationBase : public juce::JUCEApplicationBase {
  public:
    const String getApplicationName() override { return "timedata_visualizer"; }
    const String getApplicationVersion() override { return "0.0"; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override {
        VisualizerContext::global().callback("{\"event\":\"start\"}");
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

inline juce::JUCEApplicationBase* juce_CreateApplication() {
    return new ApplicationBase();
}

inline void startJuceApplication(StringCaller cb, void* userData) {
    VisualizerContext::global() = {cb, userData};
    juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
    static const char* argv[] = {"timedata_visualizer"};
    juce::JUCEApplicationBase::main(1, argv);
}

inline void callTimedata(std::string const& s) {
    VisualizerContext::global().callback(s.c_str());
}

} // timedata

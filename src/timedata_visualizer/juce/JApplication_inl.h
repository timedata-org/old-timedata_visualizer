#include <timedata_visualizer/juce/JApplication.h>

namespace timedata {


struct VisualizerContext {
    static StringCaller& global() {
        static StringCaller callback;
        return callback;
    }
};

inline void quitJuceApplication() {
    std::cerr << "Starting to quit\n";
    runOnMessageThread(JUCEApplicationBase::quit, "quit");
}

class ApplicationBase : public juce::JUCEApplicationBase {
  public:
    const String getApplicationName() override { return "timedata_visualizer"; }
    const String getApplicationVersion() override { return "0.0"; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override {
        VisualizerContext::global()("{\"event\":\"start\"}");
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
    std::cerr << "creating new juce application\n";
    return new ApplicationBase();
}

inline void startJuceApplication(StringCaller cb) {
    VisualizerContext::global() = cb;
    juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
    static const char* argv[] = {"timedata_visualizer"};
    juce::JUCEApplicationBase::main(1, argv);
    std::cerr << "done with main\n";
}

inline void callTimedata(std::string const& s) {
    VisualizerContext::global()(s.c_str());
}

} // timedata

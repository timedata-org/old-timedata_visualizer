#include <timedata_visualizer/juce/JApplication.h>

namespace timedata {

template <class Callback>
void runOnMessageThread(Callback cb, std::string message) {
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


inline static StringCaller& globalCallback() {
    static StringCaller callback;
    return callback;
}

inline void quitJuceApplication() {
    std::cerr << "quitJuceApplication\n";
    runOnMessageThread(JUCEApplicationBase::quit);
}

class ApplicationBase : public juce::JUCEApplicationBase {
  public:
    const String getApplicationName() override { return "timedata_visualizer"; }
    const String getApplicationVersion() override { return "0.0"; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override {
        globalCallback()("{\"event\":\"start\"}");
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

inline void startJuceApplication(StringCaller cb) {
    globalCallback() = cb;
    juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
    static const char* argv[] = {"timedata_visualizer"};
    juce::JUCEApplicationBase::main(1, argv);
}

inline void callTimedata(std::string const& s) {
    globalCallback()(s.c_str());
}

inline void receiveMessageToJuce(std::string const& s) {
    if (s == "quit") {
        std::cerr << "quit message\n";
        quitJuceApplication();
    }
}

} // timedata

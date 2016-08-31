#include <timedata/util/TimedataApplication.h>
#include "timedata/util/RunOnMessageThread.h"

namespace timedata {

juce::JUCEApplicationBase* juce_CreateApplication();

auto const LIBRARY_NAME = "ctimedata";
auto const VERSION_NUMBER = "0.0";

class VisualizerContext {
  public:
    bool isStarted() const {
        ScopedLock l(lock_);
        return started_;
    }

    void start(StringCaller cb, void* userData) {
        {
            ScopedLock l(lock);
            if (started)
                return;
            started = true;
        }

        callback_ = cb;
        userData_ = userData;
        juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
        static const char* argv[] = {LIBRARY_NAME};
        juce::JUCEApplicationBase::main(1, argv);
    }

    void call(std::string const& s) {
        if (isStarted())
            callback_(userData_, s.c_str());
    }

    static VisualizerContext& global() {
        VisualizerContext context;
        return context;
    }

  private:
    VisualizerContext() = default;

    StringCaller callback_;
    void* userData_;
    CriticalSection lock_;
    bool started_ = false;
};

class ApplicationBase : public juce::JUCEApplicationBase {
  public:
    String getApplicationName() override { return LIBRARY_NAME; }
    String getApplicationVersion() override { return VERSION_NUMBER; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const String&) override {
        // TODO: is this relevant any more?
        VisualizerContext::global().call("{\"event\":\"start\"}");
    }

    void systemRequestedQuit() override {
        runOnMessageThread(JUCEApplicationBase::quit);
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

} // timedata

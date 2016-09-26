/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include <random>

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include <timedata_visualizer/component/LightWindow_inl.h>

using namespace timedata;

//==============================================================================
class timedata_visualizer_appApplication  : public JUCEApplication
{
public:
    //==============================================================================
    timedata_visualizer_appApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
#ifndef USE_DEMO_COMPONENT
        LightComponent component_;
#endif
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setResizable(true, true);
            addContent();
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void addContent() {
#ifdef USE_DEMO_COMPONENT
            setContentOwned (new MainContentComponent(), true);
#else
            setContentNonOwned (&component_, false);
            size_t width = 2, height = 2;
            component_.setLights(width, height);
            auto bp = component_.bufferPointer();
            std::default_random_engine generator;
            std::uniform_int_distribution<uint8_t> random;
            for (size_t i = 0; i < 3 * width * height; ++i) {
                auto rg = random(generator);
                std::cerr << "random " << (int) rg << '\n';
                bp[i] = rg;
            }
#endif
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (timedata_visualizer_appApplication)

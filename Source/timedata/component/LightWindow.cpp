#include "timedata/color/FColorList.h"
#include "timedata/component/LightingWindow.h"
#include "timedata/util/TimedataApplication.h"
#include "timedata/util/RunOnMessageThread.h"

namespace timedata {

struct LightWindow::Impl : DocumentWindow {

    Impl() : DocumentWindow("timedata lighting simulator",
                            Colours::lightgrey,
                            DocumentWindow::allButtons),
             instrumentGrid_(new timedata::InstrumentGrid) {
        setContentOwned(instrumentGrid_, true);
        centreWithSize(getWidth(), getHeight());
        setUsingNativeTitleBar(true);
    }

    ~Impl() {}

    void saveSnapshotToFile(std::string const& name) {
        File file(name);
        if (auto format = ImageFileFormat::findImageFormatForFileExtension(file)) {
            auto image = createComponentSnapshot(getLocalBounds());
            FileOutputStream stream(file);
            if (not format->writeImageToStream(image, stream))
                LOG(DFATAL) << "Unable to write to filename " << name;
        } else {
            LOG(DFATAL) << "Don't understand filename " << name;
        }
    }

    void setLights(Segment<ColorRGB> colors) {
        // runOnMessageThread([=] () { instrumentGrid_->setLights(colors); });
    }

    LightingWindow* makeLightingWindow() {
        MessageManagerLock l;

        auto window = make_unique<LightingWindow>();
        window->toFront(true);
        window->setVisible(true);
        window->grid()->setLightCount(256);

        return window.release();
    }

    static string stringBounds(LightingWindow const& window) {
        auto bounds = window.getScreenBounds();
        return "{\"x\":" + std::to_string(bounds.getX()) +
                ",\"y\":" + std::to_string(bounds.getY()) +
                ",\"width\":" + std::to_string(bounds.getWidth()) +
                ",\"height\":" + std::to_string(bounds.getHeight()) +
                "}";
    }

    void closeButtonPressed() {
        callTimedata("{\"event\":\"closeButtonPressed\"}");
    }

    void moved() {
        callTimedata("{\"event\":\"moved\",\"bounds\":" +
                     stringBounds(*this) + "}");
    }

    void resized() {
        callTimedata("{\"event\":\"resized\",\"bounds\":" +
                     stringBounds(*this) + "}");
    }

    static void deleteWindow(LightingWindow* window) {
        delete window;
    }

    void deleteLightingWindow(LightingWindow* window) {
        runOnMessageThread([=]() { delete window; });
    }
};

LightWindow::LightWindow() : impl_(std::make_unique<LightWindow>()) {
}

LightWindow::~LightWindow() = default;



void LightWindow::setLights(Segment<ColorRGB> segment) {
    impl->setLights(segment);
}

void LightWindow::saveSnapshotToFile(std::string const& filename) {
    impl->saveSnapshotToFile(filename);
}


} // timedata

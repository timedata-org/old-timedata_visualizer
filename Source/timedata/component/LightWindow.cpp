#include <timedata/util/RunOnMessageThread.h>
#include <timedata/base/doubleBuffer.h>

namespace timedata {


template <typename Color>
inline Colour toColour(Color c) {
    return {c[0], c[1], c[2]};
}

struct LightWindow::Impl : DocumentWindow {
    using ColourList = std::vector<Colour>;
    using ColourBuffer = DoubleBuffer<ColourList>;
    using Desc = LightWindow::Desc;

    Desc desc;
    ColorBuffer colorBuffer;

    Impl(Desc d) : DocumentWindow("timedata lighting simulator",
                                  Colours::lightgrey,
                                  DocumentWindow::allButtons) {
        setDesc(d);
        // setContentOwned(instrumentGrid_, true);
        // centreWithSize(getWidth(), getHeight());
        setUsingNativeTitleBar(true);
        toFront(true);
        setVisible(true);
    }

    ~Impl() {}

    void paint(Graphics& g) {
        auto& colors = colorBuffer.out();
        auto bounds = getLocalBounds().reduced(desc.padding).toFloat();
        g.fillAll(desc.background);

        for (auto x = 0; i < desc.
    }

    void setDesc(Desc d) {
        desc_ = d;
    }

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

    void setLights(ColorSegment colors) {
        auto& in = colorBuffer.in();
        if (in.size() < colors.size)
            in.resize(colors.size());

        for (size_t i = 0; i < colors.size; ++i)
            in[i] = toColour(colors[i]);
        colorBuffer.setDirty();
        auto impl = this;
        runOnMessageThread([=] () { impl->repaint(); });
    }
};

LightWindow::LightWindow() {
    runOnMessageThread([this]() { construct(); });
}

LightWindow::~LightWindow() {
    std::shared_ptr<Impl> sp = std::move(impl_);
    auto reset = [sp]() mutable {
        sp.reset();
    };
    sp.reset();

    // http://melpon.org/wandbox/permlink/8CDdVZzZRFWa5ZgM
    // Now the lambda is guaranteed to have the last reference to sp.
    // In C++14, I could use move semantics for the capture and avoid all this.
    runOnMessageThread(reset);
}

void LightWindow::construct() {
    auto impl = std::make_unique<Impl>();
    locker_([&]() {
        impl_ = std::move(impl);
    });
}

void LightWindow::setLights(ColorSegment colors) {
    impl_->setLights(colors);
}

void LightWindow::saveSnapshotToFile(std::string const& filename) {
    impl_->saveSnapshotToFile(filename);
}

bool LightWindow::isConstructed() const {
    auto l = locker_();
    return impl_;
}

} // timedata

#include <timedata_visualizer/component/LightWindow.h>
#include <timedata_visualizer/base/make_unique.h>
#include <timedata_visualizer/base/string.h>
#include <timedata_visualizer/component/snapshot.h>

namespace timedata {

class LightComponent : public Component {
  public:
    // The MessageManagerQueue must be locked for all methods.
    LightComponent() = default;
    ~LightComponent() = default;

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void paint(Graphics& g) override;
    BufferPointer bufferPointer() { return bufferPointer_; }

  private:
    size_t width_, height_;
    LightWindowDesc desc_;
    BufferPointer bufferPointer_;
    std::vector<uint8_t> buffer_;

    Mutex mutex_;
};

inline void LightComponent::paint(Graphics& g) {
    auto bounds = getLocalBounds().reduced(desc_.windowPadding).toFloat();
    auto wtile = bounds.getWidth() / width_;
    auto htile = bounds.getHeight() / height_;
    auto tile = Rectangle<float>(wtile, htile);
    auto instrument = tile.reduced(desc_.instrumentPadding);
    auto relative = instrument;
    auto bp = bufferPointer_;

    g.fillAll(Colours::black);

    for (size_t i = 0, cp = 0; i < height_; ++i) {
        relative.setY(instrument.getY() + i * tile.getHeight());
        for (size_t j = 0; j < width_; ++j) {
            auto red = bp[cp++];
            auto green = bp[cp++];
            auto blue = bp[cp++];
            auto colour = Colour(red, green, blue);
            g.setColour(colour);
            relative.setX(instrument.getX() + j * tile.getWidth());

            if (desc_.shape == LightWindowDesc::Shape::rect)
                g.fillRect(relative);
            else
                g.fillEllipse(relative);

            if (desc_.label != LightWindowDesc::Label::none) {
                g.setColour(colour.contrasting());
                auto n = cp + 1;
                auto isNumber = (desc_.label == LightWindowDesc::Label::number);
                auto text = isNumber ? std::to_string(n) : toCharString(n);
                relative.reduce(desc_.labelPadding, desc_.labelPadding);
                g.drawFittedText(text, relative.toNearestInt(),
                                 Justification::centred, 1);
            }
        }
    }
}

inline void LightComponent::setDesc(LightWindowDesc d) {
    desc_ = d;
    repaint();
}

inline void LightComponent::setLights(size_t width, size_t height,
                                      BufferPointer bp) {
    width_ = width;
    height_ = height;

    if (bp) {
        bufferPointer_ = bp;
    } else {
        auto size = 3 * width * height;
        if (buffer_.size() < size)
            buffer_.resize(size);
        bufferPointer_ = &buffer_.front();
    }
    repaint();
}

struct LightWindow::Impl : DocumentWindow {
    LightComponent comp;

    Impl() : DocumentWindow("(not set)", Colours::black, allButtons) {
        setContentNonOwned(&comp, false);
        setUsingNativeTitleBar(true);
        toFront(true);
        setVisible(true);
    }

    void closeButtonPressed() override {
        // TODO - send a message.
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
};


inline LightWindow::LightWindow() {
    MessageManagerLock mml;
    impl_ = std::make_unique<Impl>();
}

inline LightWindow::~LightWindow() {
    MessageManagerLock mml;
    impl_.reset();
}

inline void LightWindow::setDesc(LightWindowDesc desc) {
    MessageManagerLock mml;
    impl_->setName(desc.name);
    impl_->comp.setDesc(desc);
}

inline void LightWindow::setLights(
        size_t width, size_t height, BufferPointer bp) {
    MessageManagerLock mml;
    impl_->comp.setLights(width, height, bp);
}

void LightWindow::writeSnapshotToFile(std::string const& filename) {
    timedata::writeSnapshotToFile(filename, impl_->comp);
}

BufferPointer LightWindow::bufferPointer() {
    MessageManagerLock mml;
    return impl_->comp.bufferPointer();
}

} // timedata

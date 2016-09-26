#include <timedata_visualizer/component/LightWindow.h>
#include <timedata_visualizer/base/make_unique.h>

namespace timedata {

inline UnlockedLightWindow::UnlockedLightWindow()
        : DocumentWindow("(not set)", Colours::black, allButtons) {
    setUsingNativeTitleBar(true);
    toFront(true);
    setVisible(true);
}

inline std::string toCharString(size_t n) {
    std::string result;
    while (n) {
        result += ('a' + n % 26);
        n /= 26;
    }

    if (result.empty())
        result += 'a';
    else
        std::reverse(result.begin(), result.end());
    return result;
}

inline void UnlockedLightWindow::paint(Graphics& g) {
    auto bounds = getLocalBounds().reduced(desc_.windowPadding).toFloat();
    auto wtile = bounds.getWidth() / width_;
    auto htile = bounds.getHeight() / height_;
    auto tile = Rectangle<float>(wtile, htile);
    auto instrument = tile.reduced(desc_.instrumentPadding);
    auto relative = instrument;
    auto bp = bufferPointer_;

    g.fillAll(Colours::black);

    for (size_t i = 0, cp = 0; i < height_; ++i) {
        relative.setY(instrument.getY() + i * tile.getY());
        for (size_t j = 0; j < width_; ++j) {
            auto red = bp[cp++];
            auto green = bp[cp++];
            auto blue = bp[cp++];
            auto colour = Colour(red, green, blue);
            g.setColour(colour);
            relative.setX(instrument.getX() + j * tile.getX());

            if (desc_.shape == LightWindowDesc::Shape::rect)
                g.fillRect(relative);
            else
                g.fillEllipse(relative);

            if (desc_.label != LightWindowDesc::Label::none) {
                g.setColour(colour.contrasting());
                auto n = i * width_ + j + 1;
                auto isNumber = (desc_.label == LightWindowDesc::Label::number);
                auto text = isNumber ? std::to_string(n) : toCharString(n);
                relative.reduce(desc_.labelPadding, desc_.labelPadding);
                g.drawFittedText(text, relative.toNearestInt(),
                                 Justification::centred, 1);
            }
        }
    }
}

inline void UnlockedLightWindow::setDesc(LightWindowDesc d) {
    desc_ = d;
    setName(desc_.name);
    repaint();
}

inline void UnlockedLightWindow::setLights(size_t width, size_t height,
                                           BufferPointer bp) {
    width_ = width;
    height_ = height;

    if (bp) {
        bufferPointer_ = bp;
    } else {
        auto size = width * height;
        if (buffer_.size() < size)
            buffer_.resize(size);
        bufferPointer_ = &buffer_.front();
    }
    repaint();
}

inline LightWindow::LightWindow() {
    MessageManagerLock mml;
    impl_ = std::make_unique<UnlockedLightWindow>();
}

inline LightWindow::~LightWindow() {
    MessageManagerLock mml;
    impl_.reset();
}

inline void LightWindow::setDesc(LightWindowDesc desc) {
    MessageManagerLock mml;
    impl_->setDesc(desc);
}

inline void LightWindow::setLights(
        size_t width, size_t height, BufferPointer bp) {
    MessageManagerLock mml;
    impl_->setLights(width, height, bp);
}

inline void LightWindow::saveSnapshotToFile(std::string const& name) {
    File file(name);
    if (auto format = ImageFileFormat::findImageFormatForFileExtension(file)) {
        FileOutputStream stream(file);
        auto getImage = [&]() {
            MessageManagerLock mml;
            return impl_->createComponentSnapshot(impl_->getLocalBounds());
        };
        if (not format->writeImageToStream(getImage(), stream))
            std::cerr << "Unable to write to filename " << name;
    } else {
        std::cerr << "Don't understand filename " << name;
    }
}

} // timedata

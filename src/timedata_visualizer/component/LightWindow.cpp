#include <string>

namespace timedata {

template <typename Color>
inline Colour toColour(Color c) {
    return {c[0], c[1], c[2]};
}

inline LightWindow::LightWindow() {
    // setContentOwned(instrumentGrid_, true);
    // centreWithSize(getWidth(), getHeight());
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

inline void LightWindow::paint(Graphics& g) {
    auto bounds = getLocalBounds().reduced(desc_.windowPadding).toFloat();
    auto tile = Rectangle<float>(bounds.w / width_;, bounds.h / height_);
    auto instrument = instrumentRect.reduced(desc_.instrumentPadding);
    auto relative = instrument;
    auto bp = bufferPointer();

    g.fillAll(Colours.black);

    for (size_t i = 0, cp = 0; i < height_; ++i) {
        relative.setY(instrument.getY() + i * tile.getY());
        for (size_t j = 0; j < width; ++j) {
            auto r = bp[cp++];
            auto g = bp[cp++];
            auto b = bp[cp++];
            auto colour = Colour(r, g, b);
            g.setColour(colour);
            relative.setX(instrument.getX() + j * tile.getX());

            if (desc_.shape == LightWindow::Shape::rect)
                g.fillRect(relative);
            else
                g.fillEllipse(relative);

            if (desc_.label != LightWindow::Label::none) {
                g.setColour(color.contrasting());
                auto n = i * width_ + j + 1;
                auto isNumber = (desc_.label == LightWindow::Label::number);
                auto text = isNumber ? std::to_string(n) : toCharString(n);
                relative.reduce(desc_.labelPadding);
                g.drawFittedText(text, relative, Justification::centred, 1);
            }
        }
    }
}

void LightWindow::setDesc(LightWindowDesc d) {
    desc_ = d;
    repaint();
}

void LightWindow::saveSnapshotToFile(std::string const& name) {
    File file(name);
    if (auto format = ImageFileFormat::findImageFormatForFileExtension(file)) {
        auto image = createComponentSnapshot(getLocalBounds());
        FileOutputStream stream(file);
        if (not format->writeImageToStream(image, stream))
            std::cerr << "Unable to write to filename " << name;
    } else {
        std::cerr << "Don't understand filename " << name;
    }
}

void LightWindow::setLights(size_t width, size_t height, BufferPointer* bp) {
    width_ = width;
    height_ = height;

    if (bp) {
        bufferPointer_ = bp;
    } else {
        auto size = width * height;
        if (buffer_.size() < size())
            buffer_.resize(size);
        bufferPointer_ = &buffer.front();
    }
    repaint();
}

ColorSegment colors) {
    auto& in = colorBuffer.in();
    if (in.size() < colors.size)
        in.resize(colors.size());

    for (size_t i = 0; i < colors.size; ++i)
        in[i] = toColour(colors[i]);
    colorBuffer.setDirty();
    auto impl = this;
    runOnMessageThread([=] () { impl->repaint(); });
}

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

} // timedata

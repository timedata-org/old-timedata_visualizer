#include <timedata_visualizer/component/LightComponent.h>
#include <timedata_visualizer/base/string.h>

namespace timedata {

static const auto PRINT_FRAMERATE_EVERY = 1000;

inline void LightComponent::paint(Graphics& g) {
    g.fillAll(Colours::black);

    if (not (width_ and height_ and bufferPointer_))
        return;

    auto bounds = getLocalBounds().reduced(desc_.windowPadding).toFloat();
    auto wtile = bounds.getWidth() / width_;
    auto htile = bounds.getHeight() / height_;
    auto tile = Rectangle<float>(wtile, htile);
    auto instrument = tile.reduced(desc_.instrumentPadding);
    instrument.setPosition(bounds.getPosition());
    auto relative = instrument;
    auto bp = bufferPointer_;

    for (size_t i = 0, cp = 0; i < height_; ++i) {
        relative.setY(instrument.getY() + i * tile.getHeight());
        for (size_t j = 0; j < width_; ++j) {
            auto colour = Colour(bp[cp], bp[cp + 1], bp[cp + 2]);
            cp += 3;
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

    auto now = Time::getCurrentTime();
    if (not frameCount_) {
        startTime_ = now;
    } else if (auto rate = desc_.frameRatePrintRate) {
        if (frameCount_ % rate) {
            auto elapsed = (now - startTime_).inSeconds();
            std::cout << "Frame rate: "
                      << rate << ' '
                      << frameCount_ / elapsed << '\n';
        }
    }
    frameCount_++;
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

} // timedata

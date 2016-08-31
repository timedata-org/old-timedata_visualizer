#include "timedata/component/InstrumentComponent.h"

namespace timedata {

InstrumentComponent::InstrumentComponent()
        : color_(Colours::black), labelColor_(Colours::white), isRect_(false) {
}

void InstrumentComponent::setColor(const Colour& c) {
    ScopedLock l(lock_);
    if (c == color_)
        return;

    color_ = c;
    static const auto GREY = (3 * 0xFF) / 2;
    auto r = color_.getRed();
    auto g = color_.getGreen();
    auto b = color_.getBlue();
    labelColor_ = (r + g + b >= GREY) ? Colours::black : Colours::white;
}

void InstrumentComponent::paint(Graphics& g) {
    ScopedLock l(lock_);
    Colour back, label;
    {
        back = color_;
        label = labelColor_;
    }

    g.setColour(back);
    Rectangle<int> b = getLocalBounds();

    if (isRect_)
        g.fillRect(b);
    else
        g.fillEllipse(b.getX(), b.getY(), b.getWidth(), b.getHeight());

    if (showLabel_) {
        b.reduce(labelPaddingX_ / 2, labelPaddingY_ / 2);
        g.setColour(label);
        g.drawFittedText(label_, b, Justification::centred, 1);
    }
}

void InstrumentComponent::setShape(bool isRect) {
    ScopedLock l(lock_);
    isRect_ = isRect;
}

void InstrumentComponent::setLabelPadding(int x, int y) {
    ScopedLock l(lock_);
    labelPaddingX_ = x;
    labelPaddingY_ = y;
}

void InstrumentComponent::setShowLabel(bool show) {
    ScopedLock l(lock_);
    showLabel_ = show;
}

void InstrumentComponent::setLabel(const String& label) {
    ScopedLock l(lock_);
    label_ = label;
}

}  // namespace timedata

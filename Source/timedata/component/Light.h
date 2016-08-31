#pragma once

#include <string>

namespace timedata {
namespace visualizer {

using namespace juce;

struct LabelDesc {
    bool isShowing;
    std::string text;
    Point<int> padding;
};

struct LightDesc {
    enum class Type {rect, ellipse};
    Type type;
    LabelDesc label;
};

class Light : public Component {
  public:
    Light(LightDesc desc = {}) : desc_(desc) {}
    ~Light() = default;

    void setLight(LightDesc&& d) {
        ScopedLock l(lock_);
        desc_ = std::move(d);
    }

    void setColor(const Colour& c) {
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

    void paint(Graphics& g) override {
        Colour color, labelColor;
        LightDesc desc;
        {
            ScopedLock l(lock_);
            color = color_;
            label = labelColor_;
            desc = desc_;
        }

        g.setColour(color);
        Rectangle<int> b = getLocalBounds();

        if (isRect_)
            g.fillRect(b);
        else
            g.fillEllipse(b.getX(), b.getY(), b.getWidth(), b.getHeight());

        if (showLabel_) {
            b.reduce(desc.label.padding.x / 2, desc.label.padding.y / 2);
            g.setColour(labelColor);
            g.drawFittedText(desc.label.text, b, Justification::centred, 1);
        }
    }

    void setShape(bool isRect) {
        ScopedLock l(lock_);
        isRect_ = isRect;
    }

    void setLabelPadding(int x, int y) {
        ScopedLock l(lock_);
        desc.label.padding.x = x;
        labelPaddingY_ = y;
    }

    void setShowLabel(bool show) {
        ScopedLock l(lock_);
        showLabel_ = show;
    }

    void setLabel(const String& label) {
        ScopedLock l(lock_);
        label_ = label;
    }
  private:
    String label_;
    Colour color_;
    Colour labelColor_;
    CriticalSection lock_;

    bool isRect_;
    int desc.label.padding.x;
    int labelPaddingY_;
    bool showLabel_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Light)
};

} // visualizer
} // timedata

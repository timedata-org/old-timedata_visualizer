#include "rec/util/STL.h"
#include "timedata/color/FColorList.h"
#include "timedata/color/RGB.h"
#include "timedata/component/InstrumentGrid.h"

namespace timedata {

InstrumentGrid::InstrumentGrid()
        : isUnclipped_(false),
          labelStartsAtZero_(false),
          showLabel_(false),
          background_(Colours::black),
          layout_(32, 32),
          size_(12, 12),
          padding_(4, 4),
          instrumentPadding_(2, 2),
          labelPadding_(2, 2) {
    setSize(64, 64);
    MessageManagerLock l;
    layout();
}

InstrumentGrid::~InstrumentGrid() {}

void InstrumentGrid::setLayout(
    const Point& lay, const Point& size, const Point& padding,
    const Point& instrumentPadding, const Point& labelPadding) {
    MessageManagerLock l;

    padding_ = padding;
    size_ = size;
    layout_ = lay;
    instrumentPadding_ = instrumentPadding;
    labelPadding_ = labelPadding;
    layout();
}

void InstrumentGrid::setShape(bool isRect) {
    MessageManagerLock l;
    for (auto& i: instruments_)
        i->setShape(isRect);
}

void InstrumentGrid::layout() {
    auto left = padding_.first;
    auto top = padding_.second;
    auto columns = layout_.first;
    auto rows = layout_.second;

    auto w = size_.first + instrumentPadding_.first;
    auto h = size_.second + instrumentPadding_.second;
    auto i = 0;
    for (auto y = 0; y < rows and i < instruments_.size(); ++y) {
        for (auto x = 0; x < columns and i < instruments_.size(); ++x) {
            auto& instr = instruments_[i++];
            instr->setLabelPadding(labelPadding_.first, labelPadding_.second);
            instr->setBounds(left + x * w, top + y * h, size_.first, size_.second);
            instr->setShowLabel(showLabel_);
        }
    }

    auto screenWidth = left + w * columns + padding_.first,
            screenHeight = top + h * rows + padding_.second;

    setSize(screenWidth, screenHeight);
}

void InstrumentGrid::paint(Graphics& g) {
    g.fillAll(background_);
}

void InstrumentGrid::setPaintingIsUnclipped(bool isUnclipped) {
    MessageManagerLock l;
    if (isUnclipped != isUnclipped_) {
        isUnclipped_ = isUnclipped;
        for (auto& i: instruments_)
            i->setPaintingIsUnclipped(isUnclipped);
    }
}

void InstrumentGrid::setLabelStartsAtZero(bool startsAtZero) {
    MessageManagerLock l;
    if (startsAtZero != labelStartsAtZero_) {
        labelStartsAtZero_ = startsAtZero;
        auto delta = labelStartsAtZero_ ? 0 : 1;
        for (auto i = 0; i < instruments_.size(); ++i)
            instruments_[i]->setLabel(String(i + delta));
    }
}

void InstrumentGrid::setLightCount(int count) {
    MessageManagerLock l;
    auto oldCount = instruments_.size();

    if (count == oldCount)
        return;

    cache_.resize(3 * count);

    auto delta = labelStartsAtZero_ ? 0 : 1;
    instruments_.resize(count);
    for (auto i = oldCount; i < count; ++i) {
        auto inst = make_unique<InstrumentComponent>();
        inst->setPaintingIsUnclipped(isUnclipped_);
        inst->setLabelPadding(labelPadding_.first, labelPadding_.second);
        inst->setLabel(String(uint32(i + delta)));
        addAndMakeVisible(inst.get());
        instruments_[i] = std::move(inst);
    }
    layout();
}

void InstrumentGrid::setBackground(const Colour& c) {
    MessageManagerLock l;
    background_ = c;
}

void InstrumentGrid::setShowLabel(bool showLabel) {
    MessageManagerLock l;
    if (showLabel != showLabel_) {
        showLabel_ = showLabel;
        for (auto& i: instruments_)
            i->setShowLabel(showLabel);
    }
}

int InstrumentGrid::getLightCount() const {
    MessageManagerLock l;
    return instruments_.size();
}

float InstrumentGrid::gamma() const {
    MessageManagerLock l;
    return gamma_;
}

void InstrumentGrid::setGamma(float gamma) {
    MessageManagerLock l;
    gamma_ = gamma;
    useGamma_ = not near(gamma_, 1.0f);
}

void InstrumentGrid::setLights(const color::FColorList& colors) {
    MessageManagerLock l;

    auto size = jmin(colors.size(), instruments_.size());
    for (auto i = 0; i < size; ++i) {
        auto& c = colors.get(i);
        uint32 argb;
        if (useGamma_) {
            auto c2 = c;
            c2.gamma(gamma_);
            argb = c2.argb();
        } else {
            argb = c.argb();
        }
        instruments_[i]->setColor(Colour(argb));
    }

    for (auto i = size; i < instruments_.size(); ++i)
        instruments_[i]->setColor(Colours::black);
    pythonRepaint();
}

void InstrumentGrid::pythonRepaint() {
    JUCE_AUTORELEASEPOOL {
        repaint();
    }
}

}  // namespace timedata

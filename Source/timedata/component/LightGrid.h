#pragma once

#include "timedata/base/Timedata.h"
#include "timedata/component/InstrumentComponent.h"

namespace timedata { namespace color { class FColorList; }}

namespace timedata {

struct LightConfig;

class InstrumentGrid : public Component {
  public:
    InstrumentGrid();
    virtual ~InstrumentGrid();

    void setLights(const color::FColorList&);
    void setPaintingIsUnclipped(bool);
    void setLightCount(int);
    int getLightCount() const;
    void setLabelStartsAtZero(bool);
    void setShape(bool isRect);
    void setShowLabel(bool);
    void setBackground(const Colour&);
    void setLayout(const Point& layout, const Point& size, const Point& padding,
                   const Point& instrumentPadding, const Point& labelPadding);
    float gamma() const;
    void setGamma(float gamma);

    void paint(Graphics&);
    void pythonRepaint();

  private:
    void layout();
    void doSetLights();

    vector<unique_ptr<InstrumentComponent>> instruments_;

    bool isUnclipped_;
    bool labelStartsAtZero_;
    bool showLabel_;
    Colour background_;
    float gamma_ = 1.0f;
    bool useGamma_ = false;

    Point layout_;
    Point size_;
    Point padding_;
    Point instrumentPadding_;
    Point labelPadding_;

    CriticalSection lock_;

    vector<unsigned char> cache_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InstrumentGrid)
};

}  // namespace timedata

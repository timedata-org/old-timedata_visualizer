#pragma once

#include <timedata/base/segment.h>
#include <timedata/signal/sample.h>

namespace timedata {

using ColorSegment = Segment<ColorRGB const>;

class LightWindow {
  public:
    struct Desc;

    LightWindow(Desc);
    ~LightWindow();

    void setLights(ColorSegment);

    /** Returns an error or an empty string if all went well. */
    std::string saveSnapshotToFile(std::string const& filename);
    bool isConstructed() const;

    /** Actually constructs the implementation, a JUCE component.
        I believe this must be called on the message thread. */
    void construct();

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    Locker<> locker_; // Used only for construction.
};


namespace fill {

struct Fill {
    bool vertical;
    bool bidirectional;
    bool xReversed;  // Regular is left-to-right, reversed right-to-left.
    bool yReversed;  // Regular is top to bottom,
};

struct Pair {
    size_t x, y;
};

struct LightWindow::Desc {
    enum class Shape { rect, circle };

    struct Label {
        enum class Type {none, letter, number};

        Type type;
        int padding;
        std::vector<std::string> custom;
    };

    int rows, columns;
    int padding, instrumentPadding;

    Attitude attitude;
    Colour background;
    Layout layout;
    Shape shape;

    Fill fill;
    Label label;
};

}  // namespace timedata

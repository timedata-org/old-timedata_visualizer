#pragma once

namespace timedata {

struct LightWindowDesc {
    enum class Shape { rect, ellipse };
    enum class Label { none, letter, number };

    Shape shape;
    Label label;

    int windowPadding;
    int instrumentPadding;
    int labelPadding;
};

}  // namespace timedata

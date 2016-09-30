#pragma once

namespace timedata {

struct LightWindowDesc {
    enum class Shape { rect, ellipse };
    enum class Label { none, letter, number };

    std::string name;
    Shape shape;
    Label label;

    int windowPadding;
    int instrumentPadding;
    int labelPadding;

    int x = 0, y = 0, width = 256, height = 192;
};

}  // namespace timedata

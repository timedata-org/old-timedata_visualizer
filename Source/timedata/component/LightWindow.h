#pragma once

#include <timedata/base/segment.h>
#include <timedata/signal/sample.h>

namespace timedata {

class LightWindow {
  public:
    LightWindow();
    ~LightWindow();

    void setLights(Segment<ColorRGB>);
    void saveSnapshotToFile(std::string const& filename);

  private:
    struct Impl;

    std::unique_ptr<Impl> impl;
};

}  // namespace timedata

#pragma once

#include <timedata_visualizer/base/lock.h>
#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

using BufferPointer = uint8_t*;

/** Encapsulates an LightComponent and properly locks the
    MessageManagerQueue. */
class LightWindow {
  public:
    LightWindow();
    ~LightWindow();

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void writeSnapshotToFile(std::string const& filename);
    BufferPointer bufferPointer();

  private:
    struct Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace timedata

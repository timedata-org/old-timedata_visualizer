#pragma once

#include <memory>
#include <string>
#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

using BufferPointer = uint8_t*;

/** Encapsulates an LightComponent and properly locks the
    MessageManagerQueue. */
class LightWindow {
  public:
    explicit LightWindow();
    ~LightWindow();

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void setLights(size_t width, size_t height, uint64_t bp);
    void writeSnapshotToFile(std::string const& filename);

    void repaint();
    BufferPointer bufferPointer();

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    int snapshotIndex_ = 0;
};

}  // namespace timedata

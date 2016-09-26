#pragma once

#include <timedata_visualizer/base/lock.h>
#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

class LightComponent : public Component {
  public:
    using BufferPointer = uint8_t*;

    // The MessageManagerQueue must be locked for all methods.
    LightComponent() = default;
    ~LightComponent() = default;

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void paint(Graphics& g) override;

  private:
    size_t width_, height_;
    LightWindowDesc desc_;
    BufferPointer bufferPointer_;
    std::vector<uint8_t> buffer_;

    Mutex mutex_;
};

/** Encapsulates an LightComponent and properly locks the
    MessageManagerQueue. */
class LightWindow {
  public:
    using BufferPointer = LightComponent::BufferPointer;

    LightWindow();
    ~LightWindow();

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void writeSnapshotToFile(std::string const& filename);

  private:
    struct Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace timedata

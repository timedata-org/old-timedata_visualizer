#pragma once

#include <mutex>

#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

class LightWindow : DocumentWindow {
  public:
    using BufferPointer = uint8_t*;

    LightWindow();
    ~LightWindow() = default;

    // All methods must be called on the message manager thread.
    void paint(Graphics& g) override;
    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer* p = nullptr);

    BufferPointer bufferPointer() { return bufferPointer_; }

    /** Returns an error, otherwise an empty string if all went well. */
    std::string saveSnapshotToFile(std::string const& filename);

  private:
    size_t width_, height_;
    LightWindowDesc desc;
    BufferPointer bufferPointer_;
    std::vector<uint8_t> buffer_;

    using Mutex = std::mutex;
    using Lock = std::unique_lock<Mutex>;

    Mutex mutex_;
};

}  // namespace timedata

#pragma once

#include <mutex>

#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

class UnlockedLightWindow : public DocumentWindow {
  public:
    using BufferPointer = uint8_t*;

    // The MessageManagerQueue must be locked for these next four methods.
    UnlockedLightWindow();
    ~UnlockedLightWindow() = default;

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);

    /** Called only by JUCE. */
    void paint(Graphics& g) override;

  private:
    size_t width_, height_;
    LightWindowDesc desc_;
    BufferPointer bufferPointer_;
    std::vector<uint8_t> buffer_;

    using Mutex = std::mutex;
    using Lock = std::unique_lock<Mutex>;

    Mutex mutex_;
};

/** Encapsulates an UnlockedLightWindow and properly locks the
    MessageManagerQueue. */
class LightWindow {
  public:
    using BufferPointer = UnlockedLightWindow::BufferPointer;

    LightWindow();
    ~LightWindow();

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);

    void saveSnapshotToFile(std::string const& filename);

  private:
    std::unique_ptr<UnlockedLightWindow> impl_;
};

}  // namespace timedata

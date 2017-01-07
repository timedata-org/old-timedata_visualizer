#include <JuceHeader.h>
#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

class LightComponent final : public Component {
  public:
    // The MessageManagerQueue must be locked for all methods.
    LightComponent() = default;
    ~LightComponent() = default;

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void paint(Graphics& g) override;
    BufferPointer bufferPointer() { return bufferPointer_; }

    LightWindowDesc getDesc() const { return desc_; }
    LightWindowDesc& getDesc() { return desc_; }

  private:
    size_t width_, height_;
    LightWindowDesc desc_ = {};
    BufferPointer bufferPointer_;
    std::vector<uint8_t> buffer_;
    Time startTime_;
    uint64_t frameCount_ = 0;

    Mutex mutex_;
};

} // timedata

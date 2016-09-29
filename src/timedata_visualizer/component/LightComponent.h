#include <JuceHeader.h>
#include <timedata_visualizer/component/LightWindowDesc.h>

namespace timedata {

class LightComponent : public Component {
  public:
    // The MessageManagerQueue must be locked for all methods.
    LightComponent() : desc_{0} {}
    ~LightComponent() = default;

    void setDesc(LightWindowDesc);
    void setLights(size_t width, size_t height, BufferPointer p = nullptr);
    void paint(Graphics& g) override;
    BufferPointer bufferPointer() { return bufferPointer_; }

  private:
    size_t width_, height_;
    LightWindowDesc desc_;
    BufferPointer bufferPointer_;
    std::vector<uint8_t> buffer_;

    Mutex mutex_;
};

} // timedata

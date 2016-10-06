#include <timedata_visualizer/component/LightWindow.h>
#include <timedata_visualizer/base/lock.h>
#include <timedata_visualizer/base/make_unique.h>
#include <timedata_visualizer/component/LightComponent_inl.h>
#include <timedata_visualizer/component/snapshot.h>

namespace timedata {

struct LightWindow::Impl final : DocumentWindow {
    LightComponent comp;

    Impl() : DocumentWindow("(not set)", Colours::black, allButtons) {
        setContentNonOwned(&comp, false);
        setUsingNativeTitleBar(true);
        toFront(true);
        setVisible(true);
        setSize(128, 96);
        setResizable(true, false);
        setResizeLimits(32, 8, 10240, 10240);
    }

    void setDesc(LightWindowDesc desc) {
        setName(desc.name);
        setBounds(desc.x, desc.y, desc.width, desc.height);
        comp.setDesc(desc);
    }

    void closeButtonPressed() override {
        // TODO - send a message.
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
};

inline LightWindow::LightWindow() {
    MessageManagerLock mml;
    impl_ = std::make_unique<Impl>();
}

inline LightWindow::~LightWindow() {
    reset();
}

inline void LightWindow::reset() {
    MessageManagerLock mml;
    impl_.reset();
}

inline void LightWindow::setDesc(LightWindowDesc desc) {
    MessageManagerLock mml;
    impl_->setDesc(desc);
}

inline void LightWindow::setLights(
        size_t width, size_t height, BufferPointer bp) {
    MessageManagerLock mml;
    impl_->comp.setLights(width, height, bp);
}

inline void LightWindow::setLights(size_t width, size_t height, uint64_t bp) {
    setLights(width, height, reinterpret_cast<BufferPointer>(bp));
}

void LightWindow::writeSnapshotToFile(std::string const& filename) {
    timedata::writeSnapshotToFile(filename, impl_->comp, ++snapshotIndex_);
}

BufferPointer LightWindow::bufferPointer() {
    MessageManagerLock mml;
    return impl_->comp.bufferPointer();
}

void LightWindow::repaint() {
    MessageManagerLock mml;
    impl_->repaint();
}

} // timedata

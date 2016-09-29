#include <timedata_visualizer/component/LightWindow.h>
#include <timedata_visualizer/base/lock.h>
#include <timedata_visualizer/base/make_unique.h>
#include <timedata_visualizer/component/LightComponent_inl.h>
#include <timedata_visualizer/component/snapshot.h>

namespace timedata {

struct LightWindow::Impl : DocumentWindow {
    LightComponent comp;

    Impl() : DocumentWindow("(not set)", Colours::black, allButtons) {
        setContentNonOwned(&comp, false);
        setUsingNativeTitleBar(true);
        toFront(true);
        setVisible(true);
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
    MessageManagerLock mml;
    impl_.reset();
}

inline void LightWindow::setDesc(LightWindowDesc desc) {
    MessageManagerLock mml;
    impl_->setName(desc.name);
    impl_->comp.setDesc(desc);
}

inline void LightWindow::setLights(
        size_t width, size_t height, BufferPointer bp) {
    MessageManagerLock mml;
    impl_->comp.setLights(width, height, bp);
}

inline void LightWindow::setLights(size_t width, size_t height, uint64_t size) {
    setLights(width, height, reinterpret_cast<BufferPointer>(size));
}

void LightWindow::writeSnapshotToFile(std::string const& filename) {
    timedata::writeSnapshotToFile(filename, impl_->comp);
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

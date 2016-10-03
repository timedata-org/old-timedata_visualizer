#pragma once

#include <JuceHeader.h>

namespace timedata {

inline Image getSnapshot(Component& comp) {
    MessageManagerLock mml;
    return comp.createComponentSnapshot(comp.getLocalBounds());
};

inline char const* writeImageToFile(File file, Image const& image) {
    if (file.getFileExtension() != ".png")
        return "Can only write .png files as snapshots";

    if (not file.getParentDirectory().createDirectory())
        return "Can't create directory";

    auto format = ImageFileFormat::findImageFormatForFileExtension(file);
    if (not format)
        return "Can't find ImageFileFormat";
    FileOutputStream stream(file);
    if (not format->writeImageToStream(image, stream))
        return "Unable to write";

    return nullptr;
}

inline File getSnapshotFile(std::string const& name, int index) {
    auto i = "0000" + std::to_string(index);
    if (i.size() > 5)
        i = i.substr(i.size() - 5);

    File file(name);
    auto parent = file.getParentDirectory();
    auto fname = file.getFileNameWithoutExtension();
    auto ext = file.getFileExtension();
    return parent.getChildFile(fname + "-" + i + ext);
}

inline void writeSnapshotToFile(std::string const& name, Component& c, int i) {
    auto file = getSnapshotFile(name, i);
    auto fname = file.getFileName();
    if (auto err = writeImageToFile(file, getSnapshot(c))) {
        std::cerr << err << " in file " << fname << '\n';
        file.deleteFile();
    } else {
        std::cout << "Wrote snapshot to " << fname << '\n';
    }
}

} // timedata

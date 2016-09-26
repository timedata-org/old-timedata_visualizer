#pragma once

#include <JuceHeader.h>

namespace timedata {

inline Image getSnapshot(Component& comp) {
    MessageManagerLock mml;
    return comp.createComponentSnapshot(comp.getLocalBounds());
};

inline void writeImageToFile(std::string const& name, Image const& image) {
    File file(name);
    if (auto format = ImageFileFormat::findImageFormatForFileExtension(file)) {
        FileOutputStream stream(file);
        if (not format->writeImageToStream(image, stream))
            std::cerr << "Unable to write to filename " << name;
    } else {
        std::cerr << "Don't understand file format for " << name;
    }
}

inline void writeSnapshotToFile(std::string const& name, Component& c) {
    writeImageToFile(name, getSnapshot(c));
}

} // timedata

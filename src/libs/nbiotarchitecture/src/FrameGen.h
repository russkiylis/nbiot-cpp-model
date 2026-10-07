#pragma once
#include "ResourceGrid.h"
#include "SubframeGen.h"

namespace nbiot {

class FrameGenerator {
public:
    FrameGenerator();
    
    // Генерирует заданное количество фреймов
    FramesGrid generateFrames(size_t numFrames) const;

private:
    SubframeGenerator subframeGenerator_;
};

} // namespace nbiot
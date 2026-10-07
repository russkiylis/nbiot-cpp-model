#include "FrameGen.h"

namespace nbiot {

FrameGenerator::FrameGenerator() = default;

FramesGrid FrameGenerator::generateFrames(size_t numFrames) const {
    FramesGrid frames(numFrames);
    
    for (size_t f = 0; f < numFrames; ++f) {
        for (size_t i = 0; i < NbIotConfig::NUM_SUBFRAMES_PER_FRAME; ++i) {
            frames[f][i] = subframeGenerator_.generateSubframe(i);
        }
    }
    
    return frames;
}

} // namespace nbiot
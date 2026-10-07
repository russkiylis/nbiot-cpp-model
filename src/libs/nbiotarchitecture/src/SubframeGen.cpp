#include "SubframeGen.h"

namespace nbiot {

SubframeGenerator::SubframeGenerator() = default;

SubframeGrid SubframeGenerator::generateSubframe(size_t subframeIndex) const {
    SubframeGrid grid = {}; // Инициализация нулями (комплексные нули)

    // NPSS передается только в 5-м сабфрейме каждого радиокадра
    if (subframeIndex == 5) {
        const auto& npssSeq = npssGenerator_.getNpssSequence();
        
        size_t seqIdx = 0;
        // Маппинг на последние 11 символов и первые 11 поднесущих
        for (size_t l = NbIotConfig::NPSS_START_SYMBOL; 
             l < NbIotConfig::NPSS_START_SYMBOL + NbIotConfig::NPSS_NUM_SYMBOLS; ++l) {
            for (size_t k = NbIotConfig::NPSS_START_SUBCARRIER; 
                 k < NbIotConfig::NPSS_START_SUBCARRIER + NbIotConfig::NPSS_NUM_SUBCARRIERS; ++k) {
                grid[k][l] = npssSeq[seqIdx++];
            }
        }
    }

    return grid;
}

} // namespace nbiot
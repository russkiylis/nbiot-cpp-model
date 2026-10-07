#pragma once
#include <array>
#include <complex>
#include <vector>
#include "Config.h"

namespace nbiot {

// Ресурсная сетка одного сабфрейма: [поднесущая][OFDM-символ]
using SubframeGrid = std::array<
    std::array<std::complex<float>, NbIotConfig::NUM_SYMBOLS_PER_SUBFRAME>, 
    NbIotConfig::NUM_SUBCARRIERS
>;

// Ресурсная сетка одного фрейма: массив из 10 сабфреймов
using FrameGrid = std::array<SubframeGrid, NbIotConfig::NUM_SUBFRAMES_PER_FRAME>;

// Массив фреймов
using FramesGrid = std::vector<FrameGrid>;

} // namespace nbiot
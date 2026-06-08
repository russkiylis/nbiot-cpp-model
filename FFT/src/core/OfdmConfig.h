#pragma once

#include "Modulation.h"

// Единый источник параметров для передатчика и приёмника.
// GraphDataProvider и OfdmDebug используют эти же значения.
constexpr int              kBitCount        = 512;
constexpr int              kFftSize         = 32;
constexpr int              kCpLength        = 8;
constexpr int              kStartOffset     = kCpLength;
constexpr int              kWindowSize      = kFftSize;
constexpr int              kStepSize        = kFftSize + kCpLength;
constexpr int              kSubcarrierCount = 8;
constexpr ModulationType   kModulationType  = ModulationType::Bpsk;

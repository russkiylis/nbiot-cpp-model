#pragma once
#include <cstddef>

namespace nbiot {

struct NbIotConfig {
    // Параметры ресурсной сетки (Downlink)
    static constexpr size_t NUM_SUBCARRIERS = 12;          // 12 поднесущих (180 кГц)
    static constexpr size_t NUM_SYMBOLS_PER_SLOT = 7;      // 7 OFDM-символов в слоте (нормальный CP)
    static constexpr size_t NUM_SLOTS_PER_SUBFRAME = 2;    // 2 слота в сабфрейме
    static constexpr size_t NUM_SYMBOLS_PER_SUBFRAME = NUM_SYMBOLS_PER_SLOT * NUM_SLOTS_PER_SUBFRAME; // 14 символов
    static constexpr size_t NUM_SUBFRAMES_PER_FRAME = 10;  // 10 сабфреймов в фрейме (10 мс)
    
    // Параметры NPSS (3GPP TS 36.211, 10.2.7.1)
    static constexpr size_t NPSS_NUM_SYMBOLS = 11;         // NPSS занимает 11 символов
    static constexpr size_t NPSS_NUM_SUBCARRIERS = 11;     // NPSS занимает 11 поднесущих
    static constexpr size_t NPSS_START_SYMBOL = 3;         // Последние 11 символов (индексы 3..13)
    static constexpr size_t NPSS_START_SUBCARRIER = 0;     // Поднесущие 0..10
    
    // Настройки генерации
    static constexpr size_t DEFAULT_NUM_FRAMES = 1;        // Сколько фреймов генерировать по умолчанию
};

} // namespace nbiot
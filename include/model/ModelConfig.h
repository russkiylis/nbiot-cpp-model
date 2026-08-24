/**
 * @file ModelConfig.h
 * @brief Конфигурация архитектурного каркаса нисходящего канала NB-IoT.
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "model/Types.h"

namespace nbiot {

/**
 * @brief Входная конфигурация DownlinkManager.
 *
 * Менеджер сам назначает обязательные сабфреймы NPBCH, NPSS и NSSS. Поэтому
 * flexibleSubframes может содержать только Empty, Npdsch или Npdcch. Значения на
 * фиксированных позициях 0 и 5, а также на позиции 9 чётного кадра, будут
 * переопределены правилами менеджера.
 */
struct ModelConfig {
    std::size_t frameCount{1};          ///< Число создаваемых радиокадров.
    std::uint32_t startFrameNumber{0};  ///< Абсолютный номер первого кадра.
    std::uint32_t cellId{0};            ///< Идентификатор соты, допустимо 0–503.
    std::array<SubframeType, kSubframesPerFrame> flexibleSubframes{};

    /**
     * @brief Проверяет конфигурацию до создания объектов модели.
     * @throws std::invalid_argument Если число кадров, cellId, диапазон номеров
     *         кадров или гибкий тип сабфрейма некорректны.
     */
    void validate() const;
};

}  // namespace nbiot

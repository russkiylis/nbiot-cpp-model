/**
 * @file ModelConfig.cpp
 * @brief Проверка конфигурации архитектурного каркаса NB-IoT.
 */

#include "model/ModelConfig.h"

#include <limits>
#include <stdexcept>

namespace nbiot {
namespace {

bool isFlexibleType(const SubframeType type) noexcept {
    return type == SubframeType::Empty || type == SubframeType::Npdsch ||
           type == SubframeType::Npdcch;
}

}  // namespace

void ModelConfig::validate() const {
    if (frameCount == 0) {
        throw std::invalid_argument("Количество кадров должно быть больше нуля");
    }

    if (cellId > 503) {
        throw std::invalid_argument("cellId должен находиться в диапазоне от 0 до 503");
    }

    const auto maximumFrameOffset =
        static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max() - startFrameNumber);
    if (frameCount - 1 > maximumFrameOffset) {
        throw std::invalid_argument("Диапазон номеров кадров выходит за пределы uint32_t");
    }

    for (const SubframeType type : flexibleSubframes) {
        if (!isFlexibleType(type)) {
            throw std::invalid_argument(
                "Гибкое расписание может содержать только Empty, NPDSCH или NPDCCH");
        }
    }
}

}  // namespace nbiot

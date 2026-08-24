/**
 * @file NpdcchSubframe.h
 * @brief Заготовка сабфрейма управляющего канала NPDCCH.
 */

#pragma once

#include "model/Subframe.h"

namespace nbiot {

/** @brief Конкретная архитектурная заготовка управляющего канала NPDCCH. */
class NpdcchSubframe final : public Subframe {
   public:
    using Subframe::Subframe;

    /// Возвращает SubframeType::Npdcch.
    [[nodiscard]] SubframeType type() const noexcept override;

    /// @throws std::logic_error Всегда, пока алгоритм NPDCCH не реализован.
    void mapTo(ResourceGrid& resourceGrid) const override;
};

}  // namespace nbiot

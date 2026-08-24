/**
 * @file NpdschSubframe.h
 * @brief Заготовка сабфрейма канала данных NPDSCH.
 */

#pragma once

#include "model/Subframe.h"

namespace nbiot {

/** @brief Конкретная архитектурная заготовка канала данных NPDSCH. */
class NpdschSubframe final : public Subframe {
   public:
    using Subframe::Subframe;

    /// Возвращает SubframeType::Npdsch.
    [[nodiscard]] SubframeType type() const noexcept override;

    /// @throws std::logic_error Всегда, пока алгоритм NPDSCH не реализован.
    void mapTo(ResourceGrid& resourceGrid) const override;
};

}  // namespace nbiot

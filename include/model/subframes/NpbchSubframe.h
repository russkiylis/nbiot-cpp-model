/**
 * @file NpbchSubframe.h
 * @brief Заготовка сабфрейма широковещательного канала NPBCH.
 */

#pragma once

#include "model/Subframe.h"

namespace nbiot {

/** @brief Конкретная архитектурная заготовка широковещательного канала NPBCH. */
class NpbchSubframe final : public Subframe {
   public:
    using Subframe::Subframe;

    /// Возвращает SubframeType::Npbch.
    [[nodiscard]] SubframeType type() const noexcept override;

    /// @throws std::logic_error Всегда, пока алгоритм NPBCH не реализован.
    void mapTo(ResourceGrid& resourceGrid) const override;
};

}  // namespace nbiot

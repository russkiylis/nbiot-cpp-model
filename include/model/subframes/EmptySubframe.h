/**
 * @file EmptySubframe.h
 * @brief Заготовка пустого сабфрейма.
 */

#pragma once

#include "model/Subframe.h"

namespace nbiot {

/** @brief Конкретная заготовка позиции без основного физического канала. */
class EmptySubframe final : public Subframe {
   public:
    using Subframe::Subframe;

    /// Возвращает SubframeType::Empty.
    [[nodiscard]] SubframeType type() const noexcept override;

    /// @throws std::logic_error Пока поведение пустой позиции и NRS не реализовано.
    void mapTo(ResourceGrid& resourceGrid) const override;
};

}  // namespace nbiot

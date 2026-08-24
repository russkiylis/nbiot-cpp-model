/**
 * @file NpssSubframe.h
 * @brief Заготовка сабфрейма первичного сигнала синхронизации NPSS.
 */

#pragma once

#include "model/Subframe.h"

namespace nbiot {

/** @brief Конкретная архитектурная заготовка сигнала синхронизации NPSS. */
class NpssSubframe final : public Subframe {
   public:
    using Subframe::Subframe;

    /// Возвращает SubframeType::Npss.
    [[nodiscard]] SubframeType type() const noexcept override;

    /// @throws std::logic_error Всегда, пока алгоритм NPSS не подключён.
    void mapTo(ResourceGrid& resourceGrid) const override;
};

}  // namespace nbiot

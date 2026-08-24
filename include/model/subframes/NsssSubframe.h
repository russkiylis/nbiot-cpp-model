/**
 * @file NsssSubframe.h
 * @brief Заготовка сабфрейма вторичного сигнала синхронизации NSSS.
 */

#pragma once

#include "model/Subframe.h"

namespace nbiot {

/** @brief Конкретная архитектурная заготовка сигнала синхронизации NSSS. */
class NsssSubframe final : public Subframe {
   public:
    using Subframe::Subframe;

    /// Возвращает SubframeType::Nsss.
    [[nodiscard]] SubframeType type() const noexcept override;

    /// @throws std::logic_error Всегда, пока алгоритм NSSS не подключён.
    void mapTo(ResourceGrid& resourceGrid) const override;
};

}  // namespace nbiot

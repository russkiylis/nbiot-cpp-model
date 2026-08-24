/**
 * @file NrsOverlay.h
 * @brief Отдельная стадия наложения опорного сигнала NRS.
 */

#pragma once

#include "model/ResourceGrid.h"
#include "model/Types.h"

namespace nbiot {

/**
 * @brief Заготовка сервиса, накладывающего NRS после основного канала сабфрейма.
 */
class NrsOverlay {
   public:
    /**
     * @brief Накладывает NRS на область указанного сабфрейма.
     * @throws std::logic_error Всегда в текущем архитектурном каркасе, поскольку
     *         алгоритм NRS ещё не реализован.
     */
    void apply(ResourceGrid& resourceGrid, const SubframeContext& context) const;
};

}  // namespace nbiot

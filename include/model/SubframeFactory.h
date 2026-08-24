/**
 * @file SubframeFactory.h
 * @brief Фабрика конкретных классов сабфреймов.
 */

#pragma once

#include <memory>

#include "model/Subframe.h"

namespace nbiot {

/**
 * @brief Создаёт конкретный сабфрейм, скрывая switch от менеджера и кадра.
 */
class SubframeFactory {
   public:
    SubframeFactory() = delete;

    /**
     * @brief Создаёт объект, соответствующий указанному типу.
     * @return Единственный владеющий указатель на новый сабфрейм.
     * @throws std::invalid_argument Если передано неизвестное значение enum.
     */
    [[nodiscard]] static std::unique_ptr<Subframe> create(SubframeType type,
                                                          SubframeContext context);
};

}  // namespace nbiot

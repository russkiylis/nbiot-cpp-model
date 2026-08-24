/**
 * @file NrsOverlay.cpp
 * @brief Реализация архитектурной заглушки NRS.
 */

#include "model/NrsOverlay.h"

#include <stdexcept>

namespace nbiot {

void NrsOverlay::apply(ResourceGrid& resourceGrid, const SubframeContext& context) const {
    (void)resourceGrid;
    (void)context;
    // TODO: реализовать формирование и размещение NRS.
    throw std::logic_error("Наложение NRS пока не реализовано");
}

}  // namespace nbiot

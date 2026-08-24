/**
 * @file NpdcchSubframe.cpp
 * @brief Реализация архитектурной заглушки NPDCCH.
 */

#include "model/subframes/NpdcchSubframe.h"

namespace nbiot {

SubframeType NpdcchSubframe::type() const noexcept { return SubframeType::Npdcch; }

void NpdcchSubframe::mapTo(ResourceGrid& resourceGrid) const {
    (void)resourceGrid;
    // TODO: получить DCI от планировщика и разместить NPDCCH в ресурсной сетке.
    throwMappingNotImplemented();
}

}  // namespace nbiot

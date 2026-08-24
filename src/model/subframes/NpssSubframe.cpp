/**
 * @file NpssSubframe.cpp
 * @brief Реализация архитектурной заглушки NPSS.
 */

#include "model/subframes/NpssSubframe.h"

namespace nbiot {

SubframeType NpssSubframe::type() const noexcept { return SubframeType::Npss; }

void NpssSubframe::mapTo(ResourceGrid& resourceGrid) const {
    (void)resourceGrid;
    // TODO: получить NPSS-последовательность и разместить её в ресурсной сетке.
    throwMappingNotImplemented();
}

}  // namespace nbiot

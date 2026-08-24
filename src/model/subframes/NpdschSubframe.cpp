/**
 * @file NpdschSubframe.cpp
 * @brief Реализация архитектурной заглушки NPDSCH.
 */

#include "model/subframes/NpdschSubframe.h"

namespace nbiot {

SubframeType NpdschSubframe::type() const noexcept { return SubframeType::Npdsch; }

void NpdschSubframe::mapTo(ResourceGrid& resourceGrid) const {
    (void)resourceGrid;
    // TODO: получить данные планировщика и разместить NPDSCH в ресурсной сетке.
    throwMappingNotImplemented();
}

}  // namespace nbiot

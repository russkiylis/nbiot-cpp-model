/**
 * @file EmptySubframe.cpp
 * @brief Реализация архитектурной заглушки пустого сабфрейма.
 */

#include "model/subframes/EmptySubframe.h"

namespace nbiot {

SubframeType EmptySubframe::type() const noexcept { return SubframeType::Empty; }

void EmptySubframe::mapTo(ResourceGrid& resourceGrid) const {
    (void)resourceGrid;
    // TODO: определить поведение пустого сабфрейма после реализации NRS.
    throwMappingNotImplemented();
}

}  // namespace nbiot

/**
 * @file NpbchSubframe.cpp
 * @brief Реализация архитектурной заглушки NPBCH.
 */

#include "model/subframes/NpbchSubframe.h"

namespace nbiot {

SubframeType NpbchSubframe::type() const noexcept { return SubframeType::Npbch; }

void NpbchSubframe::mapTo(ResourceGrid& resourceGrid) const {
    (void)resourceGrid;
    // TODO: подготовить и разместить NPBCH в ресурсной сетке.
    throwMappingNotImplemented();
}

}  // namespace nbiot

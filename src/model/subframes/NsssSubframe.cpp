/**
 * @file NsssSubframe.cpp
 * @brief Реализация архитектурной заглушки NSSS.
 */

#include "model/subframes/NsssSubframe.h"

namespace nbiot {

SubframeType NsssSubframe::type() const noexcept { return SubframeType::Nsss; }

void NsssSubframe::mapTo(ResourceGrid& resourceGrid) const {
    (void)resourceGrid;
    // TODO: получить NSSS-последовательность и разместить её в ресурсной сетке.
    throwMappingNotImplemented();
}

}  // namespace nbiot

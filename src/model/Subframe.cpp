/**
 * @file Subframe.cpp
 * @brief Реализация общего поведения абстрактного сабфрейма.
 */

#include "model/Subframe.h"

#include <stdexcept>
#include <string>

namespace nbiot {

Subframe::Subframe(const SubframeContext context) : context_(context) {
    if (context_.subframeIndex >= kSubframesPerFrame) {
        throw std::invalid_argument("Номер сабфрейма должен находиться в диапазоне от 0 до 9");
    }
}

const SubframeContext& Subframe::context() const noexcept { return context_; }

bool Subframe::allowsNrs() const noexcept { return supportsNrs(type()); }

void Subframe::throwMappingNotImplemented() const {
    throw std::logic_error("Маппинг сабфрейма " + std::string(toString(type())) +
                           " пока не реализован");
}

}  // namespace nbiot

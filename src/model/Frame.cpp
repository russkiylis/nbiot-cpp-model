/**
 * @file Frame.cpp
 * @brief Реализация контейнера радиокадра.
 */

#include "model/Frame.h"

#include <stdexcept>
#include <utility>

namespace nbiot {

Frame::Frame(const std::uint32_t frameNumber, Subframes subframes)
    : frameNumber_(frameNumber), subframes_(std::move(subframes)) {
    const SubframeContext* firstContext = nullptr;
    for (std::size_t subframeIndex = 0; subframeIndex < subframes_.size(); ++subframeIndex) {
        const auto& subframe = subframes_[subframeIndex];
        if (subframe == nullptr) {
            throw std::invalid_argument("Кадр не может владеть пустым указателем на сабфрейм");
        }

        const SubframeContext& context = subframe->context();
        if (context.frameNumber != frameNumber_) {
            throw std::invalid_argument("Номер кадра не совпадает с контекстом сабфрейма");
        }
        if (context.subframeIndex != subframeIndex) {
            throw std::invalid_argument("Позиция сабфрейма не совпадает с его контекстом");
        }

        if (firstContext == nullptr) {
            firstContext = &context;
        } else if (context.frameIndex != firstContext->frameIndex ||
                   context.cellId != firstContext->cellId) {
            throw std::invalid_argument(
                "Все сабфреймы кадра должны иметь одинаковые frameIndex и cellId");
        }
    }
}

std::uint32_t Frame::frameNumber() const noexcept { return frameNumber_; }

const Frame::Subframes& Frame::subframes() const noexcept { return subframes_; }

const Subframe& Frame::subframe(const std::size_t subframeIndex) const {
    return *subframes_.at(subframeIndex);
}

}  // namespace nbiot

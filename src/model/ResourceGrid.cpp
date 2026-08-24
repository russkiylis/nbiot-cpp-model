/**
 * @file ResourceGrid.cpp
 * @brief Реализация хранения и индексации ресурсной сетки.
 */

#include "model/ResourceGrid.h"

#include <stdexcept>

namespace nbiot {
namespace {

constexpr std::size_t kElementsPerSubframe = kSubcarriersPerSubframe * kSymbolsPerSubframe;
constexpr std::size_t kElementsPerFrame = kSubframesPerFrame * kElementsPerSubframe;

std::size_t checkedElementCount(const std::size_t frameCount) {
    if (frameCount == 0) {
        throw std::invalid_argument("Ресурсная сетка должна содержать хотя бы один кадр");
    }

    const std::vector<ResourceElement> emptyStorage;
    if (frameCount > emptyStorage.max_size() / kElementsPerFrame) {
        throw std::length_error("Запрошенный размер ресурсной сетки слишком велик");
    }

    return frameCount * kElementsPerFrame;
}

}  // namespace

ResourceGrid::ResourceGrid(const std::size_t frameCount)
    : frameCount_(frameCount), elements_(checkedElementCount(frameCount)) {}

std::size_t ResourceGrid::frameCount() const noexcept { return frameCount_; }

std::size_t ResourceGrid::elementCount() const noexcept { return elements_.size(); }

ResourceElement& ResourceGrid::at(const std::size_t frameIndex, const std::size_t subframeIndex,
                                  const std::size_t subcarrierIndex,
                                  const std::size_t symbolIndex) {
    return elements_.at(flatIndex(frameIndex, subframeIndex, subcarrierIndex, symbolIndex));
}

const ResourceElement& ResourceGrid::at(const std::size_t frameIndex,
                                        const std::size_t subframeIndex,
                                        const std::size_t subcarrierIndex,
                                        const std::size_t symbolIndex) const {
    return elements_.at(flatIndex(frameIndex, subframeIndex, subcarrierIndex, symbolIndex));
}

std::size_t ResourceGrid::flatIndex(const std::size_t frameIndex, const std::size_t subframeIndex,
                                    const std::size_t subcarrierIndex,
                                    const std::size_t symbolIndex) const {
    if (frameIndex >= frameCount_ || subframeIndex >= kSubframesPerFrame ||
        subcarrierIndex >= kSubcarriersPerSubframe || symbolIndex >= kSymbolsPerSubframe) {
        throw std::out_of_range("Индекс ресурсной сетки находится вне допустимого диапазона");
    }

    return frameIndex * kElementsPerFrame + subframeIndex * kElementsPerSubframe +
           symbolIndex * kSubcarriersPerSubframe + subcarrierIndex;
}

}  // namespace nbiot

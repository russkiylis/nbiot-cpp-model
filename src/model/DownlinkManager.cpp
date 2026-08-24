/**
 * @file DownlinkManager.cpp
 * @brief Реализация корневого менеджера архитектурной модели.
 */

#include "model/DownlinkManager.h"

#include <stdexcept>
#include <utility>

#include "model/SubframeFactory.h"

namespace nbiot {

DownlinkManager::DownlinkManager(ModelConfig config) : config_(std::move(config)) {
    config_.validate();
}

void DownlinkManager::buildTopology() {
    if (topologyBuilt_) {
        throw std::logic_error("Топология модели уже построена");
    }

    std::vector<Frame> newFrames;
    newFrames.reserve(config_.frameCount);
    ResourceGrid newResourceGrid(config_.frameCount);

    for (std::size_t frameIndex = 0; frameIndex < config_.frameCount; ++frameIndex) {
        const auto frameNumber = config_.startFrameNumber + static_cast<std::uint32_t>(frameIndex);
        Frame::Subframes subframes;

        for (std::size_t subframeIndex = 0; subframeIndex < kSubframesPerFrame; ++subframeIndex) {
            const SubframeContext context{
                .frameIndex = frameIndex,
                .frameNumber = frameNumber,
                .subframeIndex = subframeIndex,
                .cellId = static_cast<std::uint16_t>(config_.cellId),
            };
            const SubframeType type = resolveSubframeType(frameNumber, subframeIndex);
            subframes[subframeIndex] = SubframeFactory::create(type, context);
        }

        newFrames.emplace_back(frameNumber, std::move(subframes));
    }

    frames_ = std::move(newFrames);
    resourceGrid_ = std::move(newResourceGrid);
    topologyBuilt_ = true;
}

void DownlinkManager::generateResourceGrid() {
    requireBuiltTopology();

    ResourceGrid generatedGrid(config_.frameCount);
    for (const Frame& frame : frames_) {
        for (const auto& subframe : frame.subframes()) {
            subframe->mapTo(generatedGrid);
            if (subframe->allowsNrs()) {
                nrsOverlay_.apply(generatedGrid, subframe->context());
            }
        }
    }

    resourceGrid_ = std::move(generatedGrid);
}

bool DownlinkManager::topologyBuilt() const noexcept { return topologyBuilt_; }

const ModelConfig& DownlinkManager::config() const noexcept { return config_; }

const std::vector<Frame>& DownlinkManager::frames() const {
    requireBuiltTopology();
    return frames_;
}

const ResourceGrid& DownlinkManager::resourceGrid() const {
    requireBuiltTopology();
    return *resourceGrid_;
}

SubframeType DownlinkManager::resolveSubframeType(const std::uint32_t frameNumber,
                                                  const std::size_t subframeIndex) const {
    if (subframeIndex == 0) {
        return SubframeType::Npbch;
    }
    if (subframeIndex == 5) {
        return SubframeType::Npss;
    }
    if (subframeIndex == 9 && frameNumber % 2 == 0) {
        return SubframeType::Nsss;
    }

    return config_.flexibleSubframes.at(subframeIndex);
}

void DownlinkManager::requireBuiltTopology() const {
    if (!topologyBuilt_ || !resourceGrid_.has_value()) {
        throw std::logic_error("Сначала вызовите buildTopology()");
    }
}

}  // namespace nbiot

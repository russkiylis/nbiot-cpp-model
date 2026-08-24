#include <gtest/gtest.h>

#include <array>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "model/DownlinkManager.h"
#include "model/ModelConfig.h"
#include "model/NrsOverlay.h"
#include "model/ResourceGrid.h"
#include "model/Subframe.h"
#include "model/SubframeFactory.h"
#include "model/Types.h"
#include "model/subframes/EmptySubframe.h"
#include "model/subframes/NpbchSubframe.h"
#include "model/subframes/NpdcchSubframe.h"
#include "model/subframes/NpdschSubframe.h"
#include "model/subframes/NpssSubframe.h"
#include "model/subframes/NsssSubframe.h"

namespace nbiot {
namespace {

static_assert(std::is_abstract_v<Subframe>);
static_assert(std::has_virtual_destructor_v<Subframe>);
static_assert(!std::is_move_constructible_v<DownlinkManager>);

SubframeContext makeContext(const std::size_t subframeIndex = 3) {
    return SubframeContext{
        .frameIndex = 2,
        .frameNumber = 101,
        .subframeIndex = subframeIndex,
        .cellId = 42,
    };
}

template <typename ConcreteSubframe>
void expectFactoryProduct(const SubframeType type, const SubframeContext& expectedContext) {
    const std::unique_ptr<Subframe> subframe = SubframeFactory::create(type, expectedContext);

    ASSERT_NE(subframe, nullptr);
    EXPECT_NE(dynamic_cast<const ConcreteSubframe*>(subframe.get()), nullptr);
    EXPECT_EQ(subframe->type(), type);
    EXPECT_EQ(subframe->context().frameIndex, expectedContext.frameIndex);
    EXPECT_EQ(subframe->context().frameNumber, expectedContext.frameNumber);
    EXPECT_EQ(subframe->context().subframeIndex, expectedContext.subframeIndex);
    EXPECT_EQ(subframe->context().cellId, expectedContext.cellId);
}

ModelConfig makeValidConfig() {
    ModelConfig config;
    config.flexibleSubframes.fill(SubframeType::Empty);
    return config;
}

Frame::Subframes makeFrameSubframes(const std::size_t frameIndex, const std::uint32_t frameNumber,
                                    const std::uint16_t cellId) {
    Frame::Subframes subframes;
    for (std::size_t subframeIndex = 0; subframeIndex < subframes.size(); ++subframeIndex) {
        subframes[subframeIndex] = SubframeFactory::create(
            SubframeType::Empty, SubframeContext{frameIndex, frameNumber, subframeIndex, cellId});
    }
    return subframes;
}

TEST(SubframeFactoryTest, CreatesAllConcreteTypesAndPreservesContext) {
    const SubframeContext context = makeContext();

    expectFactoryProduct<EmptySubframe>(SubframeType::Empty, context);
    expectFactoryProduct<NpbchSubframe>(SubframeType::Npbch, context);
    expectFactoryProduct<NpssSubframe>(SubframeType::Npss, context);
    expectFactoryProduct<NsssSubframe>(SubframeType::Nsss, context);
    expectFactoryProduct<NpdschSubframe>(SubframeType::Npdsch, context);
    expectFactoryProduct<NpdcchSubframe>(SubframeType::Npdcch, context);
}

TEST(SubframeFactoryTest, RejectsContextWithInvalidSubframeIndex) {
    const SubframeContext invalidContext = makeContext(kSubframesPerFrame);

    EXPECT_THROW(static_cast<void>(SubframeFactory::create(SubframeType::Empty, invalidContext)),
                 std::invalid_argument);
}

TEST(ModelConfigTest, RejectsZeroFrameCount) {
    ModelConfig config = makeValidConfig();
    config.frameCount = 0;

    EXPECT_THROW(config.validate(), std::invalid_argument);
}

TEST(ModelConfigTest, RejectsCellIdOutsideSupportedRange) {
    ModelConfig config = makeValidConfig();
    config.cellId = 504;

    EXPECT_THROW(config.validate(), std::invalid_argument);
}

TEST(ModelConfigTest, ValidatesCellIdBeforeNarrowingToSubframeContext) {
    ModelConfig config = makeValidConfig();
    config.cellId = 65'536;

    EXPECT_THROW(config.validate(), std::invalid_argument);
}

TEST(ModelConfigTest, RejectsFixedTypesInFlexibleSchedule) {
    struct InvalidScheduleEntry {
        SubframeType type;
        std::size_t index;
    };
    const std::array<InvalidScheduleEntry, 3> invalidEntries{{
        {SubframeType::Npbch, 0},
        {SubframeType::Npss, 5},
        {SubframeType::Nsss, 9},
    }};

    for (const InvalidScheduleEntry entry : invalidEntries) {
        SCOPED_TRACE(std::string(toString(entry.type)));
        ModelConfig config = makeValidConfig();
        config.flexibleSubframes[entry.index] = entry.type;
        EXPECT_THROW(config.validate(), std::invalid_argument);
    }
}

TEST(ModelConfigTest, RejectsFrameNumberOverflow) {
    ModelConfig config = makeValidConfig();
    config.frameCount = 2;
    config.startFrameNumber = std::numeric_limits<std::uint32_t>::max();

    EXPECT_THROW(config.validate(), std::invalid_argument);
}

TEST(DownlinkManagerTest, RejectsAccessAndGenerationBeforeTopologyBuild) {
    DownlinkManager manager(makeValidConfig());

    EXPECT_FALSE(manager.topologyBuilt());
    EXPECT_THROW(static_cast<void>(manager.frames()), std::logic_error);
    EXPECT_THROW(static_cast<void>(manager.resourceGrid()), std::logic_error);
    EXPECT_THROW(manager.generateResourceGrid(), std::logic_error);
}

TEST(DownlinkManagerTest, BuildsRequestedFramesAndTenSubframesPerFrame) {
    ModelConfig config = makeValidConfig();
    config.frameCount = 3;
    config.startFrameNumber = 21;
    config.cellId = 301;
    DownlinkManager manager(config);

    manager.buildTopology();

    ASSERT_TRUE(manager.topologyBuilt());
    const std::vector<Frame>& frames = manager.frames();
    ASSERT_EQ(frames.size(), config.frameCount);
    for (std::size_t frameIndex = 0; frameIndex < frames.size(); ++frameIndex) {
        const Frame& frame = frames[frameIndex];
        EXPECT_EQ(frame.frameNumber(), config.startFrameNumber + frameIndex);
        ASSERT_EQ(frame.subframes().size(), kSubframesPerFrame);

        for (std::size_t subframeIndex = 0; subframeIndex < kSubframesPerFrame; ++subframeIndex) {
            ASSERT_NE(frame.subframes()[subframeIndex], nullptr);
            const SubframeContext& context = frame.subframe(subframeIndex).context();
            EXPECT_EQ(context.frameIndex, frameIndex);
            EXPECT_EQ(context.frameNumber, frame.frameNumber());
            EXPECT_EQ(context.subframeIndex, subframeIndex);
            EXPECT_EQ(context.cellId, config.cellId);
        }
    }
}

TEST(DownlinkManagerTest, AppliesFixedAndFlexibleSubframeRules) {
    ModelConfig config = makeValidConfig();
    config.frameCount = 2;
    config.startFrameNumber = 10;
    config.flexibleSubframes = {
        SubframeType::Npdsch, SubframeType::Npdcch, SubframeType::Npdsch, SubframeType::Empty,
        SubframeType::Npdcch, SubframeType::Npdsch, SubframeType::Empty,  SubframeType::Npdsch,
        SubframeType::Npdcch, SubframeType::Npdsch,
    };
    DownlinkManager manager(config);

    manager.buildTopology();

    const std::vector<Frame>& frames = manager.frames();
    ASSERT_EQ(frames.size(), 2U);
    for (std::size_t frameIndex = 0; frameIndex < frames.size(); ++frameIndex) {
        const Frame& frame = frames[frameIndex];
        for (std::size_t subframeIndex = 0; subframeIndex < kSubframesPerFrame; ++subframeIndex) {
            SubframeType expectedType = config.flexibleSubframes[subframeIndex];
            if (subframeIndex == 0) {
                expectedType = SubframeType::Npbch;
            } else if (subframeIndex == 5) {
                expectedType = SubframeType::Npss;
            } else if (subframeIndex == 9 && frame.frameNumber() % 2 == 0) {
                expectedType = SubframeType::Nsss;
            }

            SCOPED_TRACE("Кадр " + std::to_string(frame.frameNumber()) + ", сабфрейм " +
                         std::to_string(subframeIndex));
            EXPECT_EQ(frame.subframe(subframeIndex).type(), expectedType);
        }
    }

    // Фиксированные позиции переопределяют гибкое расписание.
    EXPECT_EQ(frames[0].subframe(0).type(), SubframeType::Npbch);
    EXPECT_EQ(frames[0].subframe(5).type(), SubframeType::Npss);
    EXPECT_EQ(frames[0].subframe(9).type(), SubframeType::Nsss);
    EXPECT_EQ(frames[1].subframe(9).type(), SubframeType::Npdsch);
}

TEST(DownlinkManagerTest, RejectsRepeatedTopologyBuild) {
    DownlinkManager manager(makeValidConfig());
    manager.buildTopology();

    EXPECT_THROW(manager.buildTopology(), std::logic_error);
}

TEST(FrameTest, RejectsSubframesWithInconsistentContext) {
    {
        Frame::Subframes subframes = makeFrameSubframes(0, 10, 42);
        subframes[3] = SubframeFactory::create(SubframeType::Empty, SubframeContext{0, 11, 3, 42});
        EXPECT_THROW(static_cast<void>(Frame(10, std::move(subframes))), std::invalid_argument);
    }

    {
        Frame::Subframes subframes = makeFrameSubframes(0, 10, 42);
        subframes[3] = SubframeFactory::create(SubframeType::Empty, SubframeContext{0, 10, 4, 42});
        EXPECT_THROW(static_cast<void>(Frame(10, std::move(subframes))), std::invalid_argument);
    }

    {
        Frame::Subframes subframes = makeFrameSubframes(0, 10, 42);
        subframes[3] = SubframeFactory::create(SubframeType::Empty, SubframeContext{1, 10, 3, 43});
        EXPECT_THROW(static_cast<void>(Frame(10, std::move(subframes))), std::invalid_argument);
    }
}

TEST(ResourceGridTest, HasExpectedDimensionsAndDefaultEmptyElements) {
    constexpr std::size_t kFrameCount = 2;
    ResourceGrid grid(kFrameCount);

    EXPECT_EQ(grid.frameCount(), kFrameCount);
    EXPECT_EQ(grid.elementCount(),
              kFrameCount * kSubframesPerFrame * kSubcarriersPerSubframe * kSymbolsPerSubframe);

    for (std::size_t frameIndex = 0; frameIndex < kFrameCount; ++frameIndex) {
        for (std::size_t subframeIndex = 0; subframeIndex < kSubframesPerFrame; ++subframeIndex) {
            for (std::size_t subcarrierIndex = 0; subcarrierIndex < kSubcarriersPerSubframe;
                 ++subcarrierIndex) {
                for (std::size_t symbolIndex = 0; symbolIndex < kSymbolsPerSubframe;
                     ++symbolIndex) {
                    const ResourceElement& element =
                        grid.at(frameIndex, subframeIndex, subcarrierIndex, symbolIndex);
                    EXPECT_EQ(element.value, std::complex<float>{});
                    EXPECT_EQ(element.type, ResourceElementType::Empty);
                }
            }
        }
    }
}

TEST(ResourceGridTest, RejectsEveryOutOfRangeDimension) {
    ResourceGrid grid(2);
    const ResourceGrid& constGrid = grid;

    EXPECT_THROW(constGrid.at(2, 0, 0, 0), std::out_of_range);
    EXPECT_THROW(constGrid.at(0, kSubframesPerFrame, 0, 0), std::out_of_range);
    EXPECT_THROW(constGrid.at(0, 0, kSubcarriersPerSubframe, 0), std::out_of_range);
    EXPECT_THROW(constGrid.at(0, 0, 0, kSymbolsPerSubframe), std::out_of_range);
}

TEST(ResourceGridTest, RejectsElementCountOverflow) {
    EXPECT_THROW(static_cast<void>(ResourceGrid(std::numeric_limits<std::size_t>::max())),
                 std::length_error);
}

TEST(SubframeTest, AllowsNrsForEveryTypeExceptSynchronizationSignals) {
    const std::array<std::pair<SubframeType, bool>, 6> cases{{
        {SubframeType::Empty, true},
        {SubframeType::Npbch, true},
        {SubframeType::Npss, false},
        {SubframeType::Nsss, false},
        {SubframeType::Npdsch, true},
        {SubframeType::Npdcch, true},
    }};

    for (std::size_t index = 0; index < cases.size(); ++index) {
        const auto [type, expected] = cases[index];
        SCOPED_TRACE(std::string(toString(type)));
        const std::unique_ptr<Subframe> subframe =
            SubframeFactory::create(type, makeContext(index));
        EXPECT_EQ(subframe->allowsNrs(), expected);
    }
}

TEST(SubframeTest, EveryMappingStubThrowsLogicError) {
    const std::array<SubframeType, 6> types{
        SubframeType::Empty, SubframeType::Npbch,  SubframeType::Npss,
        SubframeType::Nsss,  SubframeType::Npdsch, SubframeType::Npdcch,
    };
    ResourceGrid grid(1);

    for (std::size_t index = 0; index < types.size(); ++index) {
        const SubframeType type = types[index];
        SCOPED_TRACE(std::string(toString(type)));
        const std::unique_ptr<Subframe> subframe =
            SubframeFactory::create(type, makeContext(index));
        EXPECT_THROW(subframe->mapTo(grid), std::logic_error);
    }
}

TEST(NrsOverlayTest, ApplyStubThrowsLogicError) {
    ResourceGrid grid(1);
    NrsOverlay overlay;

    EXPECT_THROW(overlay.apply(grid, makeContext()), std::logic_error);
}

TEST(DownlinkManagerTest, GenerationAfterBuildStopsAtAlgorithmStub) {
    DownlinkManager manager(makeValidConfig());
    manager.buildTopology();

    EXPECT_THROW(manager.generateResourceGrid(), std::logic_error);
    const ResourceElement& firstElement = manager.resourceGrid().at(0, 0, 0, 0);
    EXPECT_EQ(firstElement.value, std::complex<float>{});
    EXPECT_EQ(firstElement.type, ResourceElementType::Empty);
}

}  // namespace
}  // namespace nbiot

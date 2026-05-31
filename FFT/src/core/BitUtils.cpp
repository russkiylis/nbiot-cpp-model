#include "BitUtils.h"

#include <random>

namespace {
constexpr std::uint32_t kDefaultSeed = 1337u;
}

std::vector<int> GenerateBitSequence(long length) {
    return GenerateBitSequence(length, kDefaultSeed);
}

std::vector<int> GenerateBitSequence(long length, std::uint32_t seed) {
    std::vector<int> bits;
    bits.reserve(length);

    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);

    for (long i = 0; i < length; ++i) {
        bits.push_back(distribution(generator));
    }

    return bits;
}

std::vector<std::vector<int>> SplitChannels(const std::vector<int>& bits) {

    std::vector<std::vector<int>> channels(4);
    for (long i = 0; i < bits.size(); ++i) {
        channels[i % 4].push_back(bits[i]);
    }

    return channels;
}

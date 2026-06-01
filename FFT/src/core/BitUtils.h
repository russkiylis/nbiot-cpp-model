#pragma once

#include <cstdint>
#include <vector>

std::vector<int> GenerateBitSequence(long length);
std::vector<int> GenerateBitSequence(long length, std::uint32_t seed);
// channelCount — число каналов (поднесущих), по умолчанию 4
std::vector<std::vector<int>> SplitChannels(const std::vector<int>& bits, int channelCount = 4);

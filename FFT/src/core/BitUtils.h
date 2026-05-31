#pragma once

#include <cstdint>
#include <vector>

std::vector<int> GenerateBitSequence(long length);
std::vector<int> GenerateBitSequence(long length, std::uint32_t seed);
std::vector<std::vector<int>> SplitChannels(const std::vector<int>& bits);

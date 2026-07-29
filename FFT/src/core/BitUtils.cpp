#include "BitUtils.h"

#include <random>

namespace {
constexpr std::uint32_t kDefaultSeed = 1337u;
}

std::vector<int> GenerateBitSequence(long length) {
    return GenerateBitSequence(length, kDefaultSeed);
}

// Генератор последовательности битов
std::vector<int> GenerateBitSequence(long length, std::uint32_t seed) {
    std::vector<int> bits;
    bits.reserve(length);

    //Установка сида
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);
    
    // Цикл генерации
    for (long i = 0; i < length; i++) {
        bits.push_back(distribution(generator));
    }

    return bits;
}

//Разделение битов на параллельные каналы
std::vector<std::vector<int>> SplitChannels(const std::vector<int>& bits, int channelCount) {
    // Проверка на корректность количества каналов
    if (channelCount <= 0)
        return {};

    std::vector<int> padded = bits;

    // Переход в знаковый тип для сравнения с channelCount
    int remainder = static_cast<int>(padded.size()) % channelCount;
    // Добавление нулей для выравнивания
    if (remainder != 0)
        padded.resize(padded.size() + (channelCount - remainder), 0);

    // Создание подпотоков
    std::vector<std::vector<int>> channels(channelCount);
    for (size_t i = 0; i < padded.size(); i++) {
        channels[i % channelCount].push_back(padded[i]);
    }
    return channels;
}


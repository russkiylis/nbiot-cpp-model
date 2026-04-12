/**
 * @file NpssGenerator.cpp
 * @brief Реализация генератора NPSS-последовательности (source).
 * @authors russkiylis, Tobyret101
 * @date 30.03.2026
 * @version 1.0
 * @copyright MIT License
 */

#include "sync/NpssGenerator.h"

NpssGenerator::NpssGenerator() = default;
NpssGenerator::~NpssGenerator() = default;

const std::array<std::complex<float>, 121>& NpssGenerator::getNpssSequence() const {
    // Реализация метода для получения NPSS последовательности

    // static const + лямбда-функция
    // лямбда выполняется ровно один раз (при первом вызове),
    // результат сохраняется в npss_sequence на всё время жизни программы.
    static const std::array<std::complex<float>, 121> npss_sequence = []() {
        // Лямбда-функция для генерации NPSS последовательности

        // Инициализация нулями {} гарантирует, что все элементы
        // в массиве не будет мусора, а будут комплексные нули (0 + j0)
        std::array<std::complex<float>, 121> sequence{};

        // Генерация NPSS последовательности по формуле
        // Проходим по каждому OFDM-символу (l) и поднесущей (n)
        constexpr float PI_F = 3.14159265358979323846f;
        for (int n = 0; n < 11; n++) {
            for (int l = 0; l < 11; l++) {
                int index = n * 11 + l;

                // static_cast<float> — явное преобразование из double (M_PI)
                // во float, чтобы избежать предупреждений компилятора
                sequence[index] =
                    static_cast<float>(s_[l]) *
                    std::exp(std::complex<float>(
                        0, static_cast<float>(-PI_F) * root_index_ * n * (n + 1) / 11));
            }
        }

        return sequence;
    }();

    return npss_sequence;
}


#include "misc/Correlator.h"

Correlator::Correlator() = default;
Correlator::~Correlator() = default;

std::array<std::complex<float>, 241> Correlator::cyclic_autocorrelation(std::array<std::complex<float>, 121>& sequence) {
    // Реализация метода для циклической корреляции

    std::array<std::complex<float>, 241> correlation;
    auto shifted_sequence = sequence;

    size_t k = 0;
    for (int tau = -120; tau <=120; tau++) {

        // Сдвинутый массив
        for (size_t i = 0; i < sequence.size(); i++) {
            int shift_i = i - tau;
            if (shift_i < 0) {
                shift_i = sequence.size() + shift_i;
            }
            if (shift_i > 120) {
                shift_i = shift_i - sequence.size();
            }

            shifted_sequence[i] = sequence[shift_i];
        }

        std::complex<float> sum;
        for (size_t i = 0; i < sequence.size(); i++) {
            sum += sequence[i] * conj(shifted_sequence[i]);
        }

        correlation[k] = sum;
        k++;
    }

    // Вернуть результат корреляции (например, массив или вектор)
    return correlation;
}

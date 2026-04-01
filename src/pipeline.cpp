/**
 * @file pipeline.cpp
 * @brief Точка входа: генерация и вывод NPSS-последовательности.
 *
 * Демонстрационный пайплайн
 * для тестирования генератора NPSS-последовательности.
 *
 * @authors russkiylis, Tobyret101
 * @date 30.03.2026
 * @version 1.0
 */

#include <iostream>

#include "sync/NpssGenerator.h"
#include "misc/Correlator.h"

int main() {
    // getNpssSequence() возвращает ссылку на static const массив
    // внутри функции, он живёт до конца программы.
    const auto& npss_seq = NpssGenerator().getNpssSequence();
    auto meow = npss_seq;

    Correlator correlator;
    const auto& npss_correlation = correlator.cyclic_autocorrelation(meow);
    

    for (size_t i = 0; i < npss_seq.size(); i++) {
        std::cout << "NPSS[" << i << "] = " << npss_seq[i] << std::endl;
    }

    for (size_t i = 0; i < npss_correlation.size(); i++) {
        std::cout << "Corr[" << i << "] = " << npss_correlation[i] << std::endl;
    }

    return 0;
}

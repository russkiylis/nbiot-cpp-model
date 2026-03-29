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

int main() {
    // getNpssSequence() возвращает ссылку на static const массив
    // внутри функции, он живёт до конца программы.
    const auto& npss_seq = NpssGenerator().getNpssSequence();

    for (size_t i = 0; i < npss_seq.size(); i++) {
        std::cout << "NPSS[" << i << "] = " << npss_seq[i] << std::endl;
    }
    
    return 0;
}


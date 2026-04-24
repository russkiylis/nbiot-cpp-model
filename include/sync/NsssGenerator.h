/**
 * @file NsssGenerator.h
 * @brief Генератор NSSS последовательности (Narrowband Secondary Synchronization Signal)
 * @authors russkiylis, Tobyret101
 * @date 19.04.2026
 */

#ifndef NSSS_GENERATOR_H
#define NSSS_GENERATOR_H

#include <array>
#include <complex>
#include <vector>

class NsssGenerator {
public:
    /**
     * @brief Конструктор
     * @param ncell_id ID соты (0-503)
     * @param frame_id Номер фрейма
     */
    NsssGenerator(int ncell_id, int frame_id);
    
    ~NsssGenerator();
    
    /**
     * @brief Получить NSSS последовательность (132 элемента)
     * @return Константная ссылка на массив комплексных чисел
     */
    const std::array<std::complex<float>, 132>& getNsssSequence() const;
    
    /**
     * @brief Получить NSSS в виде матрицы 12x11 (для ресурсной сетки)
     * @return Массив 12x11 комплексных чисел
     */
    const std::array<std::array<std::complex<float>, 11>, 12>& getNsssMatrix() const;

private:
    mutable bool is_initialized_;
    mutable std::array<std::complex<float>, 132> sequence_;
    mutable std::array<std::array<std::complex<float>, 11>, 12> matrix_;
    
    int ncell_id_;
    int frame_id_;
    
    /**
     * @brief Таблица b (128 элементов) для NSSS
     */
    static const std::array<std::array<int, 128>, 4> b_table_;
    
    /**
     * @brief Инициализация последовательности (лямбда)
     */
    void init() const;
};

#endif // NSSS_GENERATOR_H
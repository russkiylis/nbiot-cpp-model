/**
 * @file NpssGenerator.h
 * @brief Реализация генератора NPSS-последовательности (header).
 * @authors russkiylis, Tobyret101
 * @date 30.03.2026
 * @version 1.0
 * @copyright MIT License
 */

#pragma once

#include <array>
#include <complex>

/**
 * @class NpssGenerator
 * @brief Генератор NPSS (Narrowband Primary Synchronization Signal) последовательности
 *
 * Генерирует NPSS последовательность для первичной грубой синхронизации в NB-IoT.
 * Последовательность строится на основе последовательности Задова-Чу:
 *
 * @f[
 *   d_l(n) = S(l) \cdot e^{-j \frac{\pi u n(n+1)}{11}}, \quad n = 0, 1, \ldots, 10
 * @f]
 *
 * где:
 * - @f$ u = 5 @f$ — корневой индекс последовательности Задова-Чу
 * - @f$ S(l) @f$ — множитель, зависящий от индекса OFDM-символа @f$ l @f$
 *   (Table 10.2.7.1.1-1)
 * - @f$ n @f$ — индекс элемента внутри одного OFDM-символа
 *
 * Полная NPSS последовательность длиной 121 элемент формируется из 11 OFDM-символов
 * по 11 элементов в каждом.
 *
 * @see 3GPP TS 36.211 V17.0.0, Section 10.2.7.1.1 "Sequence generation"
 * @see
 * https://portal.3gpp.org/desktopmodules/Specifications/SpecificationDetails.aspx?specificationId=2425
 */
class NpssGenerator {
   public:
    /**
     * @brief Конструктор класса NpssGenerator
     */
    NpssGenerator();

    /**
     * @brief Деструктор класса NpssGenerator
     */
    ~NpssGenerator();

    /**
     * @brief Возвращает сгенерированную NPSS-последовательность.
     *
     * Последовательность вычисляется при первом вызове и кэшируется
     * на всё время жизни программы (static const + лямбда-инициализация).
     *
     * Формула (3GPP TS 36.211, 10.2.7.1):
     * @f[
     *   d_l(n) = S(l) \cdot e^{-j \frac{\pi u n(n+1)}{11}}, \quad n = 0, 1, \ldots, 10
     * @f]
     *
     * где:
     * - @f$ u = 5 @f$ — корневой индекс последовательности Задова-Чу
     * - @f$ S(l) @f$ — множитель, зависящий от индекса OFDM-символа @f$ l @f$
     *   (Table 10.2.7.1.1-1)
     * - @f$ n @f$ — индекс элемента внутри одного OFDM-символа
     *
     * @return Константная ссылка на std::array из 121 комплексных отсчётов.
     */
    const std::array<std::complex<float>, 121>& getNpssSequence() const;

   protected:
    /// @brief Корневой индекс u для генерации NPSS с помощью последовательности Задова-Чу
    static constexpr int root_index_ = 5;

    /// @brief Массив множителей S(l) для генерации NPSS, где l - индекс OFDM-символа
    ///        Длина массива соответствует количеству OFDM-символов в сабфрейме с NPSS (11 символов
    ///        из 14)
    static constexpr std::array<int, 11> s_ = {1, 1, 1, 1, -1, -1, 1, 1, 1, -1, 1};
};
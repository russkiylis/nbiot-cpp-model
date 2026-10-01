/**
 * @file crc16_bitwise.h
 * @brief Побитовая реализация CRC-16.
 * @authors russkiylis, Tobyret101
 * @date 04.07.2026
 * @version 1.0
 * @copyright MIT License
 */

#pragma once

#include <vector>
#include <cstdint>
#include <string>

/**
 * @brief Класс для побитового вычисления CRC-16
 *        Точная копия MATLAB-функции с полиномом d = [1 5 12 17]
 */
class CRC16Bitwise {
public:
    /**
     * @brief Конструктор
     */
    CRC16Bitwise() = default;
    
    /**
     * @brief Деструктор
     */
    ~CRC16Bitwise() = default;
    
    /**
     * @brief Вычисляет CRC-16 для вектора битов
     * @param arr Вектор битов (0/1), длина 34
     * @return Вектор битов (0/1), длина 50 (34 + 16)
     */
    std::vector<uint8_t> calculate(const std::vector<uint8_t>& arr);
    
    /**
     * @brief Вычисляет CRC-16 для строки битов
     * @param str Строка из '0' и '1', длина 34
     * @return Строка из '0' и '1', длина 50
     */
    std::string calculate(const std::string& str);
    
    /**
     * @brief Получить последнее вычисленное CRC-значение (16 бит)
     */
    std::vector<uint8_t> getLastCRC() const { return last_crc_; }
    
    /**
     * @brief Сбросить состояние
     */
    void reset();

private:
    std::vector<uint8_t> last_crc_;  // последние 16 бит CRC
    
    /**
     * @brief Конвертирует строку '0'/'1' в вектор битов
     */
    std::vector<uint8_t> stringToBits(const std::string& str);
    
    /**
     * @brief Конвертирует вектор битов в строку
     */
    std::string bitsToString(const std::vector<uint8_t>& bits);
    
};
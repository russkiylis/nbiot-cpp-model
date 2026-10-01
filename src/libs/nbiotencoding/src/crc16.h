
#pragma once
#include <cstdint>
#include <vector>

class CRC16 {
public:
    static constexpr uint16_t POLYNOMIAL = 0x1021;  // x^16 + x^12 + x^5 + 1
    static constexpr uint16_t INITIAL_VALUE = 0xFFFF;
    
    /**
     * @brief Вычислить CRC-16 для массива байт
     */
    static uint16_t calculate(const uint8_t* data, size_t len);
    
    /**
     * @brief Вычислить CRC-16 для вектора байт
     */
    static uint16_t calculate(const std::vector<uint8_t>& data);
    
    /**
     * @brief Добавить 2 байта CRC в конец вектора
     */
    static void append(std::vector<uint8_t>& data);
    
    /**
     * @brief Проверить CRC (последние 2 байта — CRC)
     */
    static bool verify(const std::vector<uint8_t>& data);
    
    /**
     * @brief Получить последнее вычисленное CRC
     */
    static uint16_t lastCRC() { return last_crc_; }
    
    /**
     * @brief Сбросить состояние (для следующих вычислений)
     */
    static void reset() { last_crc_ = 0; }

private:
    static uint16_t table[256];
    static bool table_initialized;
    static uint16_t last_crc_;
    
    static void initTable();
};
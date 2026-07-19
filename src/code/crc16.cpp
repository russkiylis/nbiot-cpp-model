/**
 * @file crc16.cpp
 * @brief Реализация crc-16.
 * @authors russkiylis, Tobyret101
 * @date 30.05.2026
 * @version 1.0
 * @copyright MIT License
 */

#include "code/crc16.h"
#include <cstring>
// Побайтовая реализация не работает, однако быстрее на 70-80 мс
// чем побитовая при 10000 последовательностях, на моем железе во всяком случае
// Возможно стоит вернутся позже

uint16_t CRC16::table[256];
bool CRC16::table_initialized = false;
uint16_t CRC16::last_crc_ = 0;

void CRC16::initTable() {
    for (int i = 0; i < 256; i++) {
        uint16_t crc = (i << 8);
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
        table[i] = crc;
    }
    table_initialized = true;
}

uint16_t CRC16::calculate(const uint8_t* data, size_t len) {
    if (!table_initialized) {
        initTable();
    }
    
    uint16_t crc = INITIAL_VALUE;
    
    for (size_t i = 0; i < len; i++) {
        uint8_t index = (crc >> 8) ^ data[i];
        crc = (crc << 8) ^ table[index];
    }
    
    last_crc_ = crc;
    return crc;
}

uint16_t CRC16::calculate(const std::vector<uint8_t>& data) {
    return calculate(data.data(), data.size());
}

void CRC16::append(std::vector<uint8_t>& data) {
    uint16_t crc = calculate(data);
    data.push_back((crc >> 8) & 0xFF);   // старший байт
    data.push_back(crc & 0xFF);           // младший байт
}

bool CRC16::verify(const std::vector<uint8_t>& data) {
    if (data.size() < 2) {
        return false;
    }
    
    // Данные без последних 2 байт
    std::vector<uint8_t> body(data.begin(), data.end() - 2);
    
    // Вычисляем CRC для данных
    uint16_t calculated = calculate(body);
    
    // Берём сохранённый CRC из последних 2 байт
    uint16_t received = (data[data.size() - 2] << 8) | data[data.size() - 1];
    
    return calculated == received;
}
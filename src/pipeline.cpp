/**
 * @file pipeline.cpp
 * @brief Точка входа: генерация и вывод NPSS-последовательности.
 * Демонстрационный пайплайн для тестирования генератора NPSS-последовательности.
 *
 * @authors russkiylis, Tobyret101
 * @date 30.03.2026
 * @version 1.0
 */
 // 
#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <iomanip>
#include <chrono>

#include <bitset>
#include <random>

#include "sync/NpssGenerator.h"
#include "sync/NsssGenerator.h"
#include "misc/Correlator.h"
#include "code/crc16.h"

/**
 * @brief Конвертирует 34 бита в 5 байт (с обнулением лишних битов)
 * @param bits Вектор из 34 битов (0/1)
 * @return Вектор из 5 байт (последний байт содержит только 2 старших бита данных)
 */
std::vector<uint8_t> bits34ToBytes(const std::vector<uint8_t>& bits) {
    if (bits.size() != 34) {
        throw std::runtime_error("Ожидается ровно 34 бита");
    }
    
    std::vector<uint8_t> bytes(5, 0);  // 5 байт, все нули
    
    for (size_t i = 0; i < 34; i++) {
        if (bits[i]) {
            size_t byte_idx = i / 8;
            size_t bit_idx = 7 - (i % 8);  // MSB first
            bytes[byte_idx] |= (1 << bit_idx);
        }
    }
    
    return bytes;
}

/**
 * @brief Конвертирует байты + CRC обратно в биты (для отладки)
 * @param bytes Вектор байт
 * @return Вектор битов
 */
std::vector<uint8_t> bytesToBits(const std::vector<uint8_t>& bytes) {
    std::vector<uint8_t> bits;
    bits.reserve(bytes.size() * 8);
    
    for (uint8_t byte : bytes) {
        for (int bit = 7; bit >= 0; bit--) {
            bits.push_back((byte >> bit) & 1);
        }
    }
    
    return bits;
}

/**
 * @brief Генерация случайных 34 битов
 */
std::vector<uint8_t> generateRandom34Bits() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    
    std::vector<uint8_t> bits;
    bits.reserve(34);
    for (int i = 0; i < 34; i++) {
        bits.push_back(dis(gen));
    }
    
    return bits;
}

/**
 * @brief Вывод битов в консоль
 */
void printBits(const std::vector<uint8_t>& bits, const std::string& title) {
    std::cout << title << " (" << bits.size() << " бит): ";
    for (size_t i = 0; i < bits.size(); i++) {
        std::cout << (int)bits[i];
        if ((i + 1) % 8 == 0 && i + 1 < bits.size()) {
            std::cout << " ";  // разделяем байты для читаемости
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Вывод байтов в консоль (hex)
 */
void printBytes(const std::vector<uint8_t>& bytes, const std::string& title) {
    std::cout << title << " (" << bytes.size() << " байт): ";
    for (uint8_t b : bytes) {
        printf("%02X ", b);
    }
    std::cout << std::endl;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    
#ifdef _WIN32
    // Включаем поддержку UTF-8 в консоли (только для Windows)
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::setlocale(LC_ALL, ".UTF-8");

    // Меняем режим консоли на UTF-8
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif


    // getNpssSequence() возвращает ссылку на static const массив
    // внутри функции, он живёт до конца программы.
    const auto& npss_seq = NpssGenerator().getNpssSequence();
    auto dest = npss_seq;

    Correlator correlator;
    const auto& npss_correlation = correlator.cyclic_autocorrelation(dest);

    for (size_t i = 0; i < npss_seq.size(); i++) {
        std::cout << "NPSS[" << i << "] = " << npss_seq[i] << std::endl;
    }

    for (size_t i = 0; i < npss_correlation.size(); i++) {
        std::cout << "Corr[" << i << "] = " << npss_correlation[i] << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
        // Или в секундах (с долями)
    std::chrono::duration<double> sec = end - start;
    std::cout << "Время выполнения: " << sec.count() << " с" << std::endl;
    return 0;
}


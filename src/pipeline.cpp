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
//#include "misc/NpssFileReader.h"
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


    /*
    NpssFileReader reader;
    if (reader.loadFromFile(INPUT_DIR "matrix_d_output.txt")) {
        std::cout << "NPSS последовательность успешно загружена!" << std::endl;

    const auto& sequence = reader.getSequence();
        //std::cout << "Первый элемент: " << sequence[0].real() 
        //<< " + " << sequence[0].imag() << "i" << std::endl;

        for (size_t i = 0; i < sequence.size(); i++) {
        std::cout << "Seq[" << i << "] = " << sequence[i]-npss_seq[i] << std::endl;
    }

    } else {
        std::cerr << "Ошибка загрузки: " << reader.getLastError() << std::endl;
        return 1;
    }
    
    // Константы для генерации NSSS
    for (size_t ncell_id = 0; ncell_id <= 503; ncell_id++) {
        for (size_t frame_id = 0; frame_id <= 7; frame_id += 2) {
            NsssGenerator nsss_gen(ncell_id, frame_id);
    
            const auto& nsss_seq = nsss_gen.getNsssSequence();
            //const auto& matrix = nsss_gen.getNsssMatrix();
            //std::cout << "\nNSSS Matrix 12x11:" << std::endl;
            //std::cout << "matrix[0][0] = " << matrix[0][0].real() 
            //          << " + " << matrix[0][0].imag() << "i" << std::endl;
            
            for (size_t i = 0; i < nsss_seq.size(); i++) {
                std::cout << "NNCellID[" << ncell_id << "]" << " FrameID[" << frame_id << "]" << " NSSS[" << i << "] = " << nsss_seq[i] << std::endl;
            }
        }
    }
    */
    

    std::cout << std::endl;
    std::cout << "=== NB-IoT CRC-16 Demo (NPBCH: 34 бит -> 50 бит) ===" << std::endl;
    std::cout << std::endl;
    
    // ========== ШАГ 1: Генерация случайных 34 бит ==========
    std::vector<uint8_t> input_bits = generateRandom34Bits();
    printBits(input_bits, "Входные данные (34 бита)");
    std::cout << std::endl;
    
    // ========== ШАГ 2: Конвертация 34 бит в 5 байт ==========
    std::vector<uint8_t> input_bytes = bits34ToBytes(input_bits);
    printBytes(input_bytes, "Упакованные данные (5 байт, последний обнулён)");
    std::cout << std::endl;
    
    // ========== ШАГ 3: Вычисление CRC и добавление ==========
    std::vector<uint8_t> output_bytes = input_bytes;
    CRC16::append(output_bytes);  // добавляем 2 байта CRC
    
    printBytes(output_bytes, "Данные + CRC (7 байт = 50+ бит)");
    std::cout << "CRC значение: 0x" << std::hex << CRC16::lastCRC() << std::dec << std::endl;
    std::cout << std::endl;
    
    // ========== ШАГ 4: Конвертация в биты для наглядности ==========
    std::vector<uint8_t> output_bits = bytesToBits(output_bytes);
    printBits(output_bits, "Итоговая последовательность (50+ бит)");
    std::cout << std::endl;
    
    // ========== ШАГ 5: Верификация CRC ==========
    bool valid = CRC16::verify(output_bytes);
    std::cout << "=== ВЕРИФИКАЦИЯ CRC ===" << std::endl;
    std::cout << "Результат: " << (valid ? "ПРОЙДЕНА" : "НЕ ПРОЙДЕНА") << std::endl;
    std::cout << std::endl;
    


    auto end = std::chrono::high_resolution_clock::now();
        // Или в секундах (с долями)
    std::chrono::duration<double> sec = end - start;
    std::cout << "Время выполнения: " << sec.count() << " с" << std::endl;
    return 0;
}


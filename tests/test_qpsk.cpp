
#include <gtest/gtest.h>
#include "code/QPSK.h"
#include "misc/BitUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <complex>
#include <sstream>
#include <iomanip>

TEST(test_QPSK, AllSequences) {
    std::string pathScrambled = std::string(INPUT_DIR) + "random_34bit_scrambled.txt";
    std::string pathQPSK = std::string(INPUT_DIR) + "qpsk_symbols_100.txt";
    
    std::ifstream fscrambled(pathScrambled);
    std::ifstream fqpsk(pathQPSK);
    
    ASSERT_TRUE(fscrambled.is_open()) << "Файл не найден: " << pathScrambled;
    ASSERT_TRUE(fqpsk.is_open()) << "Файл не найден: " << pathQPSK;
    
    std::string scrambledLine;
    int failed = 0;
    int lineCount = 0;
    int totalSequences = 100;
    
    // Векторы для хранения первых 5 строк
    std::vector<std::string> first5_first10bits;
    std::vector<std::string> first5_expected;
    std::vector<std::string> first5_result;
    std::vector<bool> first5_status;
    
    // Пропускаем заголовок в qpsk файле
    std::string header;
    std::getline(fqpsk, header);
    
    while (std::getline(fscrambled, scrambledLine) && lineCount < totalSequences) {
        if (!scrambledLine.empty() && scrambledLine.back() == '\r') scrambledLine.pop_back();
        if (scrambledLine.length() != 1600) continue;
        
        lineCount++;
        
        // Конвертируем строку в биты
        auto bits = BitUtils::stringToBitVector(scrambledLine);
        
        // QPSK модуляция
        NBIoTQPSK qpsk(bits);
        auto symbols = qpsk.getSymbols();
        
        // Проверка длины
        EXPECT_EQ(symbols.size(), 800) << "Строка " << lineCount << " имеет " << symbols.size() << " символов";
        
        // Проверка мощности
        float power = 0.0f;
        for (const auto& sym : symbols) {
            power += std::norm(sym);
        }
        power /= symbols.size();
        EXPECT_NEAR(power, 1.0f, 1e-6) << "Строка " << lineCount << " имеет мощность " << power;
        
        // Проверка созвездия
        bool validConstellation = true;
        for (const auto& sym : symbols) {
            float real = std::abs(sym.real());
            float imag = std::abs(sym.imag());
            if (std::abs(real - 0.70710678f) > 1e-6 && std::abs(imag - 0.70710678f) > 1e-6) {
                validConstellation = false;
                break;
            }
        }
        EXPECT_TRUE(validConstellation) << "Строка " << lineCount << " содержит неверные символы созвездия";
        
        // Сохраняем первые 5 строк для вывода
        if (lineCount <= 5) {
            // Первые 10 бит входной последовательности
            std::string first10bits = scrambledLine.substr(0, 10);
            first5_first10bits.push_back(first10bits);
            
            // Читаем ожидаемые символы из файла (первые 3)
            std::string expectedStr = "";
            for (int i = 0; i < 3; i++) {
                std::string line;
                if (std::getline(fqpsk, line)) {
                    // Парсим строку: "seq, sym, real, imag"
                    std::stringstream ss(line);
                    std::string token;
                    std::vector<std::string> parts;
                    while (std::getline(ss, token, ',')) {
                        parts.push_back(token);
                    }
                    if (parts.size() >= 4) {
                        float real = std::stof(parts[2]);
                        float imag = std::stof(parts[3]);
                        std::ostringstream oss;
                        oss << std::fixed << std::setprecision(4);
                        if (imag >= 0) {
                            oss << real << " + " << imag << "i";
                        } else {
                            oss << real << " - " << std::abs(imag) << "i";
                        }
                        if (i > 0) expectedStr += ", ";
                        expectedStr += oss.str();
                    }
                }
            }
            first5_expected.push_back(expectedStr);
            
            // Вычисленные символы (первые 3)
            std::string resultStr = "";
            for (int i = 0; i < 3 && i < (int)symbols.size(); i++) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(4);
                float real = symbols[i].real();
                float imag = symbols[i].imag();
                if (imag >= 0) {
                    oss << real << " + " << imag << "i";
                } else {
                    oss << real << " - " << std::abs(imag) << "i";
                }
                if (i > 0) resultStr += ", ";
                resultStr += oss.str();
            }
            first5_result.push_back(resultStr);
            
            // Статус
            bool status = (symbols.size() == 800 && 
                          std::abs(power - 1.0f) < 1e-6 && 
                          validConstellation);
            first5_status.push_back(status);
            
            if (!status) failed++;
        }
    }
    
    fscrambled.close();
    fqpsk.close();
    
    // ========== ВЫВОДИМ ИТОГ ==========
    std::cout << "\n=== ИТОГ ===\n";
    std::cout << "Всего строк: " << lineCount << "\n";
    std::cout << "Не прошло: " << failed << "\n";
    std::cout << "Прошло: " << (lineCount - failed) << "\n";
    if (lineCount > 0) {
        std::cout << "Процент: " << (100.0 * (lineCount - failed) / lineCount) << "%\n";
    }
    
    // ========== ПЕРВЫЕ 5 СТРОК ==========
    std::cout << "\n=== ПЕРВЫЕ 5 РЕЗУЛЬТАТОВ QPSK ===\n";
    for (size_t i = 0; i < first5_first10bits.size(); i++) {
        std::cout << "\n[" << i+1 << "]\n";
        std::cout << "  Вход (первые 10 бит): " << first5_first10bits[i] << "...\n";
        std::cout << "  Ожидается (первые 3 символа): " << first5_expected[i] << "\n";
        std::cout << "  Вычислено (первые 3 символа): " << first5_result[i] << "\n";
        std::cout << "  Статус:  " << (first5_status[i] ? "OK" : "FAIL") << "\n";
        
        if (!first5_status[i]) {
            std::cout << "  Ошибка: неверное количество символов или мощность\n";
        }
    }
}
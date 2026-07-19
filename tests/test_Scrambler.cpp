#include <gtest/gtest.h>
#include "code/Scrambler.h"
#include "misc/BitUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

TEST(test_Scrambler, AllSequences) {
    std::string pathInput = std::string(INPUT_DIR) + "random_34bit_ratematcher.txt";
    std::string pathScrambled = std::string(INPUT_DIR) + "random_34bit_scrambled.txt";
    
    std::ifstream fin(pathInput);
    std::ifstream fscrambled(pathScrambled);
    
    ASSERT_TRUE(fin.is_open()) << "Файл не найден: " << pathInput;
    ASSERT_TRUE(fscrambled.is_open()) << "Файл не найден: " << pathScrambled;
    
    std::string inputLine, expectedLine;
    int failed = 0;
    int lineCount = 0;
    int totalSequences = 500;
    int startNCellID = 1;
    int endNCellID = 20;
    
    // Читаем входные строки
    std::vector<std::string> inputLines;
    while (std::getline(fin, inputLine) && inputLines.size() < totalSequences) {
        if (!inputLine.empty() && inputLine.back() == '\r') inputLine.pop_back();
        if (inputLine.length() == 1600) {
            inputLines.push_back(inputLine);
        }
    }
    fin.close();
    
    // Читаем ожидаемые строки
    std::vector<std::string> expectedLines;
    while (std::getline(fscrambled, expectedLine)) {
        if (!expectedLine.empty() && expectedLine.back() == '\r') expectedLine.pop_back();
        if (expectedLine.length() == 1600) {
            expectedLines.push_back(expectedLine);
        }
    }
    fscrambled.close();
    
    // Проверяем, что количество ожидаемых строк соответствует
    size_t expectedTotal = inputLines.size() * (endNCellID - startNCellID + 1);
    if (expectedLines.size() != expectedTotal) {
        std::cout << "Ожидается " << expectedTotal << " строк, получено " << expectedLines.size() << "\n";
    }
    
    // Векторы для хранения первых 5 строк
    std::vector<std::string> first5_input;
    std::vector<std::string> first5_expected;
    std::vector<std::string> first5_result;
    std::vector<int> first5_ncell;
    std::vector<bool> first5_status;
    
    for (int ncell = startNCellID; ncell <= endNCellID; ncell++) {
        for (size_t i = 0; i < inputLines.size(); i++) {
            lineCount++;
            
            auto bits = BitUtils::stringToBitVector(inputLines[i]);
            NBIoTScrambler scrambler(bits, static_cast<uint32_t>(ncell), "NPBCH");
            
            auto result = scrambler.getScrambledBits();
            std::string result_str = BitUtils::bitVectorToString(result);
            
            std::string expected = (lineCount <= (int)expectedLines.size()) ? expectedLines[lineCount - 1] : "";
            
            // Сохраняем первые 5 строк
            if (lineCount <= 5) {
                first5_input.push_back(inputLines[i]);
                first5_expected.push_back(expected);
                first5_result.push_back(result_str);
                first5_ncell.push_back(ncell);
                first5_status.push_back(result_str == expected);
            }
            
            if (result_str != expected) {
                failed++;
            }
        }
    }
    
    // ========== ВЫВОДИМ ИТОГ ==========
    std::cout << "\n=== ИТОГ ===\n";
    std::cout << "Всего строк: " << lineCount << "\n";
    std::cout << "Не прошло: " << failed << "\n";
    std::cout << "Прошло: " << (lineCount - failed) << "\n";
    if (lineCount > 0) {
        std::cout << "Процент: " << (100.0 * (lineCount - failed) / lineCount) << "%\n";
    }
    
    // ========== ПЕРВЫЕ 5 СТРОК ==========
    std::cout << "\n=== ПЕРВЫЕ 5 РЕЗУЛЬТАТОВ SCRAMBLER ===\n";
    for (size_t i = 0; i < first5_input.size(); i++) {
        std::cout << "\n[" << i+1 << "]\n";
        std::cout << "  NCellID: " << first5_ncell[i] << "\n";
        std::cout << "  Вход (1600 бит): " << first5_input[i].substr(0, 10) << "..." << first5_input[i].substr(first5_input[i].length() - 10) << "\n";
        
        std::string first10_expected = first5_expected[i].substr(0, 10);
        std::string last10_expected = first5_expected[i].substr(first5_expected[i].length() - 10);
        std::string first10_result = first5_result[i].substr(0, 10);
        std::string last10_result = first5_result[i].substr(first5_result[i].length() - 10);
        
        std::cout << "  Ожидается: " << first10_expected << "..." << last10_expected << "\n";
        std::cout << "  Вычислено: " << first10_result << "..." << last10_result << "\n";
        std::cout << "  Статус:  " << (first5_status[i] ? "OK" : "FAIL") << "\n";
        
        if (!first5_status[i]) {
            for (size_t j = 0; j < 1600; j++) {
                if (first5_result[i][j] != first5_expected[i][j]) {
                    std::cout << "  Первая ошибка на позиции " << j << "\n";
                    std::cout << "  Ожидалось: " << first5_expected[i][j] << ", получено: " << first5_result[i][j] << "\n";
                    break;
                }
            }
        }
    }
}
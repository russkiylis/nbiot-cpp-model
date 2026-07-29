
#include "code/tbcc.h"
#include <cstddef>
#include <stdexcept>
// Возможно надо рассмотреть иной вариант, без отдельных таблиц под шесть итераций
// Кроме того, сам формат записи 50х3 должен быть задан явно, либо 50 после 50 после 50
// либо смежно - 1, 1, 1, 2, 2.. - огромная роль для тяжелого рейт-матчера

std::vector<uint8_t> TBCC::stringToBits(const std::string& str) {
    std::vector<uint8_t> bits;
    bits.reserve(str.length());
    for (char c : str) {
        bits.push_back(c == '1' ? 1 : 0);
    }
    return bits;
}

std::string TBCC::bitsToString(const std::vector<uint8_t>& bits) {
    std::string str;
    str.reserve(bits.size());
    for (uint8_t bit : bits) {
        str.push_back(bit ? '1' : '0');
    }
    return str;
}

std::vector<uint8_t> TBCC::encode(const std::vector<uint8_t>& input) {
    // Проверка размера
    if (input.size() != 50) {
        return std::vector<uint8_t>();
    }
    
    // Выходной массив: 150 бит (3 потока по 50)
    std::vector<uint8_t> output;
    output.reserve(150);
    
    // 6 регистров сдвига (индексы с 0)
    // Инициализация: последние 6 бит входного массива
    // MATLAB: s = [50, 49, 48, 47, 46, 45] (индексы с 1)
    // C++:    s = [49, 48, 47, 46, 45, 44] (индексы с 0)
    std::vector<int> s = {49, 48, 47, 46, 45, 44};
    
    // Основной цикл
    for (int i = 0; i < 50; i++) {
        // Полиномы (3GPP TS 36.212):
        // G1 = 133 (8) = 1 0 1 1 0 1
        // G2 = 171 (8) = 1 1 1 1 0 1
        // G3 = 165 (8) = 1 1 0 1 0 1
        
        // d1 = arr(i) + arr(s[1]) + arr(s[2]) + arr(s[4]) + arr(s[5])
        //      ↑        ↑            ↑            ↑            ↑
        //     текущий   регистр 2    регистр 3    регистр 5    регистр 6
        uint8_t d1 = input[i] ^ input[s[1]] ^ input[s[2]] ^ input[s[4]] ^ input[s[5]];
        
        // d2 = arr(i) + arr(s[0]) + arr(s[1]) + arr(s[2]) + arr(s[5])
        //      ↑        ↑            ↑            ↑            ↑
        //     текущий   регистр 1    регистр 2    регистр 3    регистр 6
        uint8_t d2 = input[i] ^ input[s[0]] ^ input[s[1]] ^ input[s[2]] ^ input[s[5]];
        
        // d3 = arr(i) + arr(s[0]) + arr(s[1]) + arr(s[3]) + arr(s[5])
        //      ↑        ↑            ↑            ↑            ↑
        //     текущий   регистр 1    регистр 2    регистр 4    регистр 6
        uint8_t d3 = input[i] ^ input[s[0]] ^ input[s[1]] ^ input[s[3]] ^ input[s[5]];
        
        // Сохраняем в порядке: d1, d2, d3
        output.push_back(d1);
        output.push_back(d2);
        output.push_back(d3);
        
        // Сдвиг регистров
        if (i < 6) {
            // Первые 6 итераций: специальный сдвиг
            // MATLAB: s = s + 1; s(1:i+1) = i+1:-1:1;
            // Пересчитываем все индексы
            // Новый s[0] = i (текущий индекс)
            // Новый s[1] = старый s[0]
            // Новый s[2] = старый s[1]
            // ...
            // Новый s[5] = старый s[4]
            
            // Старые значения
            int old_s0 = s[0];
            int old_s1 = s[1];
            int old_s2 = s[2];
            int old_s3 = s[3];
            int old_s4 = s[4];
            int old_s5 = s[5];
            
            // Новые значения
            s[0] = i;       // текущий индекс (i+1 в MATLAB → i в C++)
            s[1] = old_s0;
            s[2] = old_s1;
            s[3] = old_s2;
            s[4] = old_s3;
            s[5] = old_s4;
            
            // Для первых итераций, где ещё нет старых значений,
            // используем индексы из конца массива
            if (i == 0) {
                // i=0: [0, 49, 48, 47, 46, 45]
                s[0] = 0;
                s[1] = 49;
                s[2] = 48;
                s[3] = 47;
                s[4] = 46;
                s[5] = 45;
            } else if (i == 1) {
                // i=1: [1, 0, 49, 48, 47, 46]
                s[0] = 1;
                s[1] = 0;
                s[2] = 49;
                s[3] = 48;
                s[4] = 47;
                s[5] = 46;
            } else if (i == 2) {
                // i=2: [2, 1, 0, 49, 48, 47]
                s[0] = 2;
                s[1] = 1;
                s[2] = 0;
                s[3] = 49;
                s[4] = 48;
                s[5] = 47;
            } else if (i == 3) {
                // i=3: [3, 2, 1, 0, 49, 48]
                s[0] = 3;
                s[1] = 2;
                s[2] = 1;
                s[3] = 0;
                s[4] = 49;
                s[5] = 48;
            } else if (i == 4) {
                // i=4: [4, 3, 2, 1, 0, 49]
                s[0] = 4;
                s[1] = 3;
                s[2] = 2;
                s[3] = 1;
                s[4] = 0;
                s[5] = 49;
            } else if (i == 5) {
                // i=5: [5, 4, 3, 2, 1, 0]
                s[0] = 5;
                s[1] = 4;
                s[2] = 3;
                s[3] = 2;
                s[4] = 1;
                s[5] = 0;
            }
        } else {
            // Начиная с i >= 6: обычный сдвиг вправо
            // MATLAB: s = s + 1 (все индексы увеличиваются на 1)
            // В C++: все индексы увеличиваем на 1
            s[0] += 1;
            s[1] += 1;
            s[2] += 1;
            s[3] += 1;
            s[4] += 1;
            s[5] += 1;
        }
    }
    
    return output;
}

std::string TBCC::encode(const std::string& input) {
    auto bits = stringToBits(input);
    auto result = encode(bits);
    if (result.empty()) {
        return "";
    }
    return bitsToString(result);
}
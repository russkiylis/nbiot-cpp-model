
#pragma once

#include <vector>
#include <cstdint>
#include <cmath>

/**
 * @brief Класс для Rate Matching (согласование скоростей)
 *        Используется в NB-IoT/LTE для канала NBPCH
 *        Вход: 150 бит (3 потока по 50)
 *        Выход: 1600 бит
 */
class RateMatcher {
public:
    /**
     * @brief Конструктор
     */
    RateMatcher();
    
    /**
     * @brief Деструктор
     */
    ~RateMatcher() = default;
    
    /**
     * @brief Полный Rate Matching
     * @param input Входной массив [3 × 50] (биты 0/1)
     * @param outputLength Выходная длина (1600 для NBPCH)
     * @return Выходная последовательность (биты 0/1)
     */
    std::vector<uint8_t> rateMatch(const std::vector<std::vector<uint8_t>>& input, 
                                    int outputLength = 1600);
    
    /**
     * @brief Перемежение (Interleaving)
     * @param arr Входной массив [3 × 50]
     * @return Перемежённый массив [3 × row_num × 32] с NaN в пустых позициях
     */
    std::vector<std::vector<std::vector<float>>> interleave(
        const std::vector<std::vector<uint8_t>>& arr);
    
    /**
     * @brief Выбор битов (Bit Selection)
     * @param interSeq Перемежённый массив [3 × row_num × 32]
     * @param length Выходная длина (1600)
     * @return Выходная последовательность (биты 0/1)
     */
    std::vector<uint8_t> bitSelection(
        const std::vector<std::vector<std::vector<float>>>& interSeq,
        int length);

private:
    static constexpr int PERM_PAT[32] = {
        2, 18, 10, 26, 6, 22, 14, 30,
        4, 20, 12, 28, 8, 24, 16, 32,
        1, 17, 9, 25, 5, 21, 13, 29,
        3, 19, 11, 27, 7, 23, 15, 31
    };
    
    bool isNaN(float val) const;
};
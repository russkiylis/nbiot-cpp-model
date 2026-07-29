
#include "code/RateMatcher.h"
#include <cmath>
#include <cstddef>
#include <algorithm>

RateMatcher::RateMatcher() {}

bool RateMatcher::isNaN(float val) const {
    return std::isnan(val);
}

std::vector<std::vector<std::vector<float>>> RateMatcher::interleave(
    const std::vector<std::vector<uint8_t>>& arr) {
    
    int numStreams = static_cast<int>(arr.size());      // 3
    int lenArr = static_cast<int>(arr[0].size());       // 50
    int rowNum = static_cast<int>(std::ceil(lenArr / 32.0));  // 2
    int lenSeq = rowNum * 32;                           // 64
    int dummySize = lenSeq - lenArr;                    // 14
    
    // Промежуточный массив [3 × 64]
    std::vector<std::vector<float>> tmp(numStreams, std::vector<float>(lenSeq, 0.0f));
    
    // Добавляем NaN в начало
    for (int s = 0; s < numStreams; s++) {
        for (int i = 0; i < dummySize; i++) {
            tmp[s][i] = NAN;
        }
        for (int i = 0; i < lenArr; i++) {
            tmp[s][dummySize + i] = static_cast<float>(arr[s][i]);
        }
    }
    
    // Перестраиваем в [3 × row_num × 32]
    std::vector<std::vector<std::vector<float>>> reshaped(
        numStreams,
        std::vector<std::vector<float>>(rowNum, std::vector<float>(32, 0.0f))
    );
    
    for (int s = 0; s < numStreams; s++) {
        int idx = 0;
        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < 32; c++) {
                reshaped[s][r][c] = tmp[s][idx++];
            }
        }
    }
    
    // Перестановка столбцов согласно perm_pat
    std::vector<std::vector<std::vector<float>>> result(
        numStreams,
        std::vector<std::vector<float>>(rowNum, std::vector<float>(32, 0.0f))
    );
    
    for (int s = 0; s < numStreams; s++) {
        for (int r = 0; r < rowNum; r++) {
            for (int c = 0; c < 32; c++) {
                int newC = PERM_PAT[c] - 1;  // -1 для индексации с 0
                result[s][r][c] = reshaped[s][r][newC];
            }
        }
    }
    
    return result;
}

std::vector<uint8_t> RateMatcher::bitSelection(
    const std::vector<std::vector<std::vector<float>>>& interSeq,
    int length) {
    
    int numStreams = static_cast<int>(interSeq.size());      // 3
    int rowNum = static_cast<int>(interSeq[0].size());       // 2
    int colNum = static_cast<int>(interSeq[0][0].size());    // 32
    
    std::vector<uint8_t> result;
    result.reserve(length);
    
    int s = 0, r = 0, c = 0;
    
    while (static_cast<int>(result.size()) < length) {
        float val = interSeq[s][r][c];
        
        // Пропускаем NaN
        if (!isNaN(val)) {
            result.push_back(static_cast<uint8_t>(val));
        }
        
        // Переход к следующему элементу
        r++;
        if (r >= rowNum) {
            r = 0;
            c++;
            if (c >= colNum) {
                c = 0;
                s++;
                if (s >= numStreams) {
                    s = 0;
                    r = 0;
                    c = 0;
                }
            }
        }
    }
    
    return result;
}

std::vector<uint8_t> RateMatcher::rateMatch(
    const std::vector<std::vector<uint8_t>>& input,
    int outputLength) {
    
    // Шаг 1: Перемежение
    auto interleaved = interleave(input);
    
    // Шаг 2: Выбор битов
    return bitSelection(interleaved, outputLength);
}
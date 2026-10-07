#pragma once
#include "ResourceGrid.h"
#include "NpssGenerator.h"

namespace nbiot {

class SubframeGenerator {
public:
    SubframeGenerator();
    
    // Генерирует сабфрейм. Если subframeIndex == 5, размещает NPSS, иначе возвращает пустую сетку
    SubframeGrid generateSubframe(size_t subframeIndex) const;

private:
    NpssGenerator npssGenerator_;
};

} // namespace nbiot
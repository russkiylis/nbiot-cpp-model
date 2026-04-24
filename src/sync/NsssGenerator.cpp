/**
 * @file NsssGenerator.cpp
 * @brief Реализация генератора NSSS последовательности
 * @authors russkiylis, Tobyret101
 * @date 19.04.2026
 */

#include "sync/NsssGenerator.h"
#include <cmath>
#include <algorithm>

// Статическая таблица b (из MATLAB кода)
constexpr std::array<std::array<int, 128>, 4> NsssGenerator::b_table_ = {{
    // b(1,:) - все единицы
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    
    // b(2,:)
    {1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1,
     1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1,
     1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1,
     1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1},
    
    // b(3,:)
    {1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1,
     -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1,
     1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1,
     -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1},
    
    // b(4,:)
    {1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1,
     -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1,
     -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1,
     1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1}
}};

NsssGenerator::NsssGenerator(size_t ncell_id, size_t frame_id)
    : is_initialized_(false)
    , ncell_id_(ncell_id)
    , frame_id_(frame_id)
{
    // Инициализация нулями
    sequence_.fill(std::complex<float>(0.0f, 0.0f));
    for (auto& row : matrix_) {
        row.fill(std::complex<float>(0.0f, 0.0f));
    }
}

NsssGenerator::~NsssGenerator() = default;

const std::array<std::complex<float>, 132>& NsssGenerator::getNsssSequence() const {
    if (!is_initialized_) {
        init();
    }
    return sequence_;
}

const std::array<std::array<std::complex<float>, 11>, 12>& NsssGenerator::getNsssMatrix() const {
    if (!is_initialized_) {
        init();
    }
    return matrix_;
}

void NsssGenerator::init() const {
    static constexpr float PI_F = 3.14159265358979323846f;
    
    // Параметры из MATLAB кода
    size_t u = (ncell_id_ % 126) + 3;           // u = mod(NCellID, 126) + 3
    size_t q = ncell_id_ / 126;                 // q = floor(NCellID / 126)
    
    // q должно быть в диапазоне 0-3
    if (q < 0) q = 0;
    if (q > 3) q = 3;
    
    // Генерация последовательности d (132 элемента)
    // static const + лямбда для однократной генерации (но зависит от параметров)
    // Поэтому используем обычную генерацию, так как параметры могут меняться
    
    std::array<std::complex<float>, 132> d{};
    
    for (int idx = 0; idx < 132; ++idx) {
        int n = idx;                    // n = 0:131
        int n_ = n % 131;               // n_ = mod(n, 131)
        int m = n % 128;                // m = mod(n, 128)
        
        // b(q+1, m+1) - индексация с 1 в MATLAB, в C++ с 0
        float b_val = static_cast<float>(b_table_[q][m]);
        
        // exp(-1i * pi * u * n_ * (n_ + 1) / 131)
        float phase = -PI_F * static_cast<float>(u) * static_cast<float>(n_) * static_cast<float>(n_ + 1) / 131.0f;
        std::complex<float> zc_part = std::exp(std::complex<float>(0.0f, phase));
        
        // d = b * exp(...)
        d[idx] = b_val * zc_part;
        
        // Добавление циклического сдвига, зависящего от frameID
        // cyclic_shift = mod((33/132) * (frameID/2), 4)
        float cyclic_shift = std::fmod((33.0f / 132.0f) * (static_cast<float>(frame_id_) / 2.0f), 4.0f);
        
        // d = d * exp(-1i * 2 * pi * n * cyclic_shift)
        float shift_phase = -2.0f * PI_F * static_cast<float>(n) * cyclic_shift;
        std::complex<float> shift_factor = std::exp(std::complex<float>(0.0f, shift_phase));
        
        d[idx] = d[idx] * shift_factor;
    }
    
    // Преобразование вектора d в матрицу 12x11
    // В MATLAB: while ~isempty(d) cusochek = d(1:12)'; d = d(13:end); d_ = [d_ cusochek]; end
    // То есть: первый столбец - первые 12 элементов, второй столбец - следующие 12 и т.д.
    
    std::array<std::array<std::complex<float>, 11>, 12> temp_matrix{};
    
    for (int col = 0; col < 11; ++col) {          // 11 столбцов
        for (int row = 0; row < 12; ++row) {      // 12 строк
            int idx = col * 12 + row;             // Индекс в исходном векторе
            if (idx < 132) {
                temp_matrix[row][col] = d[idx];
            }
        }
    }
    
    // Сохраняем результат
    sequence_ = d;
    matrix_ = temp_matrix;
    is_initialized_ = true;
}
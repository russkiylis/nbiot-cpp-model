#pragma once

#include <vector>
#include <complex>
#include <cstdint>
#include <string>

/**
 * @brief Класс для QPSK модуляции в NB-IoT
 *        Вход: биты (0/1) → выход: комплексные символы
 *        Использует нормализацию 1/√2 (как в стандарте 3GPP)
 *        Для каналов NPDSCH, NPDCCH, NPBCH
 */
class NBIoTQPSK {
public:
    /**
     * @brief Конструктор
     * @param bits Входные биты (0/1) — длина должна быть чётной
     */
    NBIoTQPSK(const std::vector<uint8_t>& bits);
    
    /**
     * @brief Конструктор из строки битов
     * @param bitsStr Строка из '0' и '1'
     */
    NBIoTQPSK(const std::string& bitsStr);
    
    /**
     * @brief Деструктор
     */
    ~NBIoTQPSK() = default;
    
    /**
     * @brief Получить модулированные символы
     */
    const std::vector<std::complex<float>>& getSymbols() const { return symbols_; }
    
    /**
     * @brief Получить количество символов
     */
    size_t getSymbolCount() const { return symbols_.size(); }
    
    /**
     * @brief Получить количество битов
     */
    size_t getBitCount() const { return bits_.size(); }
    
    /**
     * @brief Статический метод для прямой модуляции
     */
    static std::vector<std::complex<float>> modulate(const std::vector<uint8_t>& bits);
    
    /**
     * @brief Статический метод для прямой модуляции из строки
     */
    static std::vector<std::complex<float>> modulate(const std::string& bitsStr);
    
    /**
     * @brief Конвертировать символы в строку для вывода
     */
    static std::string symbolsToString(const std::vector<std::complex<float>>& symbols, 
                                        int precision = 6);

private:
    std::vector<uint8_t> bits_;
    std::vector<std::complex<float>> symbols_;
    
    static constexpr float INV_SQRT2 = 0.7071067811865475f;  // 1/√2
    
    /**
     * @brief Внутренняя QPSK модуляция
     */
    std::vector<std::complex<float>> modQPSK();
    
    /**
     * @brief Конвертация строки в биты
     */
    std::vector<uint8_t> stringToBits(const std::string& str);
};
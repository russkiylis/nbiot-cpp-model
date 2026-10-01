
#pragma once

#include <vector>
#include <cstdint>
#include <string>

/**
 * @brief Класс для Tail-Biting Convolutional Code (TBCC, Encoding)
 *        Кодовая скорость 1/3, полиномы G1=133, G2=171, G3=165
 *        Используется в NB-IoT для канала NPBCH
 */
class TBCC {
public:
    /**
     * @brief Конструктор
     */
    TBCC() = default;
    
    /**
     * @brief Деструктор
     */
    ~TBCC() = default;
    
    /**
     * @brief Кодирование массива битов (Tail-Biting)
     * @param input Входной массив битов (0/1), длина 50
     * @return Выходной массив битов (0/1), длина 150 (3 × 50)
     */
    std::vector<uint8_t> encode(const std::vector<uint8_t>& input);
    
    /**
     * @brief Кодирование строки битов ('0'/'1')
     * @param input Строка из '0' и '1', длина 50
     * @return Строка из '0' и '1', длина 150
     */
    std::string encode(const std::string& input);
    
    /**
     * @brief Получить количество выходных бит
     */
    size_t getOutputSize() const { return 150; }
    
    /**
     * @brief Получить количество входных бит
     */
    size_t getInputSize() const { return 50; }
    
    /**
     * @brief Сброс состояния (если нужно)
     */
    void reset() {}

private:
    /**
     * @brief Конвертирует строку '0'/'1' в вектор битов
     */
    std::vector<uint8_t> stringToBits(const std::string& str);
    
    /**
     * @brief Конвертирует вектор битов в строку
     */
    std::string bitsToString(const std::vector<uint8_t>& bits);
};
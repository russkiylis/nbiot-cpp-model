#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <array>

/**
 * @brief Класс для скремблирования в NB-IoT/LTE
 *        Использует Gold Sequence с 31-битными регистрами
 *        Поддерживает NPBCH, NPDSCH, NPDCCH, NRS
 */
class NBIoTScrambler {
public:
    /**
     * @brief Конструктор
     * @param bits Входные биты (0/1)
     * @param c_init Начальное значение для Gold Sequence (31 бит)
     * @param signal_type Тип сигнала (NPBCH, NPDSCH, NPDCCH, NRS)
     */
    NBIoTScrambler(const std::vector<uint8_t>& bits, 
                   uint32_t c_init, 
                   const std::string& signal_type);
    
    /**
     * @brief Деструктор
     */
    ~NBIoTScrambler() = default;
    
    /**
     * @brief Скремблирование с удалением использованной части
     * @param bits Входные биты (0/1)
     * @return Скремблированные биты (0/1)
     */
    std::vector<uint8_t> scramble_seqsubstract(const std::vector<uint8_t>& bits);
    
    /**
     * @brief Получить скремблированную последовательность
     */
    const std::vector<uint8_t>& getScrambledBits() const { return scrambledBits_; }
    
    /**
     * @brief Получить скремблирующую последовательность
     */
    const std::vector<uint8_t>& getScramblingSequence() const { return scramblingSequence_; }
    
    /**
     * @brief Получить последнее вычисленное значение
     */
    uint32_t getLastCInit() const { return c_init_; }
    
    // ========== ОТЛАДОЧНЫЕ МЕТОДЫ ==========
    /**
     * @brief Получить первые 31 бит регистра x1 (для отладки)
     */
    std::array<uint8_t, 31> getX1() const { return x1_; }
    
    /**
     * @brief Получить первые 31 бит регистра x2 (для отладки)
     */
    std::array<uint8_t, 31> getX2() const { return x2_; }
    
    /**
     * @brief Получить первые 20 бит Gold Sequence (для отладки)
     */
    std::vector<uint8_t> getGoldSequenceFirst20() const;

private:
    static constexpr int NC = 1600;  // константа для LTE/NB-IoT
    
    std::vector<uint8_t> bits_;
    uint32_t c_init_;
    std::string signal_type_;
    
    std::vector<uint8_t> scramblingSequence_;
    std::vector<uint8_t> scrambledBits_;
    size_t sequenceOffset_{0};
    
    // Отладочные данные
    std::array<uint8_t, 31> x1_;
    std::array<uint8_t, 31> x2_;
    
    /**
     * @brief Генерация Gold Sequence (31-битный регистр)
     * @param length Длина последовательности
     * @param c_init Начальное значение для x2
     * @return Скремблирующая последовательность
     */
    std::vector<uint8_t> genGoldSequence(size_t length, uint32_t c_init);
    
    /**
     * @brief Инициализация x2 из c_init (MSB first)
     */
    std::array<uint8_t, 31> initX2(uint32_t c_init);
};
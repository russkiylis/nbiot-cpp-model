/**
 * @file NpssFileReader.h
 * @brief Класс для чтения NPSS последовательности из текстового файла
 * @authors russkiylis, Tobyret101
 * @date 12.04.2026
 * @version 1.0
 */

#ifndef NPSS_FILE_READER_H
#define NPSS_FILE_READER_H

#include <array>
#include <complex>
#include <string>
#include <vector>

class NpssFileReader {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    NpssFileReader();
    
    /**
     * @brief Деструктор
     */
    ~NpssFileReader();
    
    /**
     * @brief Загрузить NPSS последовательность из файла
     * @param filename Путь к файлу с комплексными числами
     * @return true если загрузка успешна, false в противном случае
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * @brief Получить NPSS последовательность в виде массива 121 элемента
     * @return Константная ссылка на массив комплексных чисел
     */
    const std::array<std::complex<float>, 121>& getSequence() const;
    
    /**
     * @brief Получить NPSS последовательность в виде матрицы 11x11
     * @return Константная ссылка на двумерный массив
     */
    const std::array<std::array<std::complex<float>, 11>, 11>& getMatrix() const;
    
    /**
     * @brief Проверить, успешно ли загружены данные
     * @return true если данные загружены, false если нет
     */
    bool isLoaded() const;
    
    /**
     * @brief Получить последнюю ошибку
     * @return Строка с описанием ошибки
     */
    std::string getLastError() const;

private:
    std::array<std::complex<float>, 121> sequence_;
    std::array<std::array<std::complex<float>, 11>, 11> matrix_;
    bool is_loaded_;
    std::string last_error_;
    
    /**
     * @brief Парсинг строки с комплексными числами
     * @param line Строка из файла
     * @return Вектор комплексных чисел
     */
    std::vector<std::complex<float>> parseLine(const std::string& line);
    
    /**
     * @brief Преобразовать вектор в массив и матрицу
     * @param vec Вектор из 121 комплексного числа
     * @return true если преобразование успешно
     */
    bool vectorToArray(const std::vector<std::complex<float>>& vec);
    
    /**
     * @brief Парсинг одного комплексного числа формата "a+bi" или "a-bi"
     * @param token Строка с комплексным числом
     * @return Комплексное число
     */
    std::complex<float> parseComplex(const std::string& token);
};

#endif // NPSS_FILE_READER_H
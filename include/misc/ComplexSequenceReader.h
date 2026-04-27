#ifndef COMPLEX_SEQUENCE_READER_H
#define COMPLEX_SEQUENCE_READER_H

#include <complex>
#include <vector>
#include <string>

class ComplexSequenceReader {
public:
    ComplexSequenceReader();
    ~ComplexSequenceReader();
    
    /**
     * @brief Загрузить последовательность комплексных чисел из файла
     * @param filename Путь к txt файлу
     * @return true если загрузка успешна
     */
    bool loadFromFile(const std::string& filename);
    
    /**
     * @brief Получить загруженную последовательность
     * @return Константная ссылка на вектор комплексных чисел
     */
    const std::vector<std::complex<float>>& getSequence() const;
    
    /**
     * @brief Получить количество элементов
     */
    size_t size() const;
    
    /**
     * @brief Проверить, загружены ли данные
     */
    bool isLoaded() const;
    
    /**
     * @brief Получить последнюю ошибку
     */
    std::string getLastError() const;
    
    /**
     * @brief Очистить загруженные данные
     */
    void clear();

private:
    std::vector<std::complex<float>> sequence_;
    bool is_loaded_;
    std::string last_error_;
    
    /**
     * @brief Парсинг строки с комплексными числами
     */
    void parseLine(const std::string& line);
    
    /**
     * @brief Парсинг одного комплексного числа формата "a+bi" или "a-bi"
     */
    std::complex<float> parseComplex(const std::string& token);
};

#endif
/**
 * @file NpssFileReader.cpp
 * @brief Реализация класса для чтения NPSS последовательности из файла
 * @authors russkiylis, Tobyret101
 * @date 12.04.2026
 */

#include "misc/NpssFileReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

NpssFileReader::NpssFileReader() 
    : is_loaded_(false) {
    // Инициализируем нулями на всякий случай
    sequence_.fill(std::complex<float>(0.0f, 0.0f));
    for (auto& row : matrix_) {
        row.fill(std::complex<float>(0.0f, 0.0f));
    }
}

NpssFileReader::~NpssFileReader() = default;

bool NpssFileReader::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        last_error_ = "Не удалось открыть файл: " + filename;
        is_loaded_ = false;
        return false;
    }
    
    std::string line;
    if (!std::getline(file, line)) {
        last_error_ = "Файл пуст: " + filename;
        file.close();
        is_loaded_ = false;
        return false;
    }
    file.close();
    
    // Парсим строку
    std::vector<std::complex<float>> parsed_data = parseLine(line);
    
    if (parsed_data.size() != 121) {
        last_error_ = "Ожидалось 121 комплексное число, получено " + 
                      std::to_string(parsed_data.size());
        is_loaded_ = false;
        return false;
    }
    
    // Преобразуем в массив и матрицу
    if (!vectorToArray(parsed_data)) {
        is_loaded_ = false;
        return false;
    }
    
    is_loaded_ = true;
    last_error_.clear();
    return true;
}

const std::array<std::complex<float>, 121>& NpssFileReader::getSequence() const {
    return sequence_;
}

const std::array<std::array<std::complex<float>, 11>, 11>& NpssFileReader::getMatrix() const {
    return matrix_;
}

bool NpssFileReader::isLoaded() const {
    return is_loaded_;
}

std::string NpssFileReader::getLastError() const {
    return last_error_;
}

std::vector<std::complex<float>> NpssFileReader::parseLine(const std::string& line) {
    std::vector<std::complex<float>> result;
    std::stringstream ss(line);
    std::string token;
    
    while (ss >> token) {
        // Убираем пробелы по краям
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        
        if (token.empty()) continue;
        
        result.push_back(parseComplex(token));
    }
    
    return result;
}

std::complex<float> NpssFileReader::parseComplex(const std::string& token) {
    std::string temp = token;
    
    // Удаляем 'i' в конце если есть
    if (!temp.empty() && temp.back() == 'i') {
        temp.pop_back();
    }
    
    // Ищем позицию разделителя между real и imag
    size_t pos = temp.find('+');
    if (pos == std::string::npos) {
        // Ищем минус, начиная с позиции 1 (чтобы не схватить минус в real части)
        pos = temp.find('-', 1);
    }
    
    if (pos != std::string::npos) {
        float real = std::stof(temp.substr(0, pos));
        float imag = std::stof(temp.substr(pos));
        return std::complex<float>(real, imag);
    }
    
    // Если разделитель не найден — возможно только действительное число
    float real = std::stof(temp);
    return std::complex<float>(real, 0.0f);
}

bool NpssFileReader::vectorToArray(const std::vector<std::complex<float>>& vec) {
    if (vec.size() != 121) {
        last_error_ = "Размер вектора не соответствует 121";
        return false;
    }
    
    // Копируем в массив
    std::copy(vec.begin(), vec.end(), sequence_.begin());
    
    // Преобразуем в матрицу 11x11 (по строкам)
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) {
            matrix_[i][j] = vec[i * 11 + j];
        }
    }
    
    return true;
}
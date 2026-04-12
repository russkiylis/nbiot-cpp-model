/**
 * @file pipeline.cpp
 * @brief Точка входа: генерация и вывод NPSS-последовательности.
 * Демонстрационный пайплайн для тестирования генератора NPSS-последовательности.
 *
 * @authors russkiylis, Tobyret101
 * @date 30.03.2026
 * @version 1.0
 */
 // 
#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>

#include "sync/NpssGenerator.h"
#include "misc/Correlator.h"
#include "misc/NpssFileReader.h"

int main() {
    
#ifdef _WIN32
    // Включаем поддержку UTF-8 в консоли (только для Windows)
    SetConsoleOutputCP(65001);
    
    // Меняем режим консоли на UTF-8
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif


    // getNpssSequence() возвращает ссылку на static const массив
    // внутри функции, он живёт до конца программы.
    const auto& npss_seq = NpssGenerator().getNpssSequence();
    auto dest = npss_seq;

    Correlator correlator;
    const auto& npss_correlation = correlator.cyclic_autocorrelation(dest);

    for (size_t i = 0; i < npss_seq.size(); i++) {
        std::cout << "NPSS[" << i << "] = " << npss_seq[i] << std::endl;
    }

    for (size_t i = 0; i < npss_correlation.size(); i++) {
        std::cout << "Corr[" << i << "] = " << npss_correlation[i] << std::endl;
    }

    NpssFileReader reader;
    
    if (reader.loadFromFile(INPUT_DIR "matrix_d_output.txt")) {
        std::cout << "NPSS последовательность успешно загружена!" << std::endl;

    const auto& sequence = reader.getSequence();
        //std::cout << "Первый элемент: " << sequence[0].real() 
        //<< " + " << sequence[0].imag() << "i" << std::endl;

        for (size_t i = 0; i < sequence.size(); i++) {
        std::cout << "Seq[" << i << "] = " << sequence[i]-npss_seq[i] << std::endl;
    }

    } else {
        std::cerr << "Ошибка загрузки: " << reader.getLastError() << std::endl;
        return 1;
    }

    return 0;
}

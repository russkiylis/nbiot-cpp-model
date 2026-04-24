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
#include <iomanip>

#include "sync/NpssGenerator.h"
#include "sync/NsssGenerator.h"
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



    // Константы для генерации NSSS
    for (size_t ncell_id = 0; ncell_id <= 503; ncell_id++) {
        for (size_t frame_id = 0; frame_id <= 7; frame_id++) {
            NsssGenerator nsss_gen(ncell_id, frame_id);
    
            const auto& nsss_seq = nsss_gen.getNsssSequence();
            //const auto& matrix = nsss_gen.getNsssMatrix();
            //std::cout << "\nNSSS Matrix 12x11:" << std::endl;
            //std::cout << "matrix[0][0] = " << matrix[0][0].real() 
            //          << " + " << matrix[0][0].imag() << "i" << std::endl;
            
            for (size_t i = 0; i < nsss_seq.size(); i++) {
                std::cout << "NSSS[" << i << "] = " << nsss_seq[i] << std::endl;
            }
        }
    }

    return 0;
}

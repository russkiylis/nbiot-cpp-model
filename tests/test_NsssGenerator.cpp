/**
 * @file test_NsssGenerator.cpp
 * @author russkiylis x Tobyret101
 * @brief Тестирование NSSS-генератора
 * @version 0.1
 * @date 03.05.2026
 * 
 */

#include <gtest/gtest.h>
#include "sync/NsssGenerator.h"
#include "misc/ComplexSequenceReader.h"

// Тест для проверки размерностей и первой последовательности
TEST(test_NsssGenerator, FileSizeAndFirstSequence){
    ComplexSequenceReader reader;
    const double tolerance = 8e-3;

    ASSERT_TRUE(reader.loadFromFile(INPUT_DIR "matrix_nsss_output.txt"));

    const auto& Read_sequence = reader.getSequence();
    
    // Проверка: размер файла должен быть кратен 132
    EXPECT_EQ(Read_sequence.size() % 132, 0)
            << "размер файла " << Read_sequence.size() 
            << " не кратен 132 (ожидается 504*4*132=" << 504*4*132 << ")";
    
    // Проверка: хотя бы одна последовательность есть
    ASSERT_GE(Read_sequence.size(), 132)
            << "файл слишком мал, содержит " << Read_sequence.size() << " элементов";
    
    // Проверка первой последовательности (ncell_id=0, frame_id=0)
    NsssGenerator generator(0, 0);
    const auto& Gen_sequence = generator.getNsssSequence();
    
    ASSERT_EQ(Gen_sequence.size(), 132)
            << "размер сгенерированной последовательности не 132, а " << Gen_sequence.size();
    
    // Сравнение первой последовательности
    for (size_t i = 0; i < Gen_sequence.size(); ++i) {
        EXPECT_NEAR(Gen_sequence[i].real(), Read_sequence[i].real(), tolerance)
                << "index=" << i << ", real part";
        EXPECT_NEAR(Gen_sequence[i].imag(), Read_sequence[i].imag(), tolerance)
                << "index=" << i << ", imag part";
    }
}

// Тест для полной проверки последовательности
TEST(test_NsssGenerator, AllCombos){
    ComplexSequenceReader reader;
    const double tolerance = 8e-3; // установка меньше чем 8e-3 - начинаются ошибки

    ASSERT_TRUE(reader.loadFromFile(INPUT_DIR "matrix_nsss_output.txt"));

    const auto& Read_sequence = reader.getSequence();
    
    // Ожидаемый размер
    size_t expected_combinations = 504 * 4;  // 2016 комбинаций
    size_t expected_size = expected_combinations * 132;
    
    ASSERT_EQ(Read_sequence.size(), expected_size)
            << "размер файла не соответствует ожидаемому: " 
            << Read_sequence.size() << " != " << expected_size;
    
    // Счетчики ошибок
    size_t total_errors = 0;
    size_t sequences_with_errors = 0;
    size_t max_errors_in_sequence = 0;
    size_t worst_sequence_ncell = 0;
    size_t worst_sequence_frame = 0;
    
    std::vector<std::tuple<size_t, size_t, size_t, float, float>> error_details;
    
    // Проходим по всем комбинациям
    for (size_t ncell_id = 0; ncell_id < 504; ++ncell_id) {
        for (size_t frame_id : {0, 2, 4, 6}) {
            NsssGenerator generator(ncell_id, frame_id);
            const auto& Gen_sequence = generator.getNsssSequence();
            
            size_t frame_idx = frame_id / 2;
            size_t start_pos = (ncell_id * 4 + frame_idx) * 132;
            
            size_t errors_in_sequence = 0;
            
            for (size_t i = 0; i < Gen_sequence.size(); ++i) {
                float real_diff = std::abs(Gen_sequence[i].real() - Read_sequence[start_pos + i].real());
                float imag_diff = std::abs(Gen_sequence[i].imag() - Read_sequence[start_pos + i].imag());
                
                bool real_ok = (real_diff <= tolerance);
                bool imag_ok = (imag_diff <= tolerance);
                
                if (!real_ok || !imag_ok) {
                    errors_in_sequence++;
                    total_errors++;
                    
                    // Сохраняем детали первых 10 ошибок для отладки
                    if (error_details.size() < 10) {
                        error_details.emplace_back(ncell_id, frame_id, i, real_diff, imag_diff);
                    }
                }
            }
            
            if (errors_in_sequence > 0) {
                sequences_with_errors++;
                if (errors_in_sequence > max_errors_in_sequence) {
                    max_errors_in_sequence = errors_in_sequence;
                    worst_sequence_ncell = ncell_id;
                    worst_sequence_frame = frame_id;
                }
            }
            
            // Проверка для Google Test (каждый элемент)
            for (size_t i = 0; i < Gen_sequence.size(); ++i) {
                EXPECT_NEAR(Gen_sequence[i].real(), Read_sequence[start_pos + i].real(), tolerance)
                        << "ncell_id=" << ncell_id << ", frame_id=" << frame_id << ", index=" << i;
                EXPECT_NEAR(Gen_sequence[i].imag(), Read_sequence[start_pos + i].imag(), tolerance)
                        << "ncell_id=" << ncell_id << ", frame_id=" << frame_id << ", index=" << i;
            }
        }
    }
    
    // Вывод статистики в конце теста
    std::cout << "\n========================================" << std::endl;
    std::cout << "=== СТАТИСТИКА СРАВНЕНИЯ NSSS ===" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Всего проверено комбинаций: " << expected_combinations << std::endl;
    std::cout << "Всего проверено элементов: " << expected_combinations * 132 << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Последовательностей с ошибками: " << sequences_with_errors << " / " << expected_combinations << std::endl;
    std::cout << "Всего ошибок (элементов): " << total_errors << std::endl;
    std::cout << "Максимум ошибок в одной последовательности: " << max_errors_in_sequence << std::endl;
    
    if (max_errors_in_sequence > 0) {
        std::cout << "Худшая последовательность: ncell_id=" << worst_sequence_ncell 
                  << ", frame_id=" << worst_sequence_frame << std::endl;
    }
    
    std::cout << "========================================" << std::endl;
}
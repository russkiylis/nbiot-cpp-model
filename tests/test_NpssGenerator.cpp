/**
 * @file test_NpssGenerator.cpp
 * @author russkiylis x Tobyret101
 * @brief Тестирование NPSS-генератора
 * @version 0.1
 * @date 24.04.2026
 * 
 */

 #include <gtest/gtest.h>
 #include "sync/NpssGenerator.h"
 #include "misc/ComplexSequenceReader.h"

 // Тест для проверки длины сгенерированной NPSS-последовательности
 TEST(test_NpssGenerator, NpssTester){
    NpssGenerator generator;
    ComplexSequenceReader reader;
    const double tolerance = 1e-4;  // точность

    ASSERT_TRUE(reader.loadFromFile(INPUT_DIR "matrix_d_output.txt"));

    const auto& Gen_sequence = generator.getNpssSequence();
    const auto& Read_sequence = reader.getSequence();

    ASSERT_EQ(Gen_sequence.size(), 121)
            << "размер сгенерированной последовательности не 121, а " << Gen_sequence.size();

    ASSERT_EQ(Gen_sequence.size(), Read_sequence.size())
            << "размер считанной последовательности не 121, а " << Read_sequence.size();
    for (size_t i = 0; i < Gen_sequence.size(); ++i) {
        EXPECT_NEAR(Gen_sequence[i].real(), Read_sequence[i].real(), tolerance);
        EXPECT_NEAR(Gen_sequence[i].imag(), Read_sequence[i].imag(), tolerance);
    }

    //
            

 }
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

 // Тест для проверки длины сгенерированной NPSS-последовательности
 TEST(test_NpssGenerator, SequenceLengthIs121){
    NpssGenerator generator;
    const auto& sequence = generator.getNpssSequence();
    EXPECT_EQ(sequence.size(), 121);
 }
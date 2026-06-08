#include "BitUtils.h"
#include "ComplexNumber.h"
#include "FourierTransform.h"
#include "Modulation.h"
#include "OfdmSignal.h"

// Параметры должны совпадать с GraphDataProvider.cpp
constexpr int            kBitCount        = 512;
constexpr int            kFftSize         = 32;
constexpr int            kCpLength        = 8;
constexpr int            kStartOffset     = kCpLength;
constexpr int            kWindowSize      = kFftSize;
constexpr int            kStepSize        = kFftSize + kCpLength;
constexpr int            kSubcarrierCount = 8;
constexpr ModulationType kModulationType  = ModulationType::Bpsk;

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ─── Вспомогательные функции вывода ──────────────────────────────────────────

static void PrintBits(const std::vector<int>& bits) {
    std::cout << "Биты (" << bits.size() << "): ";
    for (int bit : bits) std::cout << bit;
    std::cout << "\n\n";
}

static void PrintSymbols(const std::string& title, const std::vector<Complex>& symbols) {
    std::cout << title << " (" << symbols.size() << "):\n";
    for (size_t i = 0; i < symbols.size(); i++) {
        std::cout << "  [" << std::setw(2) << i << "]"
                  << "  re=" << std::setw(8) << symbols[i].re
                  << "  im=" << std::setw(8) << symbols[i].im
                  << "  |x|=" << std::setw(8) << symbols[i].abs()
                  << "\n";
    }
    std::cout << "\n";
}

static void PrintConstellation(ModulationType type) {
    std::string name;
    if (type == ModulationType::Bpsk)       name = "BPSK";
    else if (type == ModulationType::Qpsk)  name = "QPSK";
    else                                    name = "16-PSK";

    std::cout << "--- Созвездие " << name << " ---\n";
    std::cout << "  биты  |    re    |    im    | угол(°)\n";

    if (type == ModulationType::Bpsk) {
        for (int b = 0; b < 2; b++) {
            Complex s = Modulation::BpskMapBit(b);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b
                      << "     | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    } else if (type == ModulationType::Qpsk) {
        for (int b0 = 0; b0 < 2; b0++)
        for (int b1 = 0; b1 < 2; b1++) {
            Complex s = Modulation::QpskMapBits(b0, b1);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b0 << b1
                      << "    | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    } else {
        for (int b0 = 0; b0 < 2; b0++)
        for (int b1 = 0; b1 < 2; b1++)
        for (int b2 = 0; b2 < 2; b2++)
        for (int b3 = 0; b3 < 2; b3++) {
            Complex s = Modulation::Psk16MapBits(b0, b1, b2, b3);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b0 << b1 << b2 << b3
                      << " | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    }
    std::cout << "\n";
}

// ─── Демодуляция одной точки созвездия (метод ближайшего соседа) ─────────────

static std::vector<int> DemodulateSymbol(Complex received, ModulationType type) {
    if (type == ModulationType::Bpsk) {
        if (received.re >= 0.0) return {0};
        else                    return {1};
    }

    if (type == ModulationType::Qpsk) {
        double minDist = 1e18;
        std::vector<int> best;
        for (int b0 = 0; b0 < 2; b0++)
        for (int b1 = 0; b1 < 2; b1++) {
            Complex c = Modulation::QpskMapBits(b0, b1);
            double dr = received.re - c.re;
            double di = received.im - c.im;
            double dist = dr * dr + di * di;
            if (dist < minDist) {
                minDist = dist;
                best = {b0, b1};
            }
        }
        return best;
    }

    // 16-PSK
    double minDist = 1e18;
    std::vector<int> best;
    for (int b0 = 0; b0 < 2; b0++)
    for (int b1 = 0; b1 < 2; b1++)
    for (int b2 = 0; b2 < 2; b2++)
    for (int b3 = 0; b3 < 2; b3++) {
        Complex c = Modulation::Psk16MapBits(b0, b1, b2, b3);
        double dr = received.re - c.re;
        double di = received.im - c.im;
        double dist = dr * dr + di * di;
        if (dist < minDist) {
            minDist = dist;
            best = {b0, b1, b2, b3};
        }
    }
    return best;
}

// ─── Приёмник ─────────────────────────────────────────────────────────────────

static void RunReceiver(const std::vector<int>& txBits,
                        const std::vector<Complex>& signal)
{
    // Восстанавливаем TX-символы для сравнения
    const auto txChannels  = SplitChannels(txBits, kSubcarrierCount);
    const auto txModulated = Modulation::ModulateChannels(txChannels, kModulationType);

    std::cout << "==============================\n";
    std::cout << "  ПРИЁМНИК\n";
    std::cout << "==============================\n\n";
    std::cout << "Параметры: fftSize=" << kFftSize
              << "  CP=" << kCpLength
              << "  startOffset=" << kStartOffset
              << "  step=" << kStepSize
              << "  поднесущих=" << kSubcarrierCount << "\n\n";

    std::vector<int> rxBits;
    int symbolIdx = 0;
    int errCount  = 0;

    // Скользим по сигналу с шагом kStepSize, начиная с kStartOffset
    for (int offset = kStartOffset;
         offset + kWindowSize <= static_cast<int>(signal.size());
         offset += kStepSize)
    {
        // Извлекаем окно и делаем FFT
        std::vector<Complex> window;
        window.reserve(static_cast<size_t>(kWindowSize));
        for (int i = 0; i < kWindowSize; i++)
            window.push_back(signal[static_cast<size_t>(offset + i)]);

        const auto spectrum = FourierTransform::FFT(window);

        const bool printDetail = (symbolIdx < 3); // показываем первые 3 символа подробно

        if (printDetail) {
            std::cout << "─── OFDM-символ " << symbolIdx
                      << " (offset=" << offset << ") ───\n";
            std::cout << std::left
                      << std::setw(5)  << "sub"
                      << std::setw(5)  << "бин"
                      << std::setw(22) << "TX КА (re, im)"
                      << std::setw(22) << "RX КА (re, im)"
                      << std::setw(12) << "TX биты"
                      << std::setw(12) << "RX биты"
                      << "совп.\n";
            std::cout << std::string(78, '-') << "\n";
        }

        for (int sub = 0; sub < kSubcarrierCount; sub++) {
            const int bin = sub * kFftSize / kSubcarrierCount;

            // Нормируем принятый символ на размер FFT (IFFT уже нормирован на 1/N)
            Complex rxSymbol = spectrum[static_cast<size_t>(bin)];

            // TX-символ для этой поднесущей и этого OFDM-символа
            Complex txSymbol(0, 0);
            if (symbolIdx < static_cast<int>(txModulated[static_cast<size_t>(sub)].size()))
                txSymbol = txModulated[static_cast<size_t>(sub)][static_cast<size_t>(symbolIdx)];

            // Демодулируем
            const auto rxSymBits = DemodulateSymbol(rxSymbol, kModulationType);
            const auto txSymBits = DemodulateSymbol(txSymbol, kModulationType);

            // Считаем битовые ошибки
            for (size_t k = 0; k < rxSymBits.size(); k++) {
                rxBits.push_back(rxSymBits[k]);
                if (rxSymBits[k] != txSymBits[k])
                    errCount++;
            }

            if (printDetail) {
                // Форматируем TX/RX КА
                std::ostringstream txStr, rxStr, txBitsStr, rxBitsStr;
                txStr << "(" << std::setw(7) << std::fixed << std::setprecision(4) << txSymbol.re
                      << ", " << std::setw(7) << txSymbol.im << ")";
                rxStr << "(" << std::setw(7) << rxSymbol.re
                      << ", " << std::setw(7) << rxSymbol.im << ")";
                for (int b : txSymBits) txBitsStr << b;
                for (int b : rxSymBits) rxBitsStr << b;

                bool match = (txSymBits == rxSymBits);
                std::cout << std::left
                          << std::setw(5)  << sub
                          << std::setw(5)  << bin
                          << std::setw(22) << txStr.str()
                          << std::setw(22) << rxStr.str()
                          << std::setw(12) << txBitsStr.str()
                          << std::setw(12) << rxBitsStr.str()
                          << (match ? "✓" : "✗") << "\n";
            }
        }

        if (printDetail) std::cout << "\n";
        symbolIdx++;
    }

    // ─── Итог ────────────────────────────────────────────────────────────────
    int totalBits = static_cast<int>(rxBits.size());
    std::cout << "==============================\n";
    std::cout << "  РЕЗУЛЬТАТ\n";
    std::cout << "==============================\n";
    std::cout << "Символов обработано : " << symbolIdx << "\n";
    std::cout << "Бит восстановлено   : " << totalBits << "\n";
    std::cout << "Битовых ошибок      : " << errCount  << "\n";
    std::cout << "BER                 : "
              << std::fixed << std::setprecision(4)
              << (totalBits > 0 ? 100.0 * errCount / totalBits : 0.0)
              << "%\n\n";

    // Сравниваем восстановленные биты с исходными
    int cmpLen = std::min(static_cast<int>(txBits.size()), totalBits);
    std::cout << "TX биты (первые 64): ";
    for (int i = 0; i < std::min(64, cmpLen); i++) std::cout << txBits[i];
    std::cout << "\nRX биты (первые 64): ";
    for (int i = 0; i < std::min(64, totalBits); i++) std::cout << rxBits[i];
    std::cout << "\n\n";
}

// ─── main ────────────────────────────────────────────────────────────────────

int main() {
    std::cout << std::fixed << std::setprecision(4);

    // Созвездия
    for (ModulationType type : { ModulationType::Bpsk,
                                  ModulationType::Qpsk,
                                  ModulationType::Psk16 })
        PrintConstellation(type);

    // Генерируем биты и строим сигнал
    const auto bits   = GenerateBitSequence(kBitCount);
    const auto signal = BuildOfdmSignal(bits, kModulationType,
                                        kSubcarrierCount, kFftSize, kCpLength);

    PrintBits(bits);
    PrintSymbols("OFDM time signal", signal);

    // Запускаем приёмник
    RunReceiver(bits, signal);

    return 0;
}

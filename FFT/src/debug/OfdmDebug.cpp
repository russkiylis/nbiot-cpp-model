#include "BitUtils.h"
#include "ComplexNumber.h"
#include "CyclicPrefix.h"
#include "FourierTransform.h"
#include "Modulation.h"
#include "OfdmSignal.h"


constexpr int            kBitCount        = 1024;
constexpr int            kFftSize         = 32;
constexpr int            kCpLength        = 8;
constexpr int            kStartOffset     = 8;
constexpr int            kWindowSize      = 32;
constexpr int            kStepSize        = 40;
constexpr int            kSubcarrierCount = 32;
constexpr ModulationType kModulationType  = ModulationType::Qpsk;

// Параметры многолучевого канала
constexpr double kH0       = 1.0;  // амплитуда прямого луча
constexpr double kH1       = 1.0;  // амплитуда каждого эхо-луча
constexpr int    kNumRays  = 2;    // число лучей
constexpr int    kMaxDelay = 5;    // задержки 0,1,2,3,4,5 — нуль на sub=4 при h0=h1

// Значение пилотного символа на всех поднесущих (известно приёмнику)
static const Complex kPilotValue = Complex(1.0, 0.0);

#include <algorithm>
#include <cmath>
#include <random>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static std::string g_exeDir; // папка рядом с .exe, устанавливается в main()

// ─── Локальные обёртки над публичным API Modulation ──────────────────────────
//
// Ядро Modulation предоставляет только инстансный modulate(); прямого маппинга
// отдельных точек созвездия наружу нет. Для отладочного стенда (печать созвездий,
// демодуляция по ближайшему соседу) удобно получать точку по конкретным битам —
// оборачиваем modulate() на векторе из одного символа.
static Complex MapBpsk(int b) {
    return Modulation(ModulationType::Bpsk).modulate({ b })[0];
}
static Complex MapQpsk(int b0, int b1) {
    return Modulation(ModulationType::Qpsk).modulate({ b0, b1 })[0];
}
static Complex MapPsk16(int b0, int b1, int b2, int b3) {
    return Modulation(ModulationType::Psk16).modulate({ b0, b1, b2, b3 })[0];
}

// Модуляция набора каналов (поднесущих) через инстансный modulate().
static std::vector<std::vector<Complex>> ModulateChannels(
    const std::vector<std::vector<int>>& channels, ModulationType type) {
    Modulation mod(type);
    std::vector<std::vector<Complex>> out;
    out.reserve(channels.size());
    for (const auto& channel : channels)
        out.push_back(mod.modulate(channel));
    return out;
}

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
            Complex s = MapBpsk(b);
            double deg = std::atan2(s.im, s.re) * 180.0 / PI;
            std::cout << "  " << b
                      << "     | " << std::setw(8) << s.re
                      << " | " << std::setw(8) << s.im
                      << " | " << std::setw(8) << deg << "\n";
        }
    } else if (type == ModulationType::Qpsk) {
        for (int b0 = 0; b0 < 2; b0++)
        for (int b1 = 0; b1 < 2; b1++) {
            Complex s = MapQpsk(b0, b1);
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
            Complex s = MapPsk16(b0, b1, b2, b3);
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
        std::vector<int> best = {0, 0};
        for (int b0 = 0; b0 < 2; b0++)
        for (int b1 = 0; b1 < 2; b1++) {
            Complex c = MapQpsk(b0, b1);
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
    std::vector<int> best = {0, 0, 0, 0};
    for (int b0 = 0; b0 < 2; b0++)
    for (int b1 = 0; b1 < 2; b1++)
    for (int b2 = 0; b2 < 2; b2++)
    for (int b3 = 0; b3 < 2; b3++) {
        Complex c = MapPsk16(b0, b1, b2, b3);
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
    const auto txModulated = ModulateChannels(txChannels, kModulationType);

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
                          << (match ? "OK" : "ERR") << "\n";
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

// ─── Симуляция многолучевого канала 
// задержка между лучами одинаковая
static std::vector<Complex> ApplyTwoRayChannel(const std::vector<Complex>& tx,
                                                int numRays, int maxDelay) {
    std::vector<Complex> rx(tx.size(), Complex(0.0, 0.0));
    for (int ray = 0; ray < numRays; ray++) {
        int    delay = (numRays > 1) ? (ray * maxDelay / (numRays - 1)) : 0;
        double gain  = (ray == 0)    ? kH0 : kH1;
        for (int i = delay; i < (int)tx.size(); i++) {
            rx[i].re += gain * tx[i - delay].re;
            rx[i].im += gain * tx[i - delay].im;
        }
    }
    return rx;
}

// Знаем задержку - вычисляем отклик на каждой поднесущей аналитически
static Complex ChannelFreqResponse(int numRays, int maxDelay, int k, int N) {
    Complex H(kH0, 0.0);
    for (int ray = 1; ray < numRays; ray++) {
        int    delay = ray * maxDelay / (numRays - 1);
        double angle = 2.0 * PI * (k * delay) / N;
        H.re += kH1 * std::cos(angle);
        H.im -= kH1 * std::sin(angle);
    }
    return H;
}

static Complex ComplexDiv(const Complex& a, const Complex& b) {
    const double d = b.re * b.re + b.im * b.im;
    return Complex((a.re * b.re + a.im * b.im) / d,
                   (a.im * b.re - a.re * b.im) / d);
}

struct BerResult {
    int    symbols;
    int    totalBits;
    int    errorBits;
    double ber;
};

// Считает BER по принятому сигналу.
// equalize=true: делит каждый бин на H[k] перед демодуляцией.
// Окно берётся ровно с kCpLength, чтобы исключить влияние фазового сдвига от CP.
static BerResult ComputeBer(const std::vector<int>& txBits,
                             const std::vector<Complex>& signal,
                             bool equalize,
                             int numRays, int maxDelay) {
    const auto txChannels  = SplitChannels(txBits, kSubcarrierCount);
    const auto txModulated = ModulateChannels(txChannels, kModulationType);

    int symIdx   = 0;
    int errCount = 0;
    int total    = 0;

    // Начинаем ровно после CP — тогда Y[k] = H[k]*X[k] без посторонних фазовых набегов
    for (int offset = kCpLength;
         offset + kWindowSize <= static_cast<int>(signal.size());
         offset += kStepSize)
    {
        std::vector<Complex> window;
        window.reserve(static_cast<size_t>(kWindowSize));
        for (int i = 0; i < kWindowSize; i++)
            window.push_back(signal[static_cast<size_t>(offset + i)]);

        const auto spectrum = FourierTransform::FFT(window);

        for (int sub = 0; sub < kSubcarrierCount; sub++) {
            const int bin = sub * kFftSize / kSubcarrierCount;
            Complex rx = spectrum[static_cast<size_t>(bin)];

            if (equalize) {
                // Знаем задержку → вычисляем H[k] аналитически и делим
                const Complex H = ChannelFreqResponse(numRays, maxDelay, bin, kFftSize);
                rx = ComplexDiv(rx, H);
            }

            Complex txSym(0, 0);
            if (symIdx < static_cast<int>(txModulated[static_cast<size_t>(sub)].size()))
                txSym = txModulated[static_cast<size_t>(sub)][static_cast<size_t>(symIdx)];

            const auto rxBitVec = DemodulateSymbol(rx,    kModulationType);
            const auto txBitVec = DemodulateSymbol(txSym, kModulationType);

            for (size_t i = 0; i < rxBitVec.size(); i++) {
                total++;
                if (rxBitVec[i] != txBitVec[i]) errCount++;
            }
        }
        symIdx++;
    }

    return { symIdx, total, errCount,
             total > 0 ? static_cast<double>(errCount) / total : 0.0 };
}
// Генерация пилотного символа - 1+j0  на всех поднесущих, с циклическим префиксом
static std::vector<Complex> GeneratePilotSymbol() {
    std::vector<Complex> freq(kFftSize, Complex(0.0, 0.0));
    for (int sub = 0; sub < kSubcarrierCount; sub++)
        freq[sub * kFftSize / kSubcarrierCount] = kPilotValue;
    return AddCyclicPrefix(FourierTransform::IFFT(freq), kCpLength);
}

// Вычисляет отклик на поднесущей на основе принятого пилотного символа
static std::vector<Complex> EstimateChannel(const std::vector<Complex>& rxSignal) {
    std::vector<Complex> window;
    // отбрасываем CP
    for (int i = 0; i < kWindowSize; i++)
        window.push_back(rxSignal[kCpLength + i]);
    const auto spectrum = FourierTransform::FFT(window);
    std::vector<Complex> H_est(kSubcarrierCount);
    // вычисляем Y[k] / X[k] для каждой поднесущей (X[k]=1+j0)
    for (int sub = 0; sub < kSubcarrierCount; sub++)
        H_est[sub] = ComplexDiv(spectrum[sub * kFftSize / kSubcarrierCount], kPilotValue);
    return H_est;
}

//
static std::vector<double> PilotCrossCorr(const std::vector<Complex>& rxSignal,
                                           const std::vector<Complex>& txPilotData) {
    std::vector<Complex> rxPilot;
    for (int i = 0; i < kWindowSize; i++)
        rxPilot.push_back(rxSignal[kCpLength + i]);

    std::vector<double> xcorr(kFftSize / 2, 0.0);
    for (int tau = 0; tau < (int)xcorr.size(); tau++) {
        Complex sum(0.0, 0.0);
        for (int n = tau; n < kWindowSize; n++) {
            const Complex& x = txPilotData[n - tau];
            const Complex& y = rxPilot[n];
            sum.re += x.re * y.re + x.im * y.im;
            sum.im += x.re * y.im - x.im * y.re;
        }
        xcorr[tau] = sum.abs();
    }
    double maxVal = *std::max_element(xcorr.begin(), xcorr.end());
    if (maxVal > 0.0)
        for (auto& v : xcorr) v /= maxVal;
    return xcorr;
}

static int MaxDelayFromCorr(const std::vector<double>& xcorr, double threshold = 0.15) {
    int maxDelay = 0;
    for (int tau = 0; tau < (int)xcorr.size(); tau++)
        if (xcorr[tau] >= threshold)
            maxDelay = tau;
    return maxDelay;
}

// Вычисляет BER, используя оценку H[k] из пилотного символа.
// rxSignal: [пилот (kStepSize отсч.)] + [данные]; при equalize=false — без коррекции.
static BerResult ComputeBerEstimated(const std::vector<int>& txBits,
                                      const std::vector<Complex>& rxSignal,
                                      bool equalize,
                                      const std::vector<Complex>& H_est) {
    const auto txChannels  = SplitChannels(txBits, kSubcarrierCount);
    const auto txModulated = ModulateChannels(txChannels, kModulationType);

    int symIdx = 0, errCount = 0, total = 0;

    for (int offset = kStepSize + kCpLength;
         offset + kWindowSize <= static_cast<int>(rxSignal.size());
         offset += kStepSize)
    {
        std::vector<Complex> window;
        window.reserve(static_cast<size_t>(kWindowSize));
        for (int i = 0; i < kWindowSize; i++)
            window.push_back(rxSignal[static_cast<size_t>(offset + i)]);

        const auto spectrum = FourierTransform::FFT(window);

        for (int sub = 0; sub < kSubcarrierCount; sub++) {
            const int bin = sub * kFftSize / kSubcarrierCount;
            Complex rx = spectrum[static_cast<size_t>(bin)];
            if (equalize)
                rx = ComplexDiv(rx, H_est[static_cast<size_t>(sub)]);

            Complex txSym(0, 0);
            if (symIdx < static_cast<int>(txModulated[static_cast<size_t>(sub)].size()))
                txSym = txModulated[static_cast<size_t>(sub)][static_cast<size_t>(symIdx)];

            const auto rxBitVec = DemodulateSymbol(rx,    kModulationType);
            const auto txBitVec = DemodulateSymbol(txSym, kModulationType);
            for (size_t i = 0; i < rxBitVec.size(); i++) {
                total++;
                if (rxBitVec[i] != txBitVec[i]) errCount++;
            }
        }
        symIdx++;
    }
    return { symIdx, total, errCount,
             total > 0 ? static_cast<double>(errCount) / total : 0.0 };
}

static void RunMultipathSimulation(const std::vector<int>& txBits,
                                    const std::vector<Complex>& cleanSignal,
                                    int maxDelay,
                                    std::ostream& out) {
    const auto rxSignal = ApplyTwoRayChannel(cleanSignal, kNumRays, maxDelay);
    const BerResult noEq   = ComputeBer(txBits, rxSignal, false, kNumRays, maxDelay);
    const BerResult withEq = ComputeBer(txBits, rxSignal, true,  kNumRays, maxDelay);

    const auto pilotSymbol = GeneratePilotSymbol();
    std::vector<Complex> txFull;
    txFull.insert(txFull.end(), pilotSymbol.begin(), pilotSymbol.end());
    txFull.insert(txFull.end(), cleanSignal.begin(), cleanSignal.end());

    const auto rxFull    = ApplyTwoRayChannel(txFull, kNumRays, maxDelay);
    const auto H_est     = EstimateChannel(rxFull);
    const BerResult pilotEq = ComputeBerEstimated(txBits, rxFull, true, H_est);

    const std::vector<Complex> txPilotData(pilotSymbol.cbegin() + kCpLength, pilotSymbol.cend());
    const auto xcorr    = PilotCrossCorr(rxFull, txPilotData);
    const int  estDelay = MaxDelayFromCorr(xcorr);

    out << std::fixed << std::setprecision(6);
    out << "======== kMaxDelay = " << maxDelay << " =========\n\n";

    out << "Параметры канала (лучей: " << kNumRays << "):\n";
    for (int ray = 0; ray < kNumRays; ray++) {
        int    delay = (kNumRays > 1) ? (ray * maxDelay / (kNumRays - 1)) : 0;
        double gain  = (ray == 0) ? kH0 : kH1;
        std::string label = (ray == 0) ? "прямой луч" : "эхо " + std::to_string(ray);
        out << "  " << std::left << std::setw(12) << label
            << " gain=" << gain << "  задержка=" << delay << " отсч.\n";
    }
    out << "  (CP=" << kCpLength << ", макс.задержка=" << maxDelay
        << (maxDelay < kCpLength ? " < CP => ISI отсутствует)\n"
                                 : " >= CP => ВОЗМОЖНА ISI!)\n");

    out << "Пришедший сигнал\n";
    out << "  BER = " << noEq.ber * 100.0 << " %  ("
        << noEq.errorBits << " ошибок из " << noEq.totalBits << " бит)\n";
    out << "\nАналитически вычисленный отклик\n";
    out << "  BER = " << withEq.ber * 100.0 << " %  ("
        << withEq.errorBits << " ошибок из " << withEq.totalBits << " бит)\n";
    out << "\nОценка по пилотному символу\n";
    out << "  BER = " << pilotEq.ber * 100.0 << " %  ("
        << pilotEq.errorBits << " ошибок из " << pilotEq.totalBits << " бит)\n";

    out << "\nАЧХ канала: истинная vs. оценённая по пилоту\n";
    out << std::left
        << std::setw(5)  << "sub" << std::setw(5)  << "бин"
        << std::setw(10) << "|H_true|" << std::setw(13) << "угол_true(°)"
        << std::setw(10) << "|H_est|"  << std::setw(13) << "угол_est(°)" << "|ΔH|\n"
        << std::string(60, '-') << "\n";
    out << std::setprecision(4);
    for (int sub = 0; sub < kSubcarrierCount; sub++) {
        int     bin   = sub * kFftSize / kSubcarrierCount;
        Complex Htrue = ChannelFreqResponse(kNumRays, maxDelay, bin, kFftSize);
        Complex Hest  = H_est[sub];
        double  dRe   = Htrue.re - Hest.re;
        double  dIm   = Htrue.im - Hest.im;
        out << std::left
            << std::setw(5)  << sub   << std::setw(5)  << bin
            << std::setw(10) << Htrue.abs()
            << std::setw(13) << std::atan2(Htrue.im, Htrue.re) * 180.0 / PI
            << std::setw(10) << Hest.abs()
            << std::setw(13) << std::atan2(Hest.im, Hest.re) * 180.0 / PI
            << std::sqrt(dRe * dRe + dIm * dIm) << "\n";
    }

    out << "\nОценка задержки по кросс-корреляции пилота:\n";
    out << "  Истинная макс. задержка  = " << maxDelay  << " отсч.\n";
    out << "  Оценённая макс. задержка = " << estDelay  << " отсч.";
    out << (estDelay == maxDelay ? "  [точно]\n" : "  [расхождение!]\n");
    out << "\n  τ   |R[τ]|/Rmax  пик\n  " << std::string(28, '-') << "\n";
    for (int tau = 0; tau < (int)xcorr.size(); tau++) {
        double v = xcorr[tau];
        if (v < 0.02) continue;
        out << "  " << std::setw(3) << tau << "   "
            << std::setw(8) << v << "  "
            << std::string((int)(v * 20.0), '*') << "\n";
    }
    out << "\n" << std::string(60, '=') << "\n\n";
}

// ─── main ────────────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
    if (argc > 0) {
        std::string exe(argv[0]);
        auto sep = exe.find_last_of("/\\");
        g_exeDir = (sep != std::string::npos) ? exe.substr(0, sep + 1) : "";
    }

    std::cout << std::fixed << std::setprecision(4);

    // Созвездия
    for (ModulationType type : { ModulationType::Bpsk,
                                  ModulationType::Qpsk,
                                  ModulationType::Psk16 })
        PrintConstellation(type);

    // Генерируем случайные биты (seed из аппаратного источника энтропии)
    const std::uint32_t seed = std::random_device{}();
    std::cout << "Seed: " << seed << "\n\n";
    const auto bits   = GenerateBitSequence(kBitCount, seed);
    const auto signal = BuildOfdmSignal(bits, kModulationType,
                                        kSubcarrierCount, kFftSize, kCpLength);

    PrintBits(bits);
    PrintSymbols("OFDM time signal", signal);

    RunReceiver(bits, signal);

    std::string outPath = g_exeDir + "multipath_ber.txt";
    std::ofstream f(outPath);
    if (!f.is_open()) {
        std::cout << "\n[ОШИБКА] Не удалось открыть файл для записи\n";
        return 1;
    }

    f << "OFDM-сигнал: fftSize=" << kFftSize << "  CP=" << kCpLength
      << "  поднесущих=" << kSubcarrierCount << "  бит=" << kBitCount
      << "  лучей=" << kNumRays << "\n\n";

    for (int d = 0; d <= 16; d++) {
        std::cout << "\n--- kMaxDelay = " << d << " ---\n";
        RunMultipathSimulation(bits, signal, d, f);
        RunMultipathSimulation(bits, signal, d, std::cout);
    }

    std::cout << "\nРезультаты записаны в " << outPath << "\n";
    return 0;
}

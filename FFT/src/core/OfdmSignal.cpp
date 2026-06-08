#include "OfdmSignal.h"

#include "BitUtils.h"
#include "CyclicPrefix.h"
#include "FourierTransform.h"
#include "Modulation.h"

#include <algorithm>

std::vector<Complex> BuildOfdmSignal(const std::vector<int>& bits, ModulationType type, int subcarrierCount, int fftSize, int cpLength) {
    // Разбиваем биты по числу поднесущих и модулируем каждый канал
    const auto channels = SplitChannels(bits, subcarrierCount);
    const auto modulated = Modulation::ModulateChannels(channels, type);

    if (modulated.empty()) return {};

    int symbolsPerChannel = 0;
    for (const auto& ch : modulated) {
        symbolsPerChannel = std::max(symbolsPerChannel, static_cast<int>(ch.size()));
    }

    if (symbolsPerChannel == 0) return {};

    // Для каждого символьного индекса строим один OFDM-символ:
    // kSubcarrierCount активных бинов в kFftSize-точечном спектре → IFFT → kFftSize отсчётов
    const int symbolLen = fftSize + std::max(0, std::min(cpLength, fftSize));

    std::vector<Complex> signal;
    signal.reserve(static_cast<size_t>(symbolsPerChannel * symbolLen));

    for (int sym = 0; sym < symbolsPerChannel; ++sym) {
        std::vector<Complex> spectrum(static_cast<size_t>(fftSize), Complex(0.0, 0.0));

        for (int sub = 0; sub < subcarrierCount; ++sub) {
            if (sym < static_cast<int>(modulated[sub].size())) {
                // Поднесущие равномерно распределены по спектру
                const int bin = sub * fftSize / subcarrierCount;
                spectrum[static_cast<size_t>(bin)] = modulated[sub][sym];
            }
        }

        const auto symbolWithCp = AddCyclicPrefix(FourierTransform::IFFT(spectrum), cpLength);
        for (const auto& sample : symbolWithCp)
            signal.push_back(sample);
    }

    return signal;
}

std::vector<Complex> InstantSpectrum(const std::vector<Complex>& signal, int offset, int length) {
    if (offset < 0 || length <= 0 || offset >= static_cast<int>(signal.size())) {
        return {};
    }

    const int available = static_cast<int>(signal.size()) - offset;
    const int actualLength = std::min(length, available);

    std::vector<Complex> window;
    window.reserve(static_cast<size_t>(actualLength));
    for (int i = 0; i < actualLength; ++i) {
        window.push_back(signal[static_cast<size_t>(offset + i)]);
    }

    return FourierTransform::FFT(window);
}

std::vector<std::vector<Complex>> InstantSpectra(const std::vector<Complex>& signal, int length, int step, int startOffset) {
    std::vector<std::vector<Complex>> spectra;
    if (length <= 0 || step <= 0 || signal.empty()) {
        return spectra;
    }

    // startOffset пропускает CP первого символа; дальнейшие окна смещаются на step,
    // который равен fftSize + cpLength — так каждое окно попадает ровно на полезную часть символа
    for (int offset = startOffset; offset + length <= static_cast<int>(signal.size()); offset += step) {
        spectra.push_back(InstantSpectrum(signal, offset, length));
    }

    return spectra;
}

std::vector<Complex> AverageSpectrum(const std::vector<std::vector<Complex>>& spectra) {
    if (spectra.empty()) {
        return {};
    }

    size_t maxSize = 0;
    for (const auto& spectrum : spectra) {
        maxSize = std::max(maxSize, spectrum.size());
    }

    // Усредняем амплитуды по всем мгновенным спектрам.
    // Результат хранится в поле re как вещественное число (im остаётся 0).
    std::vector<Complex> average(maxSize, Complex(0.0, 0.0));
    std::vector<int> counts(maxSize, 0);

    for (const auto& spectrum : spectra) {
        for (size_t index = 0; index < spectrum.size(); ++index) {
            average[index].re += spectrum[index].abs();
            ++counts[index];
        }
    }

    for (size_t index = 0; index < average.size(); ++index) {
        if (counts[index] > 0) {
            average[index].re /= static_cast<double>(counts[index]);
        }
    }

    return average;
}

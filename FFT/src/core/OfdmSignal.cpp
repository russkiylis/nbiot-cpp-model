#include "OfdmSignal.h"

#include "BitUtils.h"
#include "FourierTransform.h"
#include "Modulation.h"

#include <algorithm>

namespace {
constexpr int kChannelCount = 4;
}

std::vector<Complex> BuildOfdmSignal(const std::vector<int>& bits, ModulationType type) {
    const auto channels = SplitChannels(bits);
    const auto bpskChannels = Modulation::ModulateChannels(channels, type);

    if (bpskChannels.empty()) {
        return {};
    }

    int symbolsPerChannel = 0;
    for (const auto& channel : bpskChannels) {
        symbolsPerChannel = std::max(symbolsPerChannel, static_cast<int>(channel.size()));
    }

    if (symbolsPerChannel == 0) {
        return {};
    }

    const int spectrumSize = symbolsPerChannel * kChannelCount;

    std::vector<Complex> spectrum(static_cast<size_t>(spectrumSize), Complex(0.0, 0.0));

    for (int channel = 0; channel < kChannelCount; ++channel) {
        const auto& values = bpskChannels[static_cast<size_t>(channel)];
        for (int symbol = 0; symbol < static_cast<int>(values.size()); ++symbol) {
            const int bin = channel + symbol * kChannelCount;
            if (bin < spectrumSize) {
                spectrum[static_cast<size_t>(bin)] = values[static_cast<size_t>(symbol)];
            }
        }
    }

    return FourierTransform::IFFT(spectrum);
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

std::vector<std::vector<Complex>> InstantSpectra(const std::vector<Complex>& signal, int length, int step) {
    std::vector<std::vector<Complex>> spectra;
    if (length <= 0 || step <= 0 || signal.empty()) {
        return spectra;
    }

    for (int offset = 0; offset + length <= static_cast<int>(signal.size()); offset += step) {
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

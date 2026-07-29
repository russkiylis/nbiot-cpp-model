#include "GraphDataProvider.h"

#include "BitUtils.h"
#include "OfdmSignal.h"
#include "SignalStats.h"

#include <QVariantMap>
#include <QString>

#include <algorithm>

namespace {
constexpr int kBitCount       = 512;                      // общее число бит
constexpr int kFftSize        = 32;                       // размер ОБПФ/БПФ одного OFDM-символа
constexpr int kCpLength       = 8;                        // длина циклического префикса 
constexpr int kStartOffset    = 8;                        // сдвиг первого окна приёмника
constexpr int kWindowSize     = 32;                       // окно анализа = полезной части символа
constexpr int kStepSize       = 40;                       // шаг = полная длина символа с CP
constexpr int kSubcarrierCount = 8;                       // число активных поднесущих
constexpr ModulationType kModulationType = ModulationType::Bpsk; // Bpsk | Qpsk | Psk16
}

GraphDataProvider::GraphDataProvider(QObject* parent)
    : QObject(parent),
      m_ofdmSignal(BuildOfdmSignal(GenerateBitSequence(kBitCount), kModulationType, kSubcarrierCount, kFftSize, kCpLength)) {
    m_spectra = InstantSpectra(m_ofdmSignal, kWindowSize, kStepSize, kStartOffset);
    m_averageSpectrum = AverageSpectrum(m_spectra);

    for (const auto& spectrum : m_spectra) {
        m_spectrumMax = std::max(m_spectrumMax, MaxAbs(spectrum));
    }

    m_spectrumMax = std::max(m_spectrumMax, MaxAbs(m_averageSpectrum));
}

int GraphDataProvider::windowSize() const {
    return kWindowSize;
}

int GraphDataProvider::cpLength() const {
    return kCpLength;
}

int GraphDataProvider::startOffset() const {
    return kStartOffset;
}

int GraphDataProvider::spectrumCount() const {
    return static_cast<int>(m_spectra.size());
}

qreal GraphDataProvider::spectrumMax() const {
    return m_spectrumMax;
}

QVariantList GraphDataProvider::spectrumPoints(int window) const {
    if (window < 0 || window >= static_cast<int>(m_spectra.size())) {
        return {};
    }

    return toPointList(m_spectra[static_cast<size_t>(window)]);
}

QVariantList GraphDataProvider::averageSpectrumPoints() const {
    return toPointList(m_averageSpectrum);
}

QVariantList GraphDataProvider::toPointList(const std::vector<Complex>& values) {
    QVariantList points;
    points.reserve(static_cast<qsizetype>(values.size()));

    for (size_t index = 0; index < values.size(); index++) {
        QVariantMap point;
        point.insert(QStringLiteral("x"), static_cast<double>(index));
        point.insert(QStringLiteral("y"), values[index].abs());
        points.push_back(point);
    }

    return points;
}

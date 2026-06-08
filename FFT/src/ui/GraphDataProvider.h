#pragma once

#include <QObject>
#include <QVariantList>

#include <vector>

#include "ComplexNumber.h"
#include "Modulation.h"

class GraphDataProvider : public QObject {
    Q_OBJECT
    Q_PROPERTY(int windowSize READ windowSize CONSTANT)
    Q_PROPERTY(int spectrumCount READ spectrumCount CONSTANT)
    Q_PROPERTY(qreal spectrumMax READ spectrumMax CONSTANT)
    Q_PROPERTY(int cpLength READ cpLength CONSTANT)
    Q_PROPERTY(int startOffset READ startOffset CONSTANT)

public:
    explicit GraphDataProvider(QObject* parent = nullptr);

    int windowSize() const;
    int spectrumCount() const;
    qreal spectrumMax() const;
    int cpLength() const;
    int startOffset() const;

    Q_INVOKABLE QVariantList spectrumPoints(int window) const;
    Q_INVOKABLE QVariantList averageSpectrumPoints() const;

private:
    static QVariantList toPointList(const std::vector<Complex>& values);

    std::vector<Complex> m_ofdmSignal;
    std::vector<std::vector<Complex>> m_spectra;
    std::vector<Complex> m_averageSpectrum;
    qreal m_spectrumMax = 0.0;
};

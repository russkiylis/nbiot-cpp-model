#pragma once

#include <vector>

#include "ComplexNumber.h"
#include "Modulation.h"

// subcarrierCount — число активных поднесущих (1..20), лучше делитель fftSize
// fftSize — размер IFFT одного OFDM-символа (степень двойки: 16, 32, 64, 128...)
std::vector<Complex> BuildOfdmSignal(const std::vector<int>& bits, ModulationType type = ModulationType::Bpsk, int subcarrierCount = 4, int fftSize = 64);
std::vector<Complex> InstantSpectrum(const std::vector<Complex>& signal, int offset, int length);
std::vector<std::vector<Complex>> InstantSpectra(const std::vector<Complex>& signal, int length, int step = 1);
std::vector<Complex> AverageSpectrum(const std::vector<std::vector<Complex>>& spectra);

#pragma once
#include <cstdint>
#include <vector>
#include "ComplexNumber.h"


std::vector<Complex> ApplyMultipathChannelFreq(const std::vector<Complex>& signal,
                                               int numRays,
                                               double maxDelaySamples,
                                               double h0 = 1.0,
                                               double h1 = 1.0);

std::vector<Complex> ApplyMultipathChannelCubic(const std::vector<Complex>& signal,
                                                int numRays,
                                                double maxDelaySamples,
                                                double h0 = 1.0,
                                                double h1 = 1.0);

std::vector<Complex> ApplyMultipathChannelSincRect(const std::vector<Complex>& signal,
                                                   int numRays,
                                                   double maxDelaySamples,
                                                   double h0 = 1.0,
                                                   double h1 = 1.0,
                                                   int halfWidth = 8);

std::vector<Complex> ApplyAwgn(const std::vector<Complex>& signal,
                               double sigma,
                               std::uint32_t seed = 42u);

std::vector<Complex> ApplyRayleighNoise(const std::vector<Complex>& signal,
                                        double sigma,
                                        std::uint32_t seed = 42u);

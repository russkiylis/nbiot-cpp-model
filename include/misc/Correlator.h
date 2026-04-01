#pragma once

#include <array>
#include <complex>

class Correlator {

    public:
        Correlator();
        ~Correlator();

        std::array<std::complex<float>, 241> cyclic_autocorrelation(std::array<std::complex<float>, 121>& sequence);

};
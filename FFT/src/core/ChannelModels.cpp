#include "ChannelModels.h"

#include <cmath>
#include <random>

#include "FourierTransform.h"

// ─── Многолучевой канал, частотная область
std::vector<Complex> ApplyMultipathChannelFreq(const std::vector<Complex>& signal,
                                               int numRays,
                                               double maxDelaySamples,
                                               double h0,
                                               double h1) {
    const auto X = FourierTransform::FFT(signal);
    const int N = static_cast<int>(X.size());

    std::vector<Complex> Y(N);
    for (int k = 0; k < N; k++) { // хвостовая часть сигнала от N до N+delay не учитывается
        Complex H(0.0, 0.0);
        for (int ray = 0; ray < numRays; ray++) {
            double delay;
            if (numRays > 1) {
                delay = ray * maxDelaySamples / (numRays - 1);
            }
            else {
                delay = 0.0;
            }             
            double gain;
            if (ray == 0) {
                gain = h0;
            }
            else {
                gain = h1;
            }          

            const double angle = 2.0 * PI * k * delay / N;
            H.re += gain * std::cos(angle);
            H.im -= gain * std::sin(angle);
        }
        Y[k] = X[k] * H;
    }

    auto y = FourierTransform::IFFT(Y);
    y.resize(signal.size()); // отбросить хвост нулевого дополнения до степени двойки
    return y;
}

//Многолучевой канал, интерполяция кубическим Лагранжем
std::vector<Complex> ApplyMultipathChannelCubic(const std::vector<Complex>& signal,
                                                int numRays,
                                                double maxDelaySamples,
                                                double h0,
                                                double h1) {
    const int n = static_cast<int>(signal.size());
    std::vector<Complex> output(n, Complex(0.0, 0.0));

    for (int ray = 0; ray < numRays; ray++) {
        double delay;
        if (numRays > 1) {
            delay = ray * maxDelaySamples / (numRays - 1);
        }
        else {
            delay = 0.0;
        }

        double gain;
        if (ray == 0) {
            gain = h0;
        }
        else {
            gain = h1;
        }

        const double M = std::floor(delay);
        const double frac = delay - M; //дробная часть задержки

        // Коэффициенты Лагранжа для 4-точечной интерполяции
        const double c[4] = {
            frac * (frac + 1.0) * (frac + 2.0) / 6.0,
            (1.0 - frac) * (frac + 1.0) * (frac + 2.0) / 2.0,
            -(1.0 - frac) * frac * (frac + 2.0) / 2.0,
            (1.0 - frac) * frac * (frac + 1.0) / 6.0
        };

        for (int i = 0; i < n; i++) {
            double accRe = 0.0, accIm = 0.0;
            for (int t = -1; t <= 2; t++) {
                const int idx = i - M + t;
                if (idx >= 0 && idx < n) {
                    const double w = c[t + 1];
                    accRe += w * signal[idx].re;
                    accIm += w * signal[idx].im;
                }
            }
            output[i].re += gain * accRe;
            output[i].im += gain * accIm;
        }
    }

    return output;
}

//Многолучевой канал, Котельников, окно прямоугольное

std::vector<Complex> ApplyMultipathChannelSincRect(const std::vector<Complex>& signal,
                                                   int numRays,
                                                   double maxDelaySamples,
                                                   double h0,
                                                   double h1,
                                                   int halfWidth) {
    const int n = static_cast<int>(signal.size());
    std::vector<Complex> output(n, Complex(0.0, 0.0));

    for (int ray = 0; ray < numRays; ray++) {
        double delay;
        if (numRays > 1) {
            delay = ray * maxDelaySamples / (numRays - 1);
        }
        else {
            delay = 0.0;
        }

        double gain;
        if (ray == 0) {
            gain = h0;
        }
        else {
            gain = h1;
        }
        const double M = std::floor(delay);
        const double frac = delay - M; // дробная часть задержки
        for (int i = 0; i < n; i++) {
            double accRe = 0.0, accIm = 0.0;
            for (int t = -(halfWidth - 1); t <= halfWidth; t++) { // ширина прямоугольного
                const int idx = i - M + t;
                if (idx >= 0 && idx < n) {
                    // sinc(x) = sin(πx)/(πx)
                    const double x = t + frac;
                    double w;
                    if (std::abs(x) < 1e-9) { // защита от деления на ноль
                        w = 1.0;
                    }
                    else {
                        w = std::sin(PI * x) / (PI * x);
                    }

                    accRe += w * signal[idx].re;
                    accIm += w * signal[idx].im;
                }
            }
            output[i].re += gain * accRe;
            output[i].im += gain * accIm;
        }
    }
    return output;
}

//АБГШ 
std::vector<Complex> ApplyAwgn(const std::vector<Complex>& signal, double sigma, std::uint32_t seed) {
    std::mt19937 rng(seed);
    std::normal_distribution<double> gauss(0.0, sigma);

    std::vector<Complex> output;
    output.reserve(signal.size());

    for (const auto& s : signal)
        output.push_back(Complex(s.re + gauss(rng), s.im + gauss(rng)));

    return output;
}

// Шум Релея
std::vector<Complex> ApplyRayleighNoise(const std::vector<Complex>& signal,
                                        double sigma,
                                        std::uint32_t seed) {
    const int n = static_cast<int>(signal.size());

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    std::vector<Complex> output;
    output.reserve(n);

    for (int i = 0; i < n; i++) {
        const double u1 = std::max(uniform(rng), 1e-12); // защита от log(0)
        const double u2 = uniform(rng);

        // Амплитуда по распределению Релея
        const double r   = sigma * std::sqrt(-2.0 * std::log(u1));
        // Равномерная случайная фаза
        const double phi = 2.0 * PI * u2;

        const Complex noise(r * std::cos(phi), r * std::sin(phi));
        output.push_back(signal[i] + noise);
    }

    return output;
}

#include "FourierTransform.h"

#include <algorithm>

long FourierTransform::opsCount = 0;

bool FourierTransform::PowTwo(int n) {
    return (n & (n - 1)) == 0;
}

int FourierTransform::NextPower(int n) {
    if (n <= 1) {
        return 1;
    }

    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

std::vector<Complex> FourierTransform::PadZeros(const std::vector<Complex>& input) {
    int n = input.size();
    if (PowTwo(n)) {
        return input;
    }

    std::vector<Complex> output = input;
    output.resize(NextPower(n), Complex(0, 0));
    return output;
}

std::vector<Complex> FourierTransform::DFT(const std::vector<Complex>& input) {
    int N = static_cast<int>(input.size());
    std::vector<Complex> output(N);
    // внешний цикл по выходным отсчетам
    for (int n = 0; n < N; n++) {
        Complex sum(0, 0);
        // внутренний цикл, вычисляющий сумму
        for (int k = 0; k < N; k++) {
            double angle = 2 * PI * n * k / N;
            Complex w(cos(angle), -sin(angle)); // Поворачивающий множитель
            sum = sum + (input[k] * w);
            opsCount++; // Считаем операцию умножения
        }
        output[n] = sum;
    }
    return output;
}

std::vector<Complex> FourierTransform::FFT(const std::vector<Complex>& input) {
    std::vector<Complex> output = PadZeros(input);
    int n = static_cast<int>(output.size());

    // перестановка элементов в битово-реверсном порядке 
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;

        if (i < j) {
            std::swap(output[i], output[j]);
        }
    }

    // Последовательное объединение «бабочек» (butterfly).
    // На каждом уровне len удваивается: обрабатываем пары, четвёрки, восьмёрки и т.д.
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2.0 * PI / len;
        Complex wn(cos(ang), -sin(ang)); // Поворачивающий множитель для текущего уровня

        for (int i = 0; i < n; i += len) {
            Complex w(1); // текущая степень wn
            for (int j = 0; j < len / 2; j++) {
                Complex u = output[i + j];
                Complex v = output[i + j + len / 2] * w;
                // Бабочка
                output[i + j]           = u + v;
                output[i + j + len / 2] = u - v;
                w = w * wn;
                opsCount++;
            }
        }
    }

    return output;
}

std::vector<Complex> FourierTransform::IFFT(const std::vector<Complex>& input) {
    // Обратное БПФ: то же самое, что прямое, но:
    //   1) поворачивающий множитель wn имеет плюс в экспоненте (сопряжённый)
    //   2) результат делится на n
    std::vector<Complex> output = PadZeros(input);
    int n = output.size();

    // Bit-reversal permutation — аналогично прямому БПФ
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;

        if (i < j) {
            std::swap(output[i], output[j]);
        }
    }

    // Butterfly-стадии с сопряжённым twiddle factor (+sin вместо -sin)
    for (int len = 2; len <= n; len *= 2) {
        double ang = 2.0 * PI / len;
        Complex wn(cos(ang), sin(ang)); // плюс — обратное ДПФ

        for (int i = 0; i < n; i += len) {
            Complex w(1);
            for (int j = 0; j < len / 2; j++) {
                Complex u = output[i + j];
                Complex v = output[i + j + len / 2] * w;
                output[i + j]           = u + v;
                output[i + j + len / 2] = u - v;
                w = w * wn;
                opsCount++;
            }
        }
    }

    // Нормировка на длину — обязательна для корректного обратного преобразования
    for (auto& value : output) {
        value = Complex(value.re / n, value.im / n);
    }

    return output;
}

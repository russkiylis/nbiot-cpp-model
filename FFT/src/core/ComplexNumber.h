#pragma once
#include <cmath>
#include <iostream>

static constexpr double PI = 3.14159265358979323846;

class Complex {
public:
    double re, im;

    Complex(double r = 0, double i = 0) : re(r), im(i) {}

    // Сложение
    Complex operator+(const Complex& other) const {
        return Complex(re + other.re, im + other.im);
    }

    // Вычитание
    Complex operator-(const Complex& other) const {
        return Complex(re - other.re, im - other.im);
    }

    // Умножение
    Complex operator*(const Complex& other) const {
        return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }

    // Деление
    Complex operator/(const Complex& other) const {
        const double d = other.re * other.re + other.im * other.im;
        return Complex((re * other.re + im * other.im) / d,
                        (im * other.re - re * other.im) / d);
    }

    // Модуль (для анализа амплитуды)
    double abs() const {
        return std::sqrt(re * re + im * im);
    }
};

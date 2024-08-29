#pragma once
#include "include_headers.h"


class DecaiableDoubleComplex {
    std::complex<double> z;
public:
    DecaiableDoubleComplex() = default;
    DecaiableDoubleComplex(double d): z{d} {}
    DecaiableDoubleComplex(const std::complex<double>& z): z{z} {}
    DecaiableDoubleComplex(std::complex<double>&& z): z{std::move(z)} {}
    operator std::complex<double>() const;
    double getImaginary() const;
    double getReal() const;
    std::complex<double> getComplex() const;
    std::complex<double>& getComplex();
    bool isReal() const;
};

template<class CharT, class Traits>
std::basic_ostream<CharT, Traits>& operator<<( std::basic_ostream<CharT, Traits>& os, const DecaiableDoubleComplex& z) {
    if (z.isReal()) {
        return os << z.getReal();
    } else {
        return os << z.getComplex();
    }
}

template<class CharT, class Traits>
std::basic_istream<CharT, Traits>& operator>>( std::basic_istream<CharT, Traits>& is, DecaiableDoubleComplex& z) {
    return is >> z.getComplex();
}

#include "value_types.h"

DecaiableDoubleComplex::operator std::complex<double>() const {
    return z;
}
double DecaiableDoubleComplex::getImaginary() const {
    return z.imag();
}
double DecaiableDoubleComplex::getReal() const {
    return z.real();
}
std::complex<double> DecaiableDoubleComplex::getComplex() const {
    return z;
}
std::complex<double>& DecaiableDoubleComplex::getComplex() {
    return z;
}
bool DecaiableDoubleComplex::isReal() const {
    return z.imag() == 0;
}

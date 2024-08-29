#pragma once
#include "include_headers.h"
#include "value_types.h"

template <class T>
T solveDelta(T a, T b, T c);

std::pair<DecaiableDoubleComplex, DecaiableDoubleComplex> quadraticSolution(const DecaiableDoubleComplex a, const DecaiableDoubleComplex b, const DecaiableDoubleComplex c) {
    using R = std::remove_reference_t<decltype(std::declval<DecaiableDoubleComplex>().getComplex())>;
    R delta = solveDelta(a.getComplex(), b.getComplex(), c.getComplex());
    return {{(std::sqrt(delta) - (R)b) / ((R)a * (R)2)}, {(-std::sqrt(delta) - (R)b) / ((R)a * (R)2)}};
}

template <class T>
T solveDelta(T a, T b, T c) {
    std::cout << "[delta] " << (b * b - a * c * (T)4) << "\r\n";      //debug 
    return b * b - a * c * (T)4;                     //flaky
}
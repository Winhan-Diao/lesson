#pragma once
#include "include_headers.h"
#include "value_types.h"

template <class T>
T solveDelta(T a, T b, T c) {
    return b * b - a * c * (T)4;                     //flaky
}

std::pair<DecaiableDoubleComplex, DecaiableDoubleComplex> quadraticSolution(const DecaiableDoubleComplex a, const DecaiableDoubleComplex b, const DecaiableDoubleComplex c);

#pragma once
#include "include_headers.h"

template <class T>
T solveDelta(T a, T b, T c);

template <class T1, class T2, class T3, class R = std::common_type_t<T1, T2, T3, double>>       //in case of long double
std::tuple<bool, std::complex<R>, std::complex<R>> quadraticSolution(T1 a, T2 b, T3 c) {
    R delta = solveDelta(static_cast<R>(a), static_cast<R>(b), static_cast<R>(c));
    if (delta < 0) return {false, (-b + std::sqrt(std::complex<R>{delta, 0}))/(2 * a), (-b - std::sqrt(std::complex<R>{delta, 0}))/(2 * a)};
    return {true, (-b + std::sqrt(delta))/(2 * a), (-b - std::sqrt(delta))/(2 * a)};
}

template <class T>
T solveDelta(T a, T b, T c) {
    std::cout << "[delta] " << (b * b - 4 * a *c) << "\r\n";      //debug 
    return b * b - 4 * a *c;                     //flaky
}
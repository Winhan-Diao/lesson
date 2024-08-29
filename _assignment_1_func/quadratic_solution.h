#pragma once
#include "include_headers.h"


template <class T>
T solveDelta(T a, T b, T c);

template <class T1, class T2, class T3, class C = std::common_type_t<T1, T2, T3, double>>       //in case of long double
std::tuple<bool, std::variant<double, std::complex<double>>, std::variant<double, std::complex<double>>> quadraticSolution(T1 a, T2 b, T3 c) {
    auto delta = solveDelta(static_cast<C>(a), static_cast<C>(b), static_cast<C>(c));
    if constexpr (std::is_floating_point_v<decltype(delta)>) {
        if (delta < 0) {
            return {false, {(std::sqrt(std::complex<double>{delta}) - b)/(a * 2)}, {(-std::sqrt(std::complex<double>{delta}) - b)/(a * 2)}};
        } else {
            return {true, {(std::sqrt(delta) - b)/(a * 2)}, {(-std::sqrt(delta) - b)/(a * 2)}};
        }
    } else {
        return {false, (std::sqrt(delta) - b)/(a * (decltype(delta))2), (-std::sqrt(delta) - b)/(a * (decltype(delta))2)};
    }
}

template <class T>
T solveDelta(T a, T b, T c) {
    std::cout << "[delta] " << (b * b - a * c * (T)4) << "\r\n";      //debug 
    return b * b - a * c * (T)4;                     //flaky
}
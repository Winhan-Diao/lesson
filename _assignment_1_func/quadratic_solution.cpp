#include "quadratic_solution.h"

std::pair<DecaiableDoubleComplex, DecaiableDoubleComplex> quadraticSolution(const DecaiableDoubleComplex a, const DecaiableDoubleComplex b, const DecaiableDoubleComplex c) {
    using R = std::remove_reference_t<decltype(std::declval<DecaiableDoubleComplex>().getComplex())>;
    R delta = solveDelta(a.getComplex(), b.getComplex(), c.getComplex());
    return {{(std::sqrt(delta) - (R)b) / ((R)a * (R)2)}, {(-std::sqrt(delta) - (R)b) / ((R)a * (R)2)}};
}

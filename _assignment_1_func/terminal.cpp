#include "terminal.h"
int main() {
    std::complex<double> a, b, c;
    std::vector<const std::complex<double> *> elems{&a, &b, &c};
    std::cout << "解决 Ax² + Bx + C = 0." << "\r\n";
    std::cout << "请依次输入A、B、C." << "\r\n";
    std::cin >> a >> b >> c;
    bool isAllReal = std::all_of(elems.cbegin(), elems.cend(), [](decltype(elems)::value_type elem){
        return elem->imag() == 0;
    });
    std::cout << "[isAllReal] " << std::boolalpha << isAllReal << "\r\n";       //debug
    bool isReal;
    std::variant<double, std::complex<double>> ans1, ans2;
    if (isAllReal) {
        std::cout << "将计算" << a.real() << "x² + " << b.real() << "x + " << c.real() << " = 0.\r\n";
        std::tie(isReal, ans1, ans2) = quadraticSolution(a.real(), b.real(), c.real());
    } else {
        std::cout << "将计算" << a << "x² + " << b << "x + " << c << " = 0.\r\n";
        std::tie(isReal, ans1, ans2) = quadraticSolution(a, b, c);
    }
    std::cout << "[ans1, variant index] " << ans1.index() << "\r\n";      //debug
    if (isReal) {
        std::cout << "答案: (" << std::get<double>(ans1) << ", " << std::get<double>(ans2) << ")" << "\r\n";
    } else {
        std::cout << "答案为复数(r, i): (" << std::get<std::complex<double>>(ans1) << ", " << std::get<std::complex<double>>(ans2) << ")" << "\r\n";
    }
    return 0;
}


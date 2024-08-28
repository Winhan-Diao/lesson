#include "terminal.h"
int main() {
    double a, b, c;
    std::cout << "解决 Ax² + Bx + C = 0." << "\r\n";
    std::cout << "请依次输入A、B、C." << "\r\n";
    std::cin >> a >> b >> c;
    std::cout << "将计算" << a << "x² + " << b << "x + " << c << " = 0.\r\n";
    auto [isReal, ans1, ans2] = quadraticSolution(a, b, c);
    if (isReal) {
        std::cout << "答案： (" << ans1.real() << ", " << ans2.real() << ")" << "\r\n";
    } else {
        std::cout << "答案为复数： (" << ans1 << ", " << ans2 << ")" << "\r\n";
    }
    return 0;
}
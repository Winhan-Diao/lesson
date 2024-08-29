#include "terminal.h"
int main() {
    DecaiableDoubleComplex a, b, c;
    std::cout << "解决 Ax² + Bx + C = 0." << "\r\n";
    std::cout << "请依次输入A、B、C." << "\r\n";
    std::cout << "支持格式1.r; 2.(r, i)" << "\r\n";
    std::cin >> a >> b >> c;
    std::cout << "将计算" << a << "x² + " << b << "x + " << c << " = 0.\r\n";
    auto [ans1, ans2] = quadraticSolution(a, b, c);
    std::cout << "答案: (" <<ans1 << ", " << ans2 << ")" << "\r\n";
    return 0;
}


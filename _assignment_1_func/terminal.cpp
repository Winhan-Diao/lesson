#include "terminal.h"
int main() {
    std::complex<double> arr[][3] = {{{0, -4}, 2, 3}, {2, 1, 4}, {5, 0, 8}, {-1, {1, 3}, 7}, {1, -5, {1, -1}}};
    std::complex<double> *flatArr = reinterpret_cast<decltype(flatArr)>(arr);
    std::array<std::pair<DecaiableDoubleComplex, DecaiableDoubleComplex>, 5> ddcPairArr;
    FlatContainerToPairSolver{flatArr, flatArr + 5*3, ddcPairArr.begin(), true}();

    return 0;
}

/* 
    std::ofstream ofs{".\\test.txt", std::ios::trunc | std::ios::out | std::ios::binary};
    do {
        StreamSolver(std::cin, ofs)();
    } while ((std::cout << "输入R再运行.\r\n", std::cin >> std::ws, std::getchar() == 'R'));
    ofs.close();
 */
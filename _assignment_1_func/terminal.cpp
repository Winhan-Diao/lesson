#include "terminal.h"
int main() {

    std::ofstream ofs{".\\test.txt", std::ios::trunc | std::ios::out | std::ios::binary};
    if (ofs.is_open()) {
        std::cout << "ofs is opened" << "\r\n";
    }

    do {
        StreamSolver(std::cin, ofs)();
    } while ((std::cout << "输入R再运行.\r\n", std::cin >> std::ws, std::getchar() == 'R'));
    
    ofs.close();

    return 0;
}


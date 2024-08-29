#include "terminal.h"
int main() {

    std::filesystem::path exePath = std::filesystem::current_path();
    std::cout << exePath << std::endl;
    std::ofstream ofs{".\\test.txt", std::ios::trunc | std::ios::out};
    if (ofs.is_open()) {
        std::cout << "ofs is opened" << "\r\n";
    }

    do {
        VerboseStreamSolver(std::cin, ofs)();
    } while ((std::cout << "输入R再运行.\r\n", std::cin >> std::ws, std::getchar() == ('R' || 'r')));
    
    ofs.close();

    return 0;
}


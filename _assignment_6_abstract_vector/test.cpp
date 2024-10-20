#include <iostream>
#include "abstract_vector.hpp"

class NoCpyMov {
public:
    NoCpyMov() = default;
    NoCpyMov(const NoCpyMov&) = delete;
    NoCpyMov(NoCpyMov&&) = delete;
};

int main() {
    char str[]{"qwert"};
    DebugVector<char, CAllocAllocator<char>> v(str, 4);
    std::cout << v << "\r\n";

    v.pushBack('F');
    std::cout << v << "\r\n";

    v.pushBack('D');
    std::cout << v << "\r\n";

    v.pushBack('O');
    std::cout << v << "\r\n";

    DebugVector<char, CAllocAllocator<char>> v2 = v;
    std::cout << v2 << "\r\n";

    v2.popBack();
    std::cout << v2 << "\r\n";

    auto iter = v2.begin();
    *iter = '$';
    std::cout << v2 << "\r\n";

    std::reverse(v2.begin(), v2.end());
    std::cout << v2 << "\r\n";

    v2.pushBack('^');
    v2.pushBack('\\');
    v2.pushBack('*');
    v2.pushBack('%');
    std::cout << v2 << "\r\n";

    std::cout << "inversed order: "; 
    std::for_each(v2.crbegin(), v2.crend(), [](auto& v){
        std::cout << v << ' ';
    });
    std::cout << "\r\n";

    std::cout << "v2[1]: " << v2[1] << "\r\n";
    v2[1] = '=';
    std::cout << "v2[1]: " << v2[1] << "\r\n";

    v = std::move(v2);

    std::cout << "v: " << v << "\r\n";
    std::cout << "v2: " << v2 << "\r\n";

    char str3[] = {"String 3"};
    DebugVector<char, CAllocAllocator<char>> v3(str3, 8);
    std::cout << v3 << "\r\n";
    v3 = v;
    std::cout << v3 << "\r\n";

    // DebugVector<NoCpyMov> invalid;

    double dArr[4]{.1, .2, .3, .4};
    DebugVector<double> v4(dArr, 4);
    std::cout << v4 << "\r\n";
    v4.pushBack(0.001);
    std::cout << v4 << "\r\n";

    return 0;
}
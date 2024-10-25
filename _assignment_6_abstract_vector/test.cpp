#include <iostream>
#include <iterator>
#include <complex>
#include <vector>
#include "abstract_vector.hpp"
#include "string_vector.hpp"

using namespace std::literals;

class NoCpyMov {
public:
    NoCpyMov() = default;
    NoCpyMov(const NoCpyMov&) = delete;
    NoCpyMov(NoCpyMov&&) = delete;
};

void archived1() {
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

    DebugVector<std::size_t, CAllocAllocator<std::size_t>> test1;       //valid
    // DebugVector<std::string, CAllocAllocator<std::string>> test2;        //invalid

    DebugVector<char, CAllocAllocator<char>> v5("Hello     ", 10);
    v5.pushBack(' ');
    std::cout << v5 << "\r\n";
    DebugVector<char> v6("W", 1);
    std::cout << v6 << "\r\n";
    // static_cast<AbstractVector<decltype(v5)::value_type, decltype(v5)::allocator_type>&>(v5) << v5;
    v5 << v6;
    std::cout << v5 << "\r\n";
}

void archived2() {
    DebugVector<char, CAllocAllocator<char>> v;
    v.pushBack('p');
    v.pushBack('o');
    v.pushBack('b');
    std::cout << v << "\r\n";
    v.clear();
    std::cout << v << "\r\n";
    v.pushBack('^');
    v.pushBack('-');
    v.pushBack('v');
    std::cout << v << "\r\n";

    auto t = v.erase(v.begin());
    std::cout << *t << " & " << v << "\r\n";
    v.erase(v.begin());
    std::cout << *t << " & " << v << "\r\n";
    v.erase(v.begin());
    std::cout << *t << " & " << v << "\r\n";        // t is end() here, actually not dereferenceable
    v.pushBack('d');
    std::cout << v << "\r\n";
    v.pushBack('o');
    std::cout << v << "\r\n";

    DebugVector<char, CAllocAllocator<char>> v1("This is a test about erase @^@", 30);
    auto iterF = v1.begin();
    std::advance(iterF, 7);
    auto iterE = v1.begin();
    std::advance(iterE, 20);
    v1.erase(iterF, iterE);
    std::cout << v1 << "\r\n";
    v1.emplaceBack('!');
    std::cout << v1 << "\r\n";
    v1.insert(iterF, ',');
    std::cout << v1 << "\r\n";
}

void archived3() {
    CollectionVector<char> v("It's said that such way of creating string is not allowed, huh?", 63);
    std::cout << v << "\r\n";

    std::complex<double> *cs = new std::complex<double>[4]{std::complex<double>(.1, .2), std::complex<double>(.3, .4), std::complex<double>(.5, .6), std::complex<double>(-.1, -.2)};
    std::complex<double> *cs1 = new std::complex<double>[4]{std::complex<double>(1, 2), std::complex<double>(3, 4), std::complex<double>(5, 6), std::complex<double>(-1, -2)};
    CollectionVector<std::complex<double>, CAllocAllocator<std::complex<double>>> v1(cs, 4);
    std::cout << v1 << "\r\n";

    v1.insert(v1.begin() + 1, std::complex<double>(.98, .99));
    std::cout << v1 << "\r\n";

    v1.pushBack(std::move(std::complex<double>(3.2, 3.3)));
    std::cout << v1 << "\r\n";
    v1.pushBack(std::move(std::complex<double>(3.3, 3.4)));
    std::cout << v1 << "\r\n";
    v1.pushBack(std::move(std::complex<double>(3.4, 3.5)));
    std::cout << v1 << "\r\n";

    CollectionVector<std::complex<double>, CAllocAllocator<std::complex<double>>> v2(cs1, 4);
    std::cout << v2 << "\r\n";
    v2 = v1;
    std::cout << v2 << "\r\n";

    v2.emplace(v2.begin() + 4, .01, .01);
    std::cout << v2 << "\r\n";

    CollectionVector<std::complex<double>, CAllocAllocator<std::complex<double>>> v3(v2, size_t(100));
    std::cout << v3 << "\r\n";

}

void archived4() {
    CollectionVector<std::string> v1{"Hello"s, "World"s, "This"s, "is"s, "Collection"s, "Vector"s, "Called"s, "v1"s};
    std::cout << v1 << "\r\n";
    v1.pushBack("Yes");
    std::cout << v1 << "\r\n";
    v1.emplace(v1.begin() + 2, "emplaced!"s);
    v1.emplaceBack("emplaced back!"s);
    std::cout << v1 << "\r\n";
    v1.erase(v1.begin() + 3, v1.begin() + 7);
    std::cout << v1 << "\r\n";
    v1.insert(v1.end() - 1, "no!"s);
    std::cout << v1 << "\r\n";
    v1.insert(v1.end() - 2, "no no no"s);
    std::cout << v1 << "\r\n";
    CollectionVector<std::string> v2 = {"VVVVVVVV"s, "22222222"s};
    v1.insert(v1.begin() + 5, v2.cbegin(), v2.cend());
    std::cout << v1 << "\r\n";
    v2 = v1;
    std::cout << v1 << "\r\n";
    std::cout << v2 << "\r\n";
    v1.insert(v1.begin() + 1, v2.cbegin(), v2.cend());
    std::cout << v1 << "\r\n";
    v1.insert(v1.begin(), "ddd"s);
    std::cout << v1 << "\r\n";
    v1.insert(v1.end(), v2.cbegin(), v2.cend());
    std::cout << v1 << "\r\n";


    CollectionVector<size_t, CAllocAllocator<size_t>> t = {1, 23, 345, 4567, 56789, 0};

    CollectionVector<std::string> v3(20, "testing"s);
    std::cout << v3 << "\r\n";
    CollectionVector<std::string> v4(38);
    std::cout << v4 << "\r\n";
    CollectionVector<size_t> t1(17, 0);
    std::cout << t1 << "\r\n";

}

int main() {
    StringVector sv1(10, 'd');
    StringVector sv2(10, 'e');
    sv1 << sv2;
    std::cout << sv1 << "\r\n";
    std::string t1 = "std::string"s;
    sv1.insert(sv1.begin() + 3, t1.cbegin(), t1.cend());
    std::cout << sv1 << "\r\n";
    return 0;
}


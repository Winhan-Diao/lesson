#include <iostream>
#include <utility>
#include <sstream>
#include "linked_int.h"

void archivedTest1() {
    LinkedInt linkedInt1(7, 11);
    linkedInt1.add(42);
    std::cout << const_cast<const LinkedInt&>(linkedInt1).getValue() << "\r\n";

    LinkedInt linkedInt2{linkedInt1};
    LinkedInt linkedInt3{std::move(linkedInt1)};

    LinkedInt linkedInt4(5, 55);
    linkedInt4 = linkedInt2;

    LinkedInt linkedInt5(2, 12);
    linkedInt5.insert(0, 336);
    linkedInt5.insert(3, 2091);

    std::cout << linkedInt5.getSize() << "\r\n";
    std::cout << linkedInt5 << "\r\n";
    std::cout << linkedInt4 << "\r\n";

    std::string s;
    std::cin >> linkedInt1;
    std::cout << "linkedint " << linkedInt1 << "\r\n";
    std::cin.clear();
    std::cin >> s;
    std::cout << "string " << s << "\r\n";
}

void archivedTest2() {
    std::stringstream ss{};
    ss << "1 2 3 4 5 6 7 8 9 10 ";
    LinkedInt linkedInt1{};
    ss >> linkedInt1;
    std::cout << linkedInt1 << "\r\n";
    linkedInt1 = *linkedInt1.remove(0);
    linkedInt1.remove(4);
    linkedInt1.remove(7);
    std::cout << linkedInt1 << "\r\n";
    linkedInt1.~LinkedInt();
}

int main() {
    std::stringstream ss{};
    ss << "1 2 3 4 5 6 7 8 9 10 ";
    LinkedInt linkedInt1{};
    ss >> linkedInt1;
    LinkedInt linkedInt1c{linkedInt1};
    const LinkedInt *linkedInt2 = new LinkedInt(3, 5);
    LinkedInt *const linkedInt3 = new LinkedInt(7, 9);
    std::cout << NumNodes(linkedInt1) << "\r\n";
    std::cout << NumNodes(&linkedInt1) << "\r\n";
    std::cout << NumNodes(std::move(linkedInt1)) << "\r\n";
    std::cout << NumNodes(linkedInt2) << "\r\n";
    std::cout << NumNodes(linkedInt3) << "\r\n";

    ShowList(linkedInt1c);
    std::cout << "\r\n";

    Append(linkedInt1c, 66);

    ShowList(linkedInt1c);
    return 0;
}

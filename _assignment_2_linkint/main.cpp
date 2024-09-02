#include <iostream>
#include <utility>
#include "linked_int.h"

int main() {
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
    return 0;
}
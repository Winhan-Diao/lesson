#include "linked_int_functions.hpp"

LinkedInt *&Create(LinkedInt *& toCreate, unsigned volume, int value) {
    toCreate = new LinkedInt(volume, value);
    return toCreate;
}

void Insert(LinkedInt& linkedInt, unsigned position, int value) {
    linkedInt.insert(position, value);
}

void Append(LinkedInt& linkedInt, int value) {
    linkedInt.add(value);
}

void FreeList(LinkedInt& linkedInt) {
    linkedInt.removeAll();
}
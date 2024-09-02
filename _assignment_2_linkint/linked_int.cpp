#include "linked_int.h"

LinkedInt::LinkedInt(unsigned volume, int value): value{value} {
    if (volume > 1U) {
        next = new LinkedInt(volume - 1U, value);
    }
}

// Shallow Copy
LinkedInt::LinkedInt(LinkedInt&& linkedInt) noexcept: value{std::move(linkedInt.value)} {
    if (this == &linkedInt) return;
    this->next = linkedInt.next;
    linkedInt.next = nullptr;
}

// Deep Copy
LinkedInt::LinkedInt(const LinkedInt& linkedInt) {
    this->value = linkedInt.value;
    if (linkedInt.next) {
        this->next = new LinkedInt{*linkedInt.next};
    }
}

// Shallow Copy
LinkedInt& LinkedInt::operator= (LinkedInt&& linkedInt) noexcept {
    if (this == &linkedInt) return *this;
    delete this->next;
    this->next = linkedInt.next;
    linkedInt.next = nullptr;
    return *this;
}

// Deep Copy
LinkedInt& LinkedInt::operator= (const LinkedInt& linkedInt) {
    std::cout << "Copy Assignment Called" << "\r\n";
    if (this == &linkedInt) return *this;
    delete this->next;
    this->value = linkedInt.value;
    if (linkedInt.next)
        this->next = new LinkedInt{*linkedInt.next};
    return *this;
}

void LinkedInt::add(int value) throw() {
    if (next) {
        next->add(value);
    } else {
        next = new LinkedInt{};
        next->value = value;
    }
}

void LinkedInt::setValue(int value) {
    this->value = value;
}

void LinkedInt::insert(unsigned position, int value) {
    if (position) {
        if (this->next) {
            this->next->insert(position - 1, value);
        } else {
            if (position - 1)
                throw std::out_of_range{"insertion failed"};
            else
                this->next = new LinkedInt(0U, value);
        }
    } else {
        auto toAdvancedNode = this->next;
        this->next = new LinkedInt(0U, this->value);
        this->next->next = toAdvancedNode;
        this->value = value;
    }
}

int& LinkedInt::getValue() noexcept {
    return value;
}
int LinkedInt::getValue() const noexcept {
    // std::cout << "[const] " << "\r\n";      //debug
    return value;
}

unsigned LinkedInt::getSize() const noexcept {
    int size = 0;
    traverse([&size](){++size;});
    return size;
}

LinkedInt::~LinkedInt() noexcept {
    // std::cout << "[~LinkedInt] " << "value: " << value << "\r\n";       //debug
    delete next;
}


std::ostream& operator<< (std::ostream& os, const LinkedInt& linkedInt) {
    os << const_cast<const LinkedInt&>(linkedInt).getValue() << ' ';
    linkedInt.traverse([](LinkedInt linkedInt){});
}

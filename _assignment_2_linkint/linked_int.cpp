#include "linked_int.h"

LinkedInt::LinkedInt(unsigned volume, int value): value{value} {
    if (volume > 1U) {
        next = new LinkedInt(volume - 1U, value);
    }
}

// Shallow Copy
LinkedInt::LinkedInt(LinkedInt&& linkedInt) noexcept: value{std::move(linkedInt.value)} {
    std::cout << "Move Constructor Called" << "\r\n";

    if (this == &linkedInt) return;
    this->next = linkedInt.next;
    linkedInt.next = nullptr;
}

// Deep Copy
LinkedInt::LinkedInt(const LinkedInt& linkedInt) {
    std::cout << "Copy Constructor Called" << "\r\n";

    this->value = linkedInt.value;
    if (linkedInt.next) {
        this->next = new LinkedInt{*linkedInt.next};
    }
}

// Shallow Copy
LinkedInt& LinkedInt::operator= (LinkedInt&& linkedInt) noexcept {
    std::cout << "Move Assignment Called" << "\r\n";        //debug

    if (this == &linkedInt) return *this;
    delete this->next;
    this->next = linkedInt.next;
    this->valid = linkedInt.valid;
    linkedInt.next = nullptr;
    return *this;
}

// Deep Copy
LinkedInt& LinkedInt::operator= (const LinkedInt& linkedInt) {
    std::cout << "Copy Assignment Called" << "\r\n";        //debug

    if (this == &linkedInt) return *this;
    delete this->next;
    this->value = linkedInt.value;
    if (linkedInt.valid) {
        this->valid = true;
    } else {
        throw AccessInvalidNodeException{};
    }
    if (linkedInt.next)
        this->next = new LinkedInt{*linkedInt.next};
    return *this;
}

void LinkedInt::add(int value) noexcept {
    if (next) {
        next->add(value);
    } else {
        next = new LinkedInt{};
        next->value = value;
    }
}

void LinkedInt::setValue(int value) {
    if (!valid) throw AccessInvalidNodeException{};
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

int& LinkedInt::getValue() {
    if (!valid) throw AccessInvalidNodeException{};
    return value;
}
int LinkedInt::getValue() const {
    if (!valid) throw AccessInvalidNodeException{};
    // std::cout << "[const] " << "\r\n";      //debug
    return value;
}

unsigned LinkedInt::getSize() const noexcept {
    int size = 0;
    runnableTraverse([&size](){++size;});
    return size;
}

LinkedInt *LinkedInt::remove(unsigned position) {
    if (position) {
        if (this->next) {
            this->next = this->next->remove(position - 1);
            return this;
        } else {
            throw std::out_of_range{"failed to remove"};
        }
    } else {
        LinkedInt *nodeNextToRemoved = this->next;
        this->next = nullptr;
        this->~LinkedInt();
        return nodeNextToRemoved;
    }
}

void LinkedInt::removeAll() noexcept {
    this->valid = false;
    delete this->next;
    this->next = nullptr;
}

LinkedInt::~LinkedInt() noexcept {
    // std::cout << "[~LinkedInt] " << "value: " << value << "\r\n";       //debug
    if (valid) {
        valid = false;
        delete next;
    }
}


std::ostream& operator<< (std::ostream& os, const LinkedInt& linkedInt) {
    linkedInt.consumerTraverse([&os](const LinkedInt& _linkedInt){
        if (!_linkedInt.valid) throw AccessInvalidNodeException{};
        os << _linkedInt.getValue() << ' ';
    });
    return os;
}

std::istream& operator>> (std::istream& is, LinkedInt& linkedInt) {
    delete linkedInt.next;
    is >> linkedInt.value;
    if (is.good()) {
        linkedInt.consumerTraverse([&is](LinkedInt& _linkedInt){
            int tmp;
            is >> tmp;
            if (is.good()) {
                _linkedInt.add(tmp);
            }
            return is.good();
        });
    }
    return is;
}
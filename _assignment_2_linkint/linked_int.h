#ifndef LINKED_INT_H
#define LINKED_INT_H
#include <iostream>
#include <exception>
#include "traits.h"

// No head
class LinkedInt {
private:
    int value{0};
    LinkedInt *next{nullptr};
protected:

public:
    LinkedInt() = default;
    explicit LinkedInt(unsigned volume, int value = 0);
    LinkedInt(LinkedInt&& linkedInt) noexcept;
    LinkedInt(const LinkedInt& linkedInt);
    LinkedInt& operator= (LinkedInt&& linkedInt) noexcept;
    LinkedInt& operator= (const LinkedInt& linkedInt);

    template <class T, typename std::enable_if_t<is_runnable<T>::value, int> = 0>
    void traverse(T&& runnable) const;
    template <class T, typename std::enable_if_t<is_consumer<T, LinkedInt>::value, int> = 0>
    void traverse(T&& consumer);

    void add(int value) throw();
    void setValue(int value);
    void insert(unsigned position, int value);
    int& getValue() noexcept;
    int getValue() const noexcept;
    unsigned getSize() const noexcept;

    ~LinkedInt() noexcept;
};

std::ostream& operator<< (std::ostream& os, LinkedInt linkedInt);

// ----------------------
//      definition
// ----------------------

template <class T, typename std::enable_if_t<is_runnable<T>::value, int>>
void LinkedInt::traverse(T&& runnable) const {
    runnable();
    if (this->next)
        this->next->traverse(std::forward<T>(runnable));
}

template <class T, typename std::enable_if_t<is_consumer<T, LinkedInt>::value, int> = 0>
void LinkedInt::traverse(T&& consumer) {
    consumer(*this);
    if (this->next)
        this->next->traverse(std::forward<T>(consumer));
}


#endif
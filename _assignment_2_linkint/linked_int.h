#ifndef LINKED_INT_H
#define LINKED_INT_H
#include <iostream>
#include <exception>
#include "traits.h"
#include "linked_int_exceptions.h"

// No head
class LinkedInt {
private:
    int value{0};
    bool valid{true};
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
    void runnableTraverse(T&& runnable) const;
    template <class T, typename = std::enable_if_t<is_consumer<T, LinkedInt&>::value>>
    void consumerTraverse(T&& consumer) const;
    template <class T, typename = std::enable_if_t<is_consumer<T, LinkedInt&>::value>>
    void consumerTraverse(T&& consumer);

    void add(int value) noexcept;
    void setValue(int value);
    void insert(unsigned position, int value);
    int& getValue();
    int getValue() const;
    unsigned getSize() const noexcept;
    // Return a pointer to the first valid node
    LinkedInt *remove(unsigned position);
    void removeAll() noexcept;

    ~LinkedInt() noexcept;

    friend std::ostream& operator<< (std::ostream& os, const LinkedInt& linkedInt);
    friend std::istream& operator>> (std::istream& is, LinkedInt& linkedInt);
};
std::ostream& operator<< (std::ostream& os, const LinkedInt& linkedInt);
std::istream& operator>> (std::istream& is, LinkedInt& linkedInt);

// ----------------------
//      definition
// ----------------------

template <class T, typename std::enable_if_t<is_runnable<T>::value, int>>
void LinkedInt::runnableTraverse(T&& runnable) const {
    bool continueFlag{true};
    if constexpr (std::is_convertible_v<decltype(runnable()), bool>) {
        continueFlag = runnable();
    } else {
        runnable();
    }
    if (this->next && continueFlag)
        this->next->runnableTraverse(std::forward<T>(runnable));
}

template <class T, typename>
void LinkedInt::consumerTraverse(T&& consumer) const {
    bool continueFlag{true};
    if constexpr (std::is_convertible_v<decltype(consumer(std::declval<const LinkedInt&>())), bool>) {
        continueFlag = consumer(*this);
    } else {
        consumer(*this);
    }
    if (this->next && continueFlag)
        const_cast<const LinkedInt*>(this->next)->consumerTraverse(std::forward<T>(consumer));
}

template <class T, typename>
void LinkedInt::consumerTraverse(T&& consumer) {
    bool continueFlag{true};
    if constexpr (std::is_convertible_v<decltype(consumer(std::declval<LinkedInt&>())), bool>) {
        continueFlag = consumer(*this);
    } else {
        consumer(*this);
    }
    // std::cout << "non const traverser is called" << "\r\n";     //debug
    if (this->next && continueFlag)
        this->next->consumerTraverse(std::forward<T>(consumer));
}

#include "linked_int_functions.hpp"
#endif
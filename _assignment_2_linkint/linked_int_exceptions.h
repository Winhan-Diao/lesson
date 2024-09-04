#ifndef LINKED_INT_EXCEPTIONS_H
#define LINKED_INT_EXCEPTIONS_H
#include <exception>

struct AccessInvalidNodeException: public std::exception {
    const char *what() const throw() override {
        return "Accessing invalid node is not allowed.";
    }
};


#endif
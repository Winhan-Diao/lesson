#pragma once
#include <memory>
#include <cstdlib>

/*
⚠️ WARNING ⚠️  
This is only safe from use for trivially copyable types.  
Take your own risk of flaky memory allocating stuff if you use it for other types!  
See [C++ named requirements: TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) for more info
*/
template <class T>
class CAllocAllocator {
public:
    using value_type = T;
    T *allocate(std::size_t size) {
        return reinterpret_cast<T *>(std::malloc(sizeof(T) * size));
    }    
    void deallocate(T *p, std::size_t) {
        std::free(p);
    }
    void reallocate(T *& p, std::size_t newSize) {
        p = reinterpret_cast<T *>(std::realloc(p, sizeof(T) * newSize));
    }
};
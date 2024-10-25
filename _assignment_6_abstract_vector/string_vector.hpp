#pragma once
#include <string_view>
#include "abstract_vector.hpp"
#include "c_alloc_allocator.hpp"
/*
    ""svc
    append
    insert(pos, SVec)
    insert(pos, CStr, size)
    insert(pos, CStr)
*/
template <class Alloc = CAllocAllocator<char>>
class StringVector: public AbstractVector<char, Alloc> {
public:
    using AbstractVector<char, Alloc>::operator<<;
    StringVector() = default;
    StringVector(char* const& data, size_t size): AbstractVector<char, Alloc>(data, size) {}
    StringVector(const StringVector& v, size_t volume): AbstractVector<char, Alloc>(v, volume) {}
    StringVector(std::initializer_list<char> l): AbstractVector<char, Alloc>(l) {}
    StringVector(size_t size): AbstractVector<char, Alloc>(size) {}
    StringVector(size_t size, const char& t): AbstractVector<char, Alloc>(size, t) {}
    AbstractVector<char, Alloc>& operator+=(const AbstractVector<char, Alloc>& v) override { 
        return *this << v;
    }
    std::unique_ptr<AbstractVector<char, Alloc>> operator+(const AbstractVector<char, Alloc>& v) const override { 
        auto neoVector = std::make_unique<StringVector<Alloc>>(*this);
        throw std::runtime_error{"Not supported for StringVector"};
    }
    [[deprecated]]AbstractVector<char, Alloc>& operator<<(long long) override { throw std::runtime_error{"Not supported for StringVector"}; }
    ~StringVector() override = default;
    template <class _Alloc>
    friend std::ostream& operator<<(std::ostream&, const StringVector<_Alloc>&);
};

template <class Alloc>
std::ostream& operator<<(std::ostream& os, const StringVector<Alloc>& sVec) {
    if (sVec.size)
        os << std::string_view(&*sVec.cbegin(), sVec.size);
    return os;
}
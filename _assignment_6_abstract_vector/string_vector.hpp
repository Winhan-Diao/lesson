#pragma once
#include <string_view>
#include <cstring>
#include "abstract_vector.hpp"
#include "c_alloc_allocator.hpp"
/*
    [x] ""svc  
    [x] append  
    [x] insert(pos, SVec)  
    [x] insert(pos, CStr, size)  
    [x] insert(pos, CStr)  
    [x] substr
*/
template <class Alloc = CAllocAllocator<char>>
class StringVector: public AbstractVector<char, Alloc> {
public:
    using AbstractVector<char, Alloc>::operator<<;
    using AbstractVector<char, Alloc>::insert;
    StringVector() = default;
    StringVector(const char* const& data, size_t size): AbstractVector<char, Alloc>(data, size) {}
    StringVector(const StringVector& v, size_t volume): AbstractVector<char, Alloc>(v, volume) {}
    StringVector(std::initializer_list<char> l): AbstractVector<char, Alloc>(l) {}
    StringVector(size_t size): AbstractVector<char, Alloc>(size) {}
    StringVector(size_t size, const char& t): AbstractVector<char, Alloc>(size, t) {}
    StringVector(const std::string& str): AbstractVector<char, Alloc>(str.data(), str.size()) {}
    StringVector& operator= (const std::string& str) {
        if (str.size() > this->volume) {
            this->expand(str.size() - this->volume);
        }
        std::memcpy(reinterpret_cast<void *>(this->begin().operator->()), reinterpret_cast<const void *>(str.data()), sizeof(char) * str.size());  // Fixed spelling of 'operator->'
        this->size = str.size();
        return *this;
    }
    AbstractVector<char, Alloc>& operator+=(const AbstractVector<char, Alloc>& v) override { 
        return *this << v;
    }
    // std::unique_ptr<AbstractVector<char, Alloc>> operator+(const AbstractVector<char, Alloc>& v) const override { 
    //     auto neoVector = std::make_unique<StringVector<Alloc>>(*this);
    //     *neoVector << v;
    //     return neoVector;
    // }
    template <class _Alloc>
    StringVector& append(const AbstractVector<char, _Alloc>& v) {
        return *this << v;
    }
    StringVector& insert(std::size_t index, const char* str) {
        insert(index, str, std::strlen(str));
        return *this;
    }
    StringVector& insert(std::size_t index, const char* str, size_t len) {
        insert(this->begin() + index, str, str + len);
        return *this;
    }
    template <class _Alloc>
    StringVector& insert(std::size_t index, const AbstractVector<char, _Alloc>& v) {
        insert(this->begin() + index, v.cbegin(), v.cend());
        return *this;
    }
    StringVector substr(size_t pos = 0, size_t count = -1) const {
        if (pos > this->size)
            throw std::out_of_range{"pos is greater than size"};
        return StringVector((this->cbegin() + pos).operator->(), std::min(count, this->size - pos));
    }
    const char *getData() const {
        return this->data;
    }
    std::unique_ptr<char[]> c_str() const {
        std::unique_ptr<char[]> arr = std::make_unique<char[]>(this->size + 1);
        std::memcpy(reinterpret_cast<void *>(arr.get()), this->cbegin().operator->(), this->size);
        arr[this->size] = '\0';
        return arr;
    }
    operator std::string() {
        return std::string(this->data, this->size);
    }
    ~StringVector() noexcept override = default;
    template <class _Alloc>
    friend std::ostream& operator<<(std::ostream&, const StringVector<_Alloc>&);
};

template <class Alloc>
std::ostream& operator<<(std::ostream& os, const StringVector<Alloc>& sVec) {
    if (sVec.size)
        os << std::string_view(&*sVec.cbegin(), sVec.size);
    return os;
}

template <class Alloc>
std::istream& operator>>(std::istream& is, StringVector<Alloc>& sVec) {
    std::string tmpBuffer;
    is >> tmpBuffer;
    sVec = tmpBuffer;
    return is;
}

StringVector<CAllocAllocator<char>> operator""_sv(const char *str, size_t size) {
    return StringVector<CAllocAllocator<char>>(str, size);
}

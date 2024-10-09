#include <iostream>
#include <cstring>
#include <algorithm>
#include <string>
#include "traits.h"

using namespace std::string_literals;

/* 
[x] Complete check for nullptr constructing
[x] +
[x] +=
[x] <<
[x] >> (dumb)
[x] ==, <, <=, >=, >, !=
[x] []
[x] SimpleFlexString <--> std::string 
 */

class SimpleFlexString {
    size_t len;
    char *buffer;
public:
    SimpleFlexString(): len{0}, buffer{nullptr} {}
    SimpleFlexString(const char *_str): len{_str? std::strlen(_str): size_t{0}}, buffer{_str? new char[len + 1]: nullptr} {
        if (buffer)
            std::memcpy(buffer, _str, len + 1);
    }
    SimpleFlexString(const std::string& _str): len{_str.length()}, buffer{new char[len + 1]{}} {
            std::memcpy(buffer, _str.c_str(), len);
    }
    explicit SimpleFlexString(size_t len, const char *_src = nullptr): len{len}, buffer{new char[len + 1]{}} {
        if (_src)
            std::memcpy(buffer, _src, len);
    }
    SimpleFlexString(const SimpleFlexString&);
    SimpleFlexString(SimpleFlexString&&) noexcept;
    SimpleFlexString& operator= (const SimpleFlexString&);
    SimpleFlexString& operator= (SimpleFlexString&&) noexcept;

    bool operator== (const SimpleFlexString&) const;
    bool operator!= (const SimpleFlexString&) const;
    bool operator< (const SimpleFlexString&) const;
    bool operator> (const SimpleFlexString&) const;
    bool operator<= (const SimpleFlexString&) const;
    bool operator>= (const SimpleFlexString&) const;

    char& operator[] (size_t pos);
    const char& operator[] (size_t pos) const;

    SimpleFlexString operator+ (const SimpleFlexString&) const;
    SimpleFlexString& operator+= (const SimpleFlexString&);
    template <class T, typename = std::enable_if_t<ctt::is_valid_to_string_v<T>>>
    SimpleFlexString& operator<< (const T&);
    SimpleFlexString& operator<< (const SimpleFlexString&);

    size_t length() const;
    const char *c_str() const;

    operator std::string();

    ~SimpleFlexString() noexcept {
        delete[] buffer;
    }
    friend std::ostream& operator<< (std::ostream& os, const SimpleFlexString& sfs);
    friend std::istream& operator>> (std::istream& is, SimpleFlexString& sfs);
};

size_t SimpleFlexString::length() const {
    return len;
}
const char *SimpleFlexString::c_str() const {
    return buffer;
}
SimpleFlexString::SimpleFlexString(const SimpleFlexString& sfs): len{sfs.len}, buffer{new char[len + 1]} {
    std::memcpy(this->buffer, sfs.buffer, this->len + 1);
}
SimpleFlexString::SimpleFlexString(SimpleFlexString&& sfs) noexcept: len{sfs.len}, buffer{sfs.buffer} {
    sfs.buffer = nullptr;
    sfs.len = 0;
}
SimpleFlexString& SimpleFlexString::operator= (const SimpleFlexString& sfs) {
    if (this == &sfs) return *this;
    delete[] this->buffer;
    this->buffer = new char[sfs.len + 1];
    std::memcpy(this->buffer, sfs.buffer, sfs.len + 1);
    len = sfs.len;
    return *this;
}
SimpleFlexString& SimpleFlexString::operator= (SimpleFlexString&& sfs) noexcept {
    if (this == &sfs) return *this;
    delete[] this->buffer;
    this->buffer = sfs.buffer;
    sfs.buffer = nullptr;
    len = sfs.len;
    sfs.len = 0;
    return *this;
}
bool SimpleFlexString::operator== (const SimpleFlexString& sfs) const {
    if (this->len != sfs.len) return false;
    return std::equal(this->buffer, this->buffer + len, sfs.buffer);
}
bool SimpleFlexString::operator!= (const SimpleFlexString& sfs) const {
    return !(*this == sfs);
}
bool SimpleFlexString::operator< (const SimpleFlexString& sfs) const {
    return std::lexicographical_compare(this->buffer, this->buffer + len, sfs.buffer, sfs.buffer + len);
}
bool SimpleFlexString::operator> (const SimpleFlexString& sfs) const {
    return std::lexicographical_compare(sfs.buffer, sfs.buffer + len, this->buffer, this->buffer + len);
}
bool SimpleFlexString::operator<= (const SimpleFlexString& sfs) const {
    return !std::lexicographical_compare(sfs.buffer, sfs.buffer + len, this->buffer, this->buffer + len);
}
bool SimpleFlexString::operator>= (const SimpleFlexString& sfs) const {
    return !std::lexicographical_compare(this->buffer, this->buffer + len, sfs.buffer, sfs.buffer + len);
}
char& SimpleFlexString::operator[] (size_t pos) {
    return const_cast<char&>(const_cast<const SimpleFlexString&>(*this).operator[](pos));
}
const char& SimpleFlexString::operator[] (size_t pos) const {
    if (pos >= len) 
        throw std::out_of_range{"argument pos("s + std::to_string(pos) + ") "s + "is greater than or equal to the string length(" + std::to_string(len) + ")"s};
    return this->buffer[pos];
}
SimpleFlexString SimpleFlexString::operator+ (const SimpleFlexString& sfs) const {
    size_t len = this->len + sfs.len;
    SimpleFlexString neoSfs{len + 1};
    neoSfs.buffer[0] = *this->buffer;
    std::memcpy(&neoSfs.buffer[0], this->buffer, this->len);
    std::memcpy(&neoSfs.buffer[this->len], sfs.buffer, sfs.len);
    return neoSfs;
}
SimpleFlexString& SimpleFlexString::operator+= (const SimpleFlexString& sfs) {
    char *neoBuffer = new char[this->len + sfs.len + 1];
    neoBuffer[this->len + sfs.len] = '\0';
    std::memcpy(&neoBuffer[0], this->buffer, this->len);
    std::memcpy(&neoBuffer[this->len], sfs.buffer, sfs.len);
    if (&this->buffer != &sfs.buffer) delete[] this->buffer;
    this->buffer = neoBuffer;
    this->len += sfs.len;
    return *this;
}
template <class T, typename>
SimpleFlexString& SimpleFlexString::operator<< (const T& t) {
    return *this << std::to_string(t);
}
SimpleFlexString& SimpleFlexString::operator<< (const SimpleFlexString& sfs) {
    return *this += sfs;
}
SimpleFlexString::operator std::string() {
    return std::string{buffer};
}
std::ostream& operator<< (std::ostream& os, const SimpleFlexString& sfs) {
    os << sfs.buffer;
    return os;
}
std::istream& operator>> (std::istream& is, SimpleFlexString& sfs) {
    std::string tmp;
    is >> tmp;
    sfs = tmp.c_str();
    return is;
}

void test1(const std::string& str) {
    std::cout << "[const std::string& str] " << str << "\r\n";
}   
void test2(const SimpleFlexString& str) {
    std::cout << "[const SimpleFlexString& str] " << str.length() << "\r\n";
}   

int main() {
    SimpleFlexString sfs1{"1234567890"};
    SimpleFlexString sfs2;
    std::cout << sfs1 << "\r\n";
    std::cin >> sfs2;
    std::cout << sfs2 << "\r\n";
    sfs1 << "~"s << sfs2 << "+"s << sfs1 << " "s << 1982.21;
    std::cout << sfs1 << "; with len:" << sfs1.length() << "\r\n";

    try {
        std::cout << sfs1[20];
    } catch (const std::exception& e) {
        std::cout << "what(): " << e.what() << "\r\n";
    }
    test1(sfs1);
    // test2(34.1);
}

/* 
https://pythontutor.com/render.html#code=%23include%20%3Ciostream%3E%0A%23include%20%3Ccstring%3E%0A%0Aclass%20SimpleFlexString%20%7B%0A%20%20%20%20size_t%20len%3B%0A%20%20%20%20char%20*buffer%3B%0Apublic%3A%0A%20%20%20%20SimpleFlexString%28%29%3A%20len%7B0%7D,%20buffer%7Bnullptr%7D%20%7B%7D%0A%20%20%20%20SimpleFlexString%28const%20char%20*_str%29%3A%20len%7Bstd%3A%3Astrlen%28_str%29%7D,%20buffer%7Bnew%20char%5Blen%20%2B%201%5D%7D%20%7B%0A%20%20%20%20%20%20%20%20std%3A%3Amemcpy%28buffer,%20_str,%20len%20%2B%201%29%3B%0A%20%20%20%20%7D%0A%20%20%20%20SimpleFlexString%28const%20char%20*_pos,%20size_t%20len%29%3A%20len%7Blen%7D,%20buffer%7Bnew%20char%5Blen%20%2B%201%5D%7D%20%7B%0A%20%20%20%20%20%20%20%20std%3A%3Amemcpy%28buffer,%20_pos,%20len%29%3B%0A%20%20%20%20%20%20%20%20buffer%5Blen%5D%20%3D%20'%5C0'%3B%0A%20%20%20%20%7D%0A%20%20%20%20SimpleFlexString%26%20operator%3D%20%28const%20SimpleFlexString%26%20sfs%29%3B%0A%20%20%20%20SimpleFlexString%26%20operator%3D%20%28SimpleFlexString%26%26%20sfs%29%3B%0A%0A%20%20%20%20size_t%20length%28%29%20const%3B%0A%20%20%20%20char%20*c_str%28%29%3B%0A%20%20%20%20const%20char%20*c_str%28%29%20const%3B%0A%0A%20%20%20%20~SimpleFlexString%28%29%20noexcept%20%7B%0A%20%20%20%20%20%20%20%20delete%5B%5D%20buffer%3B%0A%20%20%20%20%7D%0A%7D%3B%0A%0Asize_t%20SimpleFlexString%3A%3Alength%28%29%20const%20%7B%0A%20%20%20%20return%20len%3B%0A%7D%0Achar%20*SimpleFlexString%3A%3Ac_str%28%29%20%7B%0A%20%20%20%20return%20buffer%3B%0A%7D%0Aconst%20char%20*SimpleFlexString%3A%3Ac_str%28%29%20const%20%7B%0A%20%20%20%20return%20buffer%3B%0A%7D%0ASimpleFlexString%26%20SimpleFlexString%3A%3Aoperator%3D%20%28const%20SimpleFlexString%26%20sfs%29%20%7B%0A%20%20%20%20if%20%28this%20%3D%3D%20%26sfs%29%20return%20*this%3B%0A%20%20%20%20delete%5B%5D%20this-%3Ebuffer%3B%0A%20%20%20%20this-%3Ebuffer%20%3D%20new%20char%5Bsfs.len%20%2B%201%5D%3B%0A%20%20%20%20std%3A%3Amemcpy%28this-%3Ebuffer,%20sfs.buffer,%20sfs.len%20%2B%201%29%3B%0A%20%20%20%20return%20*this%3B%0A%7D%0ASimpleFlexString%26%20SimpleFlexString%3A%3Aoperator%3D%20%28SimpleFlexString%26%26%20sfs%29%20%7B%0A%20%20%20%20if%20%28this%20%3D%3D%20%26sfs%29%20return%20*this%3B%0A%20%20%20%20delete%5B%5D%20this-%3Ebuffer%3B%0A%20%20%20%20this-%3Ebuffer%20%3D%20sfs.buffer%3B%0A%20%20%20%20sfs.buffer%20%3D%20nullptr%3B%0A%20%20%20%20return%20*this%3B%0A%7D%0A%0Aint%20main%28%29%20%7B%0A%20%20%20%20SimpleFlexString%20sfs1%7B%22asllldddld%22%7D%3B%0A%20%20%20%20SimpleFlexString%20sfs2%7B%22123456789%22,%200%7D%3B%0A%20%20%20%20SimpleFlexString%20sfs3%7B%22%22%7D%3B%0A%7D&cumulative=false&curInstr=0&heapPrimitives=nevernest&mode=display&origin=opt-frontend.js&py=cpp_g%2B%2B9.3.0&rawInputLstJSON=%5B%5D&textReferences=false
 */
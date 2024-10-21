#include <iostream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <type_traits>
#include "c_alloc_allocator.hpp"

/*
函数&方法：
protected:
    expand：一次性扩展向量1.5倍，注意是volume变不是size变。
public：
    构造函数1：无参数构造
    构造函数2：由C数组构造
    拷贝构造函数
    移动构造函数
    #常量实例# []重载
    []重载
    #常量实例# at：获取元素
    at：获取元素
    %虚% pushBack：追加末元素
    %虚% popBack：删除末元素
    %纯虚% +=(AbstractVector)重载
    %纯虚% <<(size_t)重载
    %虚% 取首个/尾个（常量）（逆向）迭代器
    %虚% 析构函数
*/

template <class T>
class AbstractVectorIterator;

template <class T>
class AbstractVectorReversedIterator;

template <class T, class Alloc = std::allocator<T>, typename = std::enable_if_t<(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>) && (!std::is_same_v<Alloc, CAllocAllocator<T>> || std::is_trivially_copyable_v<T>)>>
class AbstractVector {
public:
    using value_type = T;
    using allocator_type = Alloc;
    using allocator_traits_type = std::allocator_traits<Alloc>;
    using a_t_t = std::allocator_traits<Alloc>;
protected:
    Alloc alloc;
    T *data;
    size_t size;
    size_t volume;
    virtual void expand() {     // 一次性扩展向量1.5倍，而不是pushBack一个扩展一个
        if (volume)
            volume *= 1.5;
        else
            ++volume;
        if constexpr (std::is_same_v<Alloc, CAllocAllocator<T>>) {
            std::cout << "[debug] CAllocAllocator 'Specified' Implementation of AbstractVector::expand" << "\r\n";       //debug
            alloc.reallocate(data, volume);
        } else {
            T *neoData = a_t_t::allocate(alloc, volume);
            if (data)
                std::move(data, data + size, neoData);
            for (size_t i = 0; i < size; ++i) a_t_t::destroy(alloc, &data[i]);
            a_t_t::deallocate(alloc, data, size);
            data = neoData;
        }
    }
public:
    AbstractVector(): data(nullptr), size(0), volume(0) {}
    AbstractVector(T* const& data, size_t size): alloc(), data(a_t_t::allocate(alloc, size)), size(size), volume(size) {
        if (data)
            std::copy(data, data + size, this->data);
    }
    AbstractVector(const AbstractVector& v): alloc(v.alloc), data(a_t_t::allocate(alloc, v.volume)), size(v.size), volume(v.volume) {
        for (size_t i = 0; i < size; ++i) a_t_t::construct(alloc, &data[i], v.data[i]);
    }
    AbstractVector(AbstractVector&& v): alloc(std::move(v.alloc)), data(v.data), size(v.size), volume(v.volume) {
        v.data = nullptr;
        v.size = 0;
        v.volume = 0;
    }
    AbstractVector& operator= (const AbstractVector& v) {
        if (data == v.data) return *this;
        for (size_t i = 0; i < size; ++i) a_t_t::destroy(alloc, &data[i]);
        a_t_t::deallocate(alloc, data, volume);     // can improve, actually
        data = a_t_t::allocate(alloc, v.volume);
        std::copy(v.cbegin(), v.cend(), begin());
        size = v.size;
        volume = v.volume;
        return *this;
    }
    AbstractVector& operator= (AbstractVector&& v) {
        if (data == v.data) return *this;
        for (size_t i = 0; i < size; ++i) a_t_t::destroy(alloc, &data[i]);
        a_t_t::deallocate(alloc, data, volume);
        data = v.data;
        size = v.size;
        volume = v.volume;
        v.data = nullptr;
        v.size = 0;
        v.volume = 0;
        return *this;
    }
    const T& operator[] (size_t index) const noexcept {      // 常量实例的访问，无检查与异常
        return data[index];
    }
    T& operator[] (size_t index) noexcept {     // 变量实例的访问，无检查与异常
        return const_cast<T&>(const_cast<const AbstractVector&>(*this)[index]);
    }
    const T& at(size_t index) const {       // 常量实例的访问，有检查与异常
        if (index < size)
            return data[index];
        else
            throw std::out_of_range{"range out of bound"};
    }
    T& at(size_t index) {       // 变量实例的访问，有检查与异常
        return const_cast<T&>(const_cast<const AbstractVector&>(*this).at(index));
    }
    virtual void pushBack(const T& element) {       // 往后追加，可以在string重写以适配'\0'
        if (size == volume) {
            expand();
        }
        data[size] = element;
        ++size;
    }
    virtual void popBack() {
        if (size > 0) {
            a_t_t::destroy(alloc, &data[size - 1]);
            --size;
        }
    }
    virtual AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>&) = 0;       // vector:数值加；string:追加
    virtual AbstractVector<T, Alloc>& operator<<(long long) = 0;       // vector:移位；string:追加数字
    virtual AbstractVectorIterator<T> begin() { return AbstractVectorIterator<T>(&data[0]); }
    virtual AbstractVectorIterator<const T> cbegin() const { return AbstractVectorIterator<const T>(&data[0]); }
    virtual AbstractVectorIterator<T> end() { return AbstractVectorIterator<T>(&data[size]); }
    virtual AbstractVectorIterator<const T> cend() const { return AbstractVectorIterator<const T>(&data[size]); }
    virtual AbstractVectorReversedIterator<T> rbegin() { return AbstractVectorReversedIterator<T>(&data[size? size - 1: 0]); }
    virtual AbstractVectorReversedIterator<const T> crbegin() const { return AbstractVectorReversedIterator<const T>(&data[size? size - 1: 0]); }
    virtual AbstractVectorReversedIterator<T> rend() { return AbstractVectorReversedIterator<T>(size? data - 1: data); }
    virtual AbstractVectorReversedIterator<const T> crend() const { return AbstractVectorReversedIterator<const T>(size? data - 1: data); }

    virtual ~AbstractVector() noexcept {
        for (size_t i = 0; i < size; ++i) a_t_t::destroy(alloc, &data[i]);
        a_t_t::deallocate(alloc, data, size);
    }
    friend int main();
    template <class _T, class _Alloc>
    friend std::ostream& operator<<(std::ostream&, const AbstractVector<_T, _Alloc>&);
};

template <class T, class Alloc = std::allocator<T>>
class DebugVector: public AbstractVector<T, Alloc> {
public:
    DebugVector() = default;
    DebugVector(T* const& data, size_t size): AbstractVector<T, Alloc>(data, size) {}
    AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>&) override { return *this; }       // vector:数值加；string:追加
    AbstractVector<T, Alloc>& operator<<(long long) override { return *this; }       // vector:移位；string:追加数字
};

template <class T>
class AbstractVectorIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
protected:
    T *ptr;
public:
    AbstractVectorIterator(T *ptr): ptr(ptr) {};
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    AbstractVectorIterator& operator++() {
        ++ptr;
        return *this;
    }
    AbstractVectorIterator operator++(int) {
        auto tmp = *this;
        ++ptr;
        return tmp;
    }
    AbstractVectorIterator& operator--() {
        --ptr;
        return *this;
    }
    AbstractVectorIterator operator--(int) {
        auto tmp = *this;
        --ptr;
        return tmp;
    }
    AbstractVectorIterator operator+(std::ptrdiff_t n) const { return AbstractVectorIterator(ptr + n); }
    AbstractVectorIterator operator-(std::ptrdiff_t n) const { return AbstractVectorIterator(ptr - n); }
    std::ptrdiff_t operator-(const AbstractVectorIterator& i) const { return std::ptrdiff_t(ptr - i.ptr); }
    T& operator[](std::ptrdiff_t n) { return ptr[n]; }
    bool operator==(const AbstractVectorIterator& i) const { return ptr == i.ptr; };
    bool operator!=(const AbstractVectorIterator& i) const { return ptr != i.ptr; };
    bool operator>(const AbstractVectorIterator& i) const { return ptr > i.ptr; };
    bool operator<(const AbstractVectorIterator& i) const { return ptr < i.ptr; };
    bool operator>=(const AbstractVectorIterator& i) const { return ptr >= i.ptr; };
    bool operator<=(const AbstractVectorIterator& i) const { return ptr <= i.ptr; };
};

template <class T>
class AbstractVectorReversedIterator: public AbstractVectorIterator<T> {
public:
    AbstractVectorReversedIterator(T *ptr): AbstractVectorIterator<T>(ptr) {}
    AbstractVectorReversedIterator& operator--() {
        ++this->ptr;
        return *this;
    }
    AbstractVectorReversedIterator operator--(int) {
        auto tmp = *this;
        ++this->ptr;
        return tmp;
    }
    AbstractVectorReversedIterator& operator++() {
        --this->ptr;
        return *this;
    }
    AbstractVectorReversedIterator operator++(int) {
        auto tmp = *this;
        --this->ptr;
        return tmp;
    }
    std::ptrdiff_t operator-(const AbstractVectorReversedIterator& i) const { return std::ptrdiff_t(i.ptr - this->ptr); }
    AbstractVectorReversedIterator operator-(std::ptrdiff_t n) const { return AbstractVectorReversedIterator(this->ptr + n); }
    AbstractVectorReversedIterator operator+(std::ptrdiff_t n) const { return AbstractVectorReversedIterator(this->ptr - n); }
    bool operator<(const AbstractVectorReversedIterator& i) const { return this->ptr > i.ptr; };
    bool operator>(const AbstractVectorReversedIterator& i) const { return this->ptr < i.ptr; };
    bool operator<=(const AbstractVectorReversedIterator& i) const { return this->ptr >= i.ptr; };
    bool operator>=(const AbstractVectorReversedIterator& i) const { return this->ptr <= i.ptr; };
};

template <class _T, class _Alloc>
std::ostream& operator<<(std::ostream& os, const AbstractVector<_T, _Alloc>& v) {
    os << '{' << '[';
    for (size_t i = 0; i < v.size; ++i) {
        os << v[i];
        if (i < v.size - 1) 
            os << ", ";
    }
    os << "]; v: " << v.volume << "; s: " << v.size << '}';
    return os;
}
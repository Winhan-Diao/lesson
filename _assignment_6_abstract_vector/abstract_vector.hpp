#include <iostream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <cmath>
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
    virtual void expand(size_t requestedExtra = 0) {     // 一次性扩展向量1.5倍，而不是pushBack一个扩展一个
        if (requestedExtra) {
            volume += requestedExtra;
        } else {
            volume = 1.5 * volume + 1;
        }
        if constexpr (std::is_same_v<allocator_type, CAllocAllocator<T>>) {
            std::cout << "[debug] CAllocAllocator 'Specified' Implementation of AbstractVector::expand" << "\r\n";       //debug
            alloc.reallocate(data, volume);
        } else {
            T *neoData = a_t_t::allocate(alloc, volume);
            if (data)
                std::move(data, data + size, neoData);
            deleteAll();
            data = neoData;
        }
    }
    void deleteAll() noexcept {
        if (data) {
            for (size_t i = 0; i < size; ++i) a_t_t::destroy(alloc, &data[i]);
            a_t_t::deallocate(alloc, data, size);
            data = nullptr;
        }
    }
public:
    AbstractVector(): alloc(), data(nullptr), size(0), volume(0) {}
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
        deleteAll();
        data = a_t_t::allocate(alloc, v.volume);
        std::copy(v.cbegin(), v.cend(), begin());
        size = v.size;
        volume = v.volume;
        return *this;
    }
    AbstractVector& operator= (AbstractVector&& v) {
        if (data == v.data) return *this;
        deleteAll();
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
    void pushBack(const T& element) {       // 往后追加，可以在string重写以适配'\0'
        if (size == volume) {
            expand();
        }
        data[size] = element;
        ++size;
    }
    void popBack() {
        if (size > 0) {
            a_t_t::destroy(alloc, &data[size - 1]);
            --size;
        }
    }
    void clear() {
        deleteAll();
        size = 0;
        volume = 0;
    }
    AbstractVectorIterator<T> erase(AbstractVectorIterator<T> first, AbstractVectorIterator<T> last) {      // passing last exceeding end() is invalidated
        if (first < last) {
            if constexpr (std::is_same_v<allocator_type, CAllocAllocator<value_type>>)
                std::memcpy(reinterpret_cast<void *>(first.operator->()), reinterpret_cast<void *>(last.operator->()), sizeof(value_type) * (end() - last));
            else 
                std::move(last, end(), first);
            for (size_t i = size - (last - first); i < size; ++i) a_t_t::destroy(alloc, &data[size - 1]);
            size -= last - first;
        }
        return first;
    }
    AbstractVectorIterator<T> erase(AbstractVectorIterator<T> pos) {        // passing end() as pos is invalidated, which is the same policy as the std::vector
        AbstractVectorIterator<T> nextPos = pos + 1;
        if (pos < end()) {
            if constexpr (std::is_same_v<allocator_type, CAllocAllocator<value_type>>)
                std::memcpy(reinterpret_cast<void *>(pos.operator->()), reinterpret_cast<void *>(nextPos.operator->()), sizeof(value_type) * (end() - nextPos));
            else 
                std::move(nextPos, end(), pos);
            a_t_t::destroy(alloc, &data[size - 1]);
            --size;
        }
        return pos;
    }

    virtual AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>&) = 0;       // vector:数值加；string:追加
    virtual std::unique_ptr<AbstractVector<T, Alloc>> operator+(const AbstractVector<T, Alloc>&) const = 0;       // vector:数值加；string:追加
    virtual AbstractVector<T, Alloc>& operator<<(long long) = 0;       // vector:移位；string:追加数字
    template<class _Alloc>
    AbstractVector<T, Alloc>& operator<<(const AbstractVector<T, _Alloc>& v) {
        if (this->volume < (this->size + v.getSize())) {
            this->expand(this->size + v.getSize() - this->volume);
        }
        std::copy(v.cbegin(), v.cend(), this->end());
        this->size += v.getSize();
        return *this;
    }
    AbstractVectorIterator<T> begin() { return AbstractVectorIterator<T>(&data[0]); }
    AbstractVectorIterator<const T> cbegin() const { return AbstractVectorIterator<const T>(&data[0]); }
    AbstractVectorIterator<T> end() { return AbstractVectorIterator<T>(&data[size]); }
    AbstractVectorIterator<const T> cend() const { return AbstractVectorIterator<const T>(&data[size]); }
    AbstractVectorReversedIterator<T> rbegin() { return AbstractVectorReversedIterator<T>(&data[size? size - 1: 0]); }
    AbstractVectorReversedIterator<const T> crbegin() const { return AbstractVectorReversedIterator<const T>(&data[size? size - 1: 0]); }
    AbstractVectorReversedIterator<T> rend() { return AbstractVectorReversedIterator<T>(size? data - 1: data); }
    AbstractVectorReversedIterator<const T> crend() const { return AbstractVectorReversedIterator<const T>(size? data - 1: data); }

    size_t getSize() const {
        return size;
    }

    virtual ~AbstractVector() noexcept {
        deleteAll();
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
    DebugVector(const DebugVector& v): AbstractVector<T, Alloc>(v) {}
    AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>&) override { return *this; }       // vector:数值加；string:追加
    std::unique_ptr<AbstractVector<T, Alloc>> operator+(const AbstractVector<T, Alloc>&) const override { return std::make_unique<DebugVector<T, Alloc>>(*this); }       // vector:数值加；string:追加
    AbstractVector<T, Alloc>& operator<<(long long) override { return *this; }       // vector:移位；string:追加数字
    using AbstractVector<T, Alloc>::operator<<;
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
    AbstractVectorIterator& operator+= (ssize_t n) {
        ptr += n;
        return *this;
    }
    AbstractVectorIterator& operator-= (ssize_t n) {
        ptr -= n;
        return *this;
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
#pragma once
#include <iostream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <cmath>
#include "c_alloc_allocator.hpp"
#define OPTIMIZE true
#define HI_COMPAT true
#define EXPAND_FACTOR 2     //must be greater than 1, otherwise the behavior were undefined
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
    void expand(size_t requestedExtra = 0) {     // 一次性扩展向量1.5倍，而不是pushBack一个扩展一个
        if (requestedExtra) {
            volume += requestedExtra;
        } else {
            volume = EXPAND_FACTOR * volume + 1;
        }
        if constexpr (std::is_same_v<allocator_type, CAllocAllocator<T>>) {
            // std::cout << "[debug] CAllocAllocator 'Specified' Implementation of AbstractVector::expand" << "\r\n";       //debug
            alloc.reallocate(data, volume);
        } else {
            T *neoData = a_t_t::allocate(alloc, volume);
            if (data)
                if constexpr (std::is_trivially_copyable_v<value_type>)
                    memcpy(reinterpret_cast<void *>(neoData), reinterpret_cast<const void *>(data), sizeof(value_type) * size);
                else
                    for (size_t i = 0; i < size; ++i)
                        assignThanConstruct<value_type&&, std::false_type::value>(&neoData[i], std::move(data[i]));
            deleteAll();
            data = neoData;
        }
    }
    void factorExpand(size_t requestedExtra) {
        std::ptrdiff_t expandVolume = size * (EXPAND_FACTOR - 1) + 1;
        size_t expandTimes = requestedExtra / expandVolume + 1;
        expand(expandTimes * expandVolume);
    }
    void deleteAll() noexcept {
        if (data) {
            if constexpr (!std::is_trivially_destructible_v<value_type>)
                for (size_t i = 0; i < size; ++i) destroy(&data[i]);
            a_t_t::deallocate(alloc, data, volume);
            data = nullptr;
        }
    }
    void destroy(value_type *p) noexcept {
        if constexpr (!std::is_trivially_destructible_v<value_type>) a_t_t::destroy(alloc, p); 
    }
    template <class _Iterator>
    void destroy(const _Iterator& iter) noexcept {
        if constexpr (!std::is_trivially_destructible_v<value_type>) a_t_t::destroy(alloc, iter.operator->()); 
    }
    template <class... _Args>
    void construct(value_type *p, _Args&&... args) {
        a_t_t::construct(alloc, p, std::forward<_Args>(args)...);
    }
    template <class... _Args>
    void reconstruct(value_type *p, _Args&&... args) {
        destroy(p);
        construct(p, std::forward<_Args>(args)...);
    }
    template <class U, bool _Cond = std::is_assignable_v<value_type, U>>
    void assignThanReconstruct(value_type *original, U&& u) {
        if constexpr (_Cond) {
            *original = std::forward<U>(u);
        } else {
            reconstruct(original, std::forward<U>(u));
        }
    }
    template <class U, bool _Cond = std::is_assignable_v<value_type, U>>
    void assignThanConstruct(value_type *p, U&& u) {
        if constexpr (_Cond) {
            *p = std::forward<U>(u);
        } else {
            construct(p, std::forward<U>(u));
        }
    }
    template <class _Iterator, bool _Cond = std::is_assignable_v<value_type, decltype(*std::declval<_Iterator>())>>
    void copyThanReconstruct(_Iterator first, _Iterator last, AbstractVectorIterator<value_type> pos) {
        if constexpr (_Cond && HI_COMPAT) {
            std::copy(first, last, pos);
        } else {
            while (first != last) {
                reconstruct(pos.operator->(), *first);
                ++first;
                ++pos;
            }
        }
    }
    template <class _Iterator, bool _Cond = std::is_assignable_v<value_type, decltype(*std::declval<_Iterator>())>>
    void moveThanReconstruct(_Iterator first, _Iterator last, AbstractVectorIterator<value_type> pos) {
        if constexpr (_Cond && HI_COMPAT) {
            std::move(first, last, pos);
        } else {
            while (first != last) {
                reconstruct(pos.operator->(), *first);
                ++first;
                ++pos;
            }
        }
    }
    template <class _Iterator, bool _Cond = std::is_assignable_v<value_type, decltype(*std::declval<_Iterator>())>>
    void moveBackwardThanReconstruct(_Iterator first, _Iterator last, AbstractVectorIterator<value_type> pos) {
        if constexpr (_Cond && HI_COMPAT) {
            std::move_backward(first, last, pos);
        } else {
            while (first != last) {
                --last;
                --pos;
                reconstruct(pos.operator->(), std::move(*last));
            }
        }
    }
public:
    AbstractVector(): alloc(), data(nullptr), size(0), volume(0) {}
    AbstractVector(const T* const& data, size_t size): alloc(), data(size? a_t_t::allocate(alloc, size): nullptr), size(size), volume(size) {
        if (data) {
            if constexpr (std::is_trivially_copyable_v<T>)
                std::memmove(reinterpret_cast<void *>(this->data), reinterpret_cast<const void *>(data), sizeof(T) * size);
            else
                for (size_t i = 0; i < size; ++i) construct(&this->data[i], data[i]);
        }
            
    }
    AbstractVector(const AbstractVector& v, size_t i): alloc(v.alloc), data(a_t_t::allocate(alloc, std::max(i, v.volume))), size(v.size), volume(std::max(i, v.volume)) {
        if constexpr (std::is_trivially_copyable_v<value_type>)
            std::memcpy(reinterpret_cast<void *>(data), reinterpret_cast<const void *>(v.data), sizeof(value_type) * v.size);
        else
            for (size_t i = 0; i < size; ++i) construct(&data[i], v.data[i]);
    }
    AbstractVector(const AbstractVector& v): AbstractVector(v, 0) {
        // std::cout << "[debug] Copy Constructor Called" << "\r\n";       //debug
    }
    AbstractVector(AbstractVector&& v) noexcept: alloc(std::move(v.alloc)), data(v.data), size(v.size), volume(v.volume) {
        v.data = nullptr;
        v.size = 0;
        v.volume = 0;
    }
    AbstractVector(std::initializer_list<T> l): data(a_t_t::allocate(alloc, l.size())), size(l.size()), volume(l.size()) {
        for (size_t i = 0; i < size; ++i)
            assignThanConstruct<const value_type&, std::is_trivially_copyable_v<value_type>>(&data[i], l.begin()[i]);
    }
    AbstractVector(ssize_t size, const T& t): data(a_t_t::allocate(alloc, size)), size(size), volume(size) {
        if constexpr (std::is_trivially_copyable_v<T>)
            std::fill(data, data + size, t);
        else
            for (size_t i = 0; i < size; ++i)
                construct(data + i, t);
    }
    AbstractVector(ssize_t size): data(a_t_t::allocate(alloc, size)), size(size), volume(size) {
        if constexpr (!std::is_trivially_copyable_v<T>)
            for (size_t i = 0; i < size; ++i)
                construct(data + i);
    }
    AbstractVector& operator= (const AbstractVector& v) {
        if (data == v.data) return *this;
        if (v.size > volume) {
            factorExpand(v.size - volume);
            // std::ptrdiff_t expandVolume = size * (EXPAND_FACTOR - 1) + 1;
            // size_t expandTimes = (v.size - volume) / expandVolume + 1;
            // expand(expandTimes * expandVolume);
        }
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memcpy(reinterpret_cast<void *>(data), reinterpret_cast<const void *>(v.data), sizeof(value_type) * v.size);
        } else {
            copyThanReconstruct(v.cbegin(), v.cbegin() + std::min(v.size, size), begin());
            if (size < v.size)
                for (size_t i = size; i < v.size; ++i)
                    construct(&data[i], v.data[i]);
        }
        size = v.size;
        return *this;
    }
    AbstractVector& operator= (AbstractVector&& v) noexcept {
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
    template <class U, typename = std::enable_if_t<std::is_constructible_v<T, U>>>
    void pushBack(U&& element) {
        if (size == volume) {
            expand();
        }
        assignThanConstruct<decltype(std::forward<U>(element)), std::is_trivially_copyable_v<value_type>>(end().operator->(), std::forward<U>(element));
        ++size;
    }
    template <class... _Args, typename = std::enable_if_t<std::is_constructible_v<T, _Args...>>>
    void emplaceBack(_Args&&... args) {
        if (size == volume) {
            expand();
        }
        construct(end().operator->(), std::forward<_Args>(args)...);
        ++size;
    }
    template <class U, typename = std::enable_if_t<std::is_constructible_v<T, U>>>
    AbstractVectorIterator<T> insert(AbstractVectorIterator<T> pos, U&& element) {
        // pos == end() starts a new logic for some tricky handling reasons
        if (pos == end()) {
            pushBack(std::forward<U>(element));
            return end();
        }
        if constexpr (!OPTIMIZE) {
            if (pos > end()) throw std::out_of_range{"range out of bound"};;
        }
        if (size == volume) {
            std::ptrdiff_t distance = pos - begin();
            expand();
            pos = begin() + distance;
        }
        if constexpr (std::is_trivially_copyable_v<value_type>) {
            std::memmove(reinterpret_cast<void *>((pos + 1).operator->()), reinterpret_cast<const void *>(pos.operator->()), sizeof(value_type) * (end() - pos));
        } else {
            construct(end().operator->(), std::move(*(end() - 1)));
            std::move_backward(pos, end() - 1, end());
        }
        assignThanReconstruct(pos.operator->(), std::forward<U>(element));
        ++size;
        return AbstractVectorIterator<T>(pos);
    }
    template <class _InputIter>
    AbstractVectorIterator<T> insert(AbstractVectorIterator<T> pos, _InputIter first, _InputIter last) {
        size_t inputSize = last - first;
        if constexpr (!OPTIMIZE) {
            if (first > last) throw std::runtime_error{"invalid input of first and last"};
            if (pos > end()) throw std::out_of_range{"iterator pos exceeds end"};
        }
        if (!inputSize) return pos;
        if (size + inputSize > volume) {
            std::ptrdiff_t distance = pos - begin();
            factorExpand(inputSize);
            pos = begin() + distance;
        }
        if constexpr (std::is_trivially_copyable_v<value_type>) {
            std::memmove(reinterpret_cast<void *>((pos + inputSize).operator->()), reinterpret_cast<void *>(pos.operator->()), sizeof(value_type) * (end() - pos));
            if constexpr (
                std::is_same_v<
                    typename std::conditional_t<
                        std::is_pointer_v<std::decay_t<_InputIter>>
                        , AbstractVectorIterator<T>
                        , _InputIter
                    >::iterator_category
                    , std::random_access_iterator_tag
                >
                && HI_COMPAT
            )
                std::memmove(reinterpret_cast<void *>(pos.operator->()), reinterpret_cast<const void *>(&*first), sizeof(value_type) * inputSize);
            else {
                for(auto pos1 = pos; first != last; ++pos1, ++first) {
                    std::memmove(reinterpret_cast<void *>(pos1.operator->()), reinterpret_cast<const void *>(&*first), sizeof(value_type));
                }
            }
                // std::copy(first, last, pos);        // std::copy may boil down to memmove, which is good
        } else {
            std::ptrdiff_t toShift = end() - pos;
            std::ptrdiff_t toMoveConstruct = inputSize > toShift? toShift: inputSize;
            std::ptrdiff_t toMoveAssign = inputSize >= toShift? 0: toShift - inputSize;
            std::ptrdiff_t toConstruct = inputSize > toShift? inputSize - toShift: 0;
            std::ptrdiff_t toAssign = toMoveConstruct;
            for (size_t i = 0; i < toMoveConstruct; ++i)
                construct((end() - 1 + inputSize - i).operator->(), std::move(*(end() - 1 - i)));
            if (toMoveAssign)
                moveBackwardThanReconstruct(pos, pos + toMoveAssign, end());
            for (size_t i = 0; i < toConstruct; ++i)
                construct((end() - 1 + toConstruct - i).operator->(), *(last - 1 - i));
            if (toAssign)
                copyThanReconstruct(first, first + toAssign, pos);
        }
        size += inputSize;
        return pos;
    }
    template <class... _Args, typename = std::enable_if_t<std::is_constructible_v<T, _Args...>>>
    AbstractVectorIterator<T> emplace(AbstractVectorIterator<T> pos, _Args&&... args) {
        // pos == end() starts a new logic for some tricky handling reasons
        if (pos == end()) {
            emplaceBack(std::forward<_Args>(args)...);
            return end();
        }
        if constexpr (!OPTIMIZE) {
            if (pos > end()) throw std::out_of_range{"range out of bound"};;
        }
        if (size == volume) {
            std::ptrdiff_t distance = pos - begin();
            expand();
            pos = begin() + distance;
        }
        if constexpr (std::is_trivially_copyable_v<value_type>) {
            std::copy_backward(reinterpret_cast<char *>(pos.operator->()), reinterpret_cast<char *>(end().operator->()), reinterpret_cast<char *>((end() + 1).operator->()));
        } else {
            construct(end().operator->(), std::move(*(end() - 1)));
            std::move_backward(pos, end() - 1, end());
        }
        reconstruct(pos.operator->(), std::forward<_Args>(args)...);
        ++size;
        return AbstractVectorIterator<T>(pos.operator->());
    }
    void resize(size_t neoSize) {       // new values are garbage values for trivially copyable types
        if (neoSize == size) return;
        if (neoSize < size) erase(begin() + neoSize, end());
        if (neoSize > size) {
            if (volume < neoSize)
                factorExpand(neoSize - volume);
            if constexpr (!std::is_trivially_copyable_v<value_type>) {
                for (size_t i = size; i < neoSize; ++i)
                    construct(data + i);
            }
        }
        size = neoSize;
    }
    void resize(size_t neoSize, const value_type& value) {
        if (neoSize == size) return;
        if (neoSize < size) erase(begin() + neoSize, end());
        if (neoSize > size) {
            if (volume < neoSize)
                factorExpand(neoSize - volume);
            if constexpr (std::is_trivially_copyable_v<value_type>) 
                std::fill(begin() + size, begin() + neoSize, value);
            else {
                for (size_t i = size; i < neoSize; ++i)
                    construct(data + i, value);
            }
        }
        size = neoSize;
    }
    void popBack() {
        if (size > 0) {
            destroy(&data[size - 1]);
            --size;
        }
    }
    void clear() {
        deleteAll();
        size = 0;
        volume = 0;
    }
    AbstractVectorIterator<T> erase(const AbstractVectorIterator<T>& first, const AbstractVectorIterator<T>& last) {      // passing last exceeding end() is invalidated
        if constexpr (!OPTIMIZE) {
            if (last > end()) throw std::out_of_range{"iterator last exceeds end"};;
        }
        if (first < last) {
            if constexpr (std::is_trivially_copyable_v<value_type>)
                std::memcpy(reinterpret_cast<void *>(first.operator->()), reinterpret_cast<void *>(last.operator->()), sizeof(value_type) * (end() - last));
            else 
                moveThanReconstruct(last, end(), first);
            for (size_t i = size - (last - first); i < size; ++i) destroy(&data[size - 1]);
            size -= last - first;
        }
        return first;
    }
    AbstractVectorIterator<T> erase(const AbstractVectorIterator<T>& pos) {        // passing end() as pos is invalidated, which is the same policy as the std::vector
        if constexpr (!OPTIMIZE) {
            if (pos >= end()) throw std::out_of_range{"pos exceeds the last element address"};;
        }
        AbstractVectorIterator<T> nextPos = pos + 1;
        if (pos < end()) {
            if constexpr (std::is_trivially_copyable_v<value_type>)
                std::memcpy(reinterpret_cast<void *>(pos.operator->()), reinterpret_cast<void *>(nextPos.operator->()), sizeof(value_type) * (end() - nextPos));
            else 
                std::move(nextPos, end(), pos);
            destroy(&data[size - 1]);
            --size;
        }
        return pos;
    }
    void shrinkToFit() {
        if (!volume || size == volume) return;
        value_type *neoData = a_t_t::allocate(alloc, size);
        if constexpr (std::is_trivially_copyable_v<value_type>) 
            memcpy(reinterpret_cast<void *>(neoData), reinterpret_cast<const void *>(data), sizeof(value_type) * size);
        else
            for (size_t i = 0; i < size; ++i)
                construct(neoData + i, std::move(*(data + i)));
        deleteAll();
        data = neoData;
        volume = size;
    }

    virtual AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>&) = 0;       // vector:数值加；string:追加
    template<class _Alloc>
    AbstractVector<T, Alloc>& operator<<(const AbstractVector<T, _Alloc>& v) {
        insert(end(), v.cbegin(), v.cend());
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
    // void set(int dex, int minterm) {
    //     data[dex] = minterm;
    // }
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
    using AbstractVector<T, Alloc>::operator<<;
    DebugVector() = default;
    DebugVector(const T* const& data, size_t size): AbstractVector<T, Alloc>(data, size) {}
    DebugVector(const DebugVector& v): AbstractVector<T, Alloc>(v) {}
    AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>&) override { return *this; }       // vector:数值加；string:追加
};

template <class T, class Alloc = std::allocator<T>>
class CollectionVector: public AbstractVector<T, Alloc> {
public:
    using AbstractVector<T, Alloc>::operator<<;
    CollectionVector() = default;
    CollectionVector(const T* const& data, size_t size): AbstractVector<T, Alloc>(data, size) {}
    CollectionVector(const CollectionVector& v, size_t i): AbstractVector<T, Alloc>(v, i) {}
    CollectionVector(std::initializer_list<T> l): AbstractVector<T, Alloc>(l) {}
    CollectionVector(ssize_t size, const T& t): AbstractVector<T, Alloc>(size, t) {}
    CollectionVector(ssize_t size): AbstractVector<T, Alloc>(size) {}
    AbstractVector<T, Alloc>& operator+=(const AbstractVector<T, Alloc>& v) override { 
        return *this << v;
    }
    ~CollectionVector() noexcept override = default;
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
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
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
    AbstractVectorReversedIterator& operator+= (ssize_t n) {
        this->ptr -= n;
        return *this;
    }
    AbstractVectorReversedIterator& operator-= (ssize_t n) {
        this->ptr += n;
        return *this;
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
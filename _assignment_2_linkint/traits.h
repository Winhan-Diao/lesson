#pragma once
#include <type_traits>

template <class T>
struct is_runnable {
private:
    static constexpr auto test(int) -> decltype(std::declval<T>()(), std::true_type());
    static constexpr auto test(...) -> std::false_type;
public:
    static constexpr bool value = decltype(test(std::declval<int>()))::value;
};

template <class T, class P>
struct is_consumer {
private:
    static constexpr auto test(int) -> decltype(std::declval<T>()(std::declval<P>()), std::true_type());
    static constexpr auto test(...) -> std::false_type;
public:
    static constexpr bool value = decltype(test(std::declval<int>()))::value;
};

template <class T>
struct clean_type {
    using type = std::decay_t<std::remove_pointer_t<std::decay_t<T>>>;
};

template <class T>
using clean_type_t = typename clean_type<T>::type;

/*
expected behavior:
In Arguments List -> In Function Body (-> Derefernce To If Pointer) -> As Return Type
T&& ->T -> T&
const T&& -> const T -> const T&
T& -> T& -> T&
const T& -> const T& -> const T&
T*&& -> T* (-> T) -> T&
T*& -> T*& (-> T) -> T&
T*const -> T*const (-> T) -> T&
const T*&& -> const T* (-> const T) -> const T&
const T*& -> const T* (-> const T) -> const T&
*/
template <class T, class U = std::remove_pointer_t<std::remove_reference_t<T>>>
constexpr U& toReference(T&& t) {
    if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
        return *t;
    } else {
        return t;
    }
}
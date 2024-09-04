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
struct remove_cv_ref_ptr_ptrref {
    using type = std::decay_t<std::remove_pointer_t<std::decay_t<T>>>;
};

template <class T>
using remove_cv_ref_ptr_ptrref_t = typename remove_cv_ref_ptr_ptrref<T>::type;

template<class T>
struct ptr_to_ref_while_forwarding {
    using type = std::conditional_t<std::is_lvalue_reference_v<T>, T, T&&>;
};

template<class T>
struct ptr_to_ref_while_forwarding<T *> {
    using type = std::add_lvalue_reference_t<T>;
};

template<class T>
struct ptr_to_ref_while_forwarding<T *&> {
    using type = std::add_lvalue_reference_t<T>;
};

template<class T>
struct ptr_to_ref_while_forwarding<T *const> {
    using type = std::add_lvalue_reference_t<T>;
};

template<class T>
struct ptr_to_ref_while_forwarding<T *const&> {
    using type = std::add_lvalue_reference_t<T>;
};

/*
expected behavior:
In Arguments List -> In Function Body (-> Derefernce To If Pointer) -> As Return Type
T&& ->T -> T&&
const T&& -> const T -> const T&&
T& -> T& -> T&
const T& -> const T& -> const T&
T*&& -> T* (-> T) -> T&
T*& -> T*& (-> T) -> T&
T*const -> T*const (-> T) -> T&
const T*&& -> const T* (-> const T) -> const T&
const T*& -> const T* (-> const T) -> const T&
*/
template <class T, class U = typename ptr_to_ref_while_forwarding<T>::type>
constexpr U toReference(T&& t) {
    if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
        return *t;
    } else {
        return t;
    }
}
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
    static constexpr auto test(int) -> decltype(std::declval<T>()(std::ref(std::declval<P>())), std::true_type());
    static constexpr auto test(...) -> std::false_type;
public:
    static constexpr bool value = decltype(test(std::declval<int>()))::value;
};
#pragma once

template <class T, class... Args>
struct can_take {
private:
    static constexpr auto test(int) -> decltype(std::declval<T>()(std::declval<Args>()...), std::true_type());
    static constexpr auto test(...) -> std::false_type;
public:
    static constexpr bool value = decltype(test(0))::value;
};

// // Require C++17
// template <class _To, class... _From>
// struct all_convertible {
//     static constexpr bool value = (std::is_constructible_v<_From, _To> && ...);
// };

template <class _To, class... _From>
struct all_convertible;

template <class _To, class _From>
struct all_convertible<_To, _From> {
    static constexpr bool value = std::is_convertible_v<_From, _To>;
};

template <class _To, class _From, class... _Args>
struct all_convertible<_To, _From, _Args...> {
    static constexpr bool value = std::is_convertible_v<_From, _To> && all_convertible<_To, _Args...>::value;
};


#pragma once

template <class T, class... Args>
struct can_take {
private:
    static constexpr auto test(int) -> decltype(std::declval<T>()(std::declval<Args>()...), std::true_type());
    static constexpr auto test(...) -> std::false_type;
public:
    static constexpr bool value = decltype(test(0))::value;
};


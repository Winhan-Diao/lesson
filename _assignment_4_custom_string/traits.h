#pragma once
#include <type_traits>
#include <string>

namespace ctt {
    template <class T>
    struct is_valid_to_string {
    private:
        template <class U>
        static constexpr auto test(int) -> decltype(std::to_string(std::declval<U>()), std::true_type());
        template <class>
        static constexpr auto test(...) -> std::false_type;
    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

    template <class T> 
    static constexpr bool is_valid_to_string_v = is_valid_to_string<T>::value;
};


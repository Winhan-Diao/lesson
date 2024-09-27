#pragma once

template <class T, class... Args>
struct can_take {       // 编译时执行，如果T能接Args...为参数，就返回真，否则假
private:
    static constexpr auto test(int) -> decltype(std::declval<T>()(std::declval<Args>()...), std::true_type());      // 先跑这个，使用","运算符先跑让T接受Args...类的假对象，若通过判断则返回逗号后的真值的对象的类
    static constexpr auto test(...) -> std::false_type;     // 上一个test在substitution failure is not an error失败后才会跑这个重载，因为参数列表...比int低一级
public:
    static constexpr bool value = decltype(test(0))::value;     // 布尔value的值是执行test(0)的结果，同样是编译时就知道了。
};

// // Require C++17
// template <class _To, class... _From>
// struct all_convertible {
//     static constexpr bool value = (std::is_constructible_v<_From, _To> && ...);
// };

template <class _To, class... _From>
struct all_convertible;     // 编译时执行，如果所有_From...的类型能转换为_To的类型，就返回真，否则假。（PS：这个原始类永远不会执行到，因为没有以下两个具体）

template <class _To, class _From>
struct all_convertible<_To, _From> {        // _Args...被抽没了，就跑这个了，递归将终止。
    static constexpr bool value = std::is_convertible_v<_From, _To>;
};

template <class _To, class _From, class... _Args>
struct all_convertible<_To, _From, _Args...> {      // 从_Args...里面抽取一个，进行_From是否能转为_To的判断
    static constexpr bool value = std::is_convertible_v<_From, _To> && all_convertible<_To, _Args...>::value;       // 若不能转换，则直接返回假；若能转换，则对_Args...的接下来的一个类进行判断，是递归。
};


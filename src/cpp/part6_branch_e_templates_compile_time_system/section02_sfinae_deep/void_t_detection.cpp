// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E2 SFINAE 深水)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : void_t_detection
// Topic id : part6/e/section02/void_t_detection
//
// 要点: void_t 把"类型是否合法"变成偏特化开关; 是 detection idiom 的核心。
// 参考: [meta.trans.other] Walter Brown CppCon 2016; cppreference void_t

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// 探测嵌套类型
template <typename T, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

// 探测成员函数
template <typename T, typename = void>
struct has_size : std::false_type {};

template <typename T>
struct has_size<T, std::void_t<decltype(std::declval<const T&>().size())>> : std::true_type {};

// 探测二元表达式
template <typename T, typename U, typename = void>
struct are_equality_comparable : std::false_type {};

template <typename T, typename U>
struct are_equality_comparable<T, U, std::void_t<decltype(std::declval<const T&>() == std::declval<const U&>())>>
    : std::true_type {};

// 探测可调用 (void_t 必须包住 decltype, 避免硬错误)
template <typename AlwaysVoid, typename F, typename... Args>
struct is_callable_impl : std::false_type {};

template <typename F, typename... Args>
struct is_callable_impl<std::void_t<decltype(std::declval<F>()(std::declval<Args>()...))>, F, Args...>
    : std::true_type {};

template <typename F, typename... Args>
using is_callable = is_callable_impl<void, F, Args...>;

// 组合: 有 size 且 value_type 可默认构造
template <typename T>
inline constexpr bool is_sized_container_v = [] {
    if constexpr (has_size<T>::value && has_value_type<T>::value) {
        return std::is_default_constructible_v<typename T::value_type>;
    } else {
        return false;
    }
}();

struct NoSize {
    using value_type = int;
};

struct HasSize {
    std::size_t size() const { return 0; }
};

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E2 void_t detection ===\n";

    static_assert(has_value_type<std::vector<int>>::value);
    static_assert(!has_value_type<int>::value);
    static_assert(has_size<std::vector<int>>::value);
    static_assert(!has_size<int>::value);
    static_assert(has_size<HasSize>::value);
    static_assert(!has_size<NoSize>::value);

    static_assert(are_equality_comparable<int, int>::value);
    static_assert(!are_equality_comparable<int, std::string>::value);

    auto add = [](int a, int b) { return a + b; };
    static_assert(is_callable<decltype(add), int, int>::value);
    static_assert(!is_callable<decltype(add), int>::value);

    static_assert(is_sized_container_v<std::vector<int>>);
    static_assert(!is_sized_container_v<int>);

    std::vector<int> v{1, 2};
    assert(has_size<decltype(v)>::value);
    assert(v.size() == 2);

    std::cout << "  vector has value_type+size; int has neither\n";
    std::cout << "  concepts supersede void_t for constraints,"
                 " but void_t remains useful for trait libraries.\n";
    std::cout << "void_t_detection: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/void_t_detection", run>;

}  // namespace

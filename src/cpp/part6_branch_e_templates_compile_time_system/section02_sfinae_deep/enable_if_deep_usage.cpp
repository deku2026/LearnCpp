// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E2 SFINAE 深水)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section02_sfinae_deep
// Item     : enable_if_deep_usage
// Topic id : part6/e/section02/enable_if_deep_usage
//
// 要点: enable_if 把条件塞进返回类型 / 默认模板参数 / 形参;
//       失败 = 从重载集移除 (SFINAE), 不是硬错误。
// 参考: [temp.deduct] cppreference std::enable_if

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

namespace {

// 位置 1: 返回类型
template <typename T>
std::enable_if_t<std::is_integral_v<T>, const char*> kind_of(T) {
    return "integral";
}

template <typename T>
std::enable_if_t<std::is_floating_point_v<T>, const char*> kind_of(T) {
    return "floating";
}

// 位置 2: 默认模板参数
template <typename T, typename = void>
struct serialize_impl {
    static std::string apply(const T&) { return "fallback"; }
};

template <typename T>
struct serialize_impl<T, std::enable_if_t<std::is_arithmetic_v<T>>> {
    static std::string apply(T v) { return "arith:" + std::to_string(v); }
};

template <typename T>
std::string serialize(const T& v) {
    return serialize_impl<T>::apply(v);
}

// 位置 3: 额外默认形参 (C++11 风格)
template <typename It>
auto distance_if_random(
    It first, It last,
    std::enable_if_t<
        std::is_same_v<typename std::iterator_traits<It>::iterator_category, std::random_access_iterator_tag>, int> = 0)
    -> typename std::iterator_traits<It>::difference_type {
    return last - first;
}

// 否定条件: 非指针走这份
template <typename T>
std::enable_if_t<!std::is_pointer_v<T>, T> decay_copy(T x) {
    return x;
}

template <typename T>
std::enable_if_t<std::is_pointer_v<T>, std::remove_pointer_t<T>> decay_copy(T p) {
    return *p;
}

// enable_if 与 constexpr if 对照 (现代首选 if constexpr)
template <typename T>
std::string modern_kind(T) {
    if constexpr (std::is_integral_v<T>) {
        return "integral-if-constexpr";
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating-if-constexpr";
    } else {
        return "other";
    }
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E2 enable_if deep usage ===\n";

    assert(std::string(kind_of(42)) == "integral");
    assert(std::string(kind_of(3.14)) == "floating");
    std::cout << "  return-type enable_if: " << kind_of(1) << " / " << kind_of(1.0) << '\n';

    assert(serialize(7) == "arith:7");
    assert(serialize(std::string{"hi"}) == "fallback");
    std::cout << "  default-param specialization: " << serialize(7) << " / " << serialize(std::string{"hi"}) << '\n';

    std::vector<int> v{1, 2, 3, 4};
    assert(distance_if_random(v.begin(), v.end()) == 4);

    [[maybe_unused]] int x = 5;
    assert(decay_copy(x) == 5);
    assert(decay_copy(&x) == 5);

    assert(modern_kind(1) == "integral-if-constexpr");
    assert(modern_kind(1.5) == "floating-if-constexpr");
    assert(modern_kind(std::string{}) == "other");

    std::cout << "  prefer concepts / if constexpr for new code;"
                 " enable_if still appears in older libraries.\n";
    std::cout << "enable_if_deep_usage: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section02/enable_if_deep_usage", run>;

}  // namespace

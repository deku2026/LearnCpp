// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E8 variadics)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section08_advanced_variadics
// Item     : variadic_dispatch_patterns
// Topic id : part6/e/section08/variadic_dispatch_patterns
//
// 要点: fold / 索引展开 / overload 递归 / 逗号折叠副作用 等可变参分发模式。
// 参考: [temp.variadic] cppreference parameter pack / fold

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

// 1) 一元右折叠加
template <typename... Ts>
auto sum(Ts... xs) {
    return (xs + ...);
}

// 2) 逗号折叠: 对每个参数调用 f
template <typename F, typename... Ts>
void for_each_arg(F f, Ts&&... xs) {
    (f(std::forward<Ts>(xs)), ...);
}

// 3) 递归 overload 剥 pack (C++11 风格)
template <typename T>
std::string join_rec(const T& x) {
    return std::to_string(x);
}
template <typename T, typename... Rest>
std::string join_rec(const T& x, const Rest&... rest) {
    return std::to_string(x) + "," + join_rec(rest...);
}

// 4) 索引分发
template <typename Tuple, typename F, std::size_t... I>
void for_each_tuple_impl(Tuple&& t, F&& f, std::index_sequence<I...>) {
    (f(std::get<I>(std::forward<Tuple>(t))), ...);
}

template <typename Tuple, typename F>
void for_each_tuple(Tuple&& t, F&& f) {
    constexpr auto N = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    for_each_tuple_impl(std::forward<Tuple>(t), std::forward<F>(f), std::make_index_sequence<N>{});
}

// 5) 类型分发: 第一个匹配的 handler
template <typename T, typename... Fs>
decltype(auto) dispatch(T&& v, Fs&&... fs) {
    // overload 集合
    struct overload : std::decay_t<Fs>... {
        using std::decay_t<Fs>::operator()...;
    };
    overload o{std::forward<Fs>(fs)...};
    return o(std::forward<T>(v));
}

// 6) 编译期计数满足条件
template <typename Pred, typename... Ts>
constexpr std::size_t count_if_types() {
    return (std::size_t{0} + ... + (Pred::template value<Ts> ? 1u : 0u));
}

struct IsIntegral {
    template <typename T>
    static constexpr bool value = std::is_integral_v<T>;
};

// 7) apply-like
template <typename F, typename Tuple>
decltype(auto) apply_tuple(F&& f, Tuple&& t) {
    return std::apply(std::forward<F>(f), std::forward<Tuple>(t));
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E8 variadic dispatch patterns ===\n";

    assert(sum(1, 2, 3, 4) == 10);
    assert(join_rec(1, 2, 3) == "1,2,3");

    std::vector<int> got;
    for_each_arg([&](int x) { got.push_back(x); }, 4, 5, 6);
    assert((got == std::vector<int>{4, 5, 6}));

    int acc = 0;
    for_each_tuple(std::make_tuple(1, 2, 3), [&](int x) { acc += x; });
    assert(acc == 6);

    auto r = dispatch(
        42, [](int x) { return std::string{"i"} + std::to_string(x); }, [](const std::string& s) { return s; });
    assert(r == "i42");

    static_assert(count_if_types<IsIntegral, int, double, char, void*>() == 2);

    [[maybe_unused]] auto prod = apply_tuple([](int a, int b, int c) { return a * b * c; }, std::make_tuple(2, 3, 4));
    assert(prod == 24);

    std::cout << "  folds + index packs + overload sets cover most dispatch needs\n";
    std::cout << "variadic_dispatch_patterns: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section08/variadic_dispatch_patterns", run>;

}  // namespace

// LearnCpp topic
// Doc      : part6-branch-e-templates-compile-time-system.md (E4 TMP)
// Stage    : part6_branch_e_templates_compile_time_system
// Section  : section04_template_metaprogramming
// Item     : compile_time_if_switch
// Topic id : part6/e/section04/compile_time_if_switch
//
// 要点: if constexpr 做编译期分支; 类型 switch 可用 constexpr if 链 /
//       tag dispatch / variant 访问器。
// 参考: [stmt.if] cppreference if constexpr

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace {

// 编译期 if: 丢弃未选分支 (不实例化)
template <typename T>
std::string describe(const T& v) {
    if constexpr (std::is_integral_v<T>) {
        return "intish:" + std::to_string(static_cast<long long>(v));
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floatish";
    } else if constexpr (requires { v.size(); }) {
        return "sized:" + std::to_string(v.size());
    } else {
        return "other";
    }
}

// 编译期 "switch" on type index
template <typename T>
constexpr int type_tag() {
    if constexpr (std::is_same_v<T, int>) {
        return 0;
    } else if constexpr (std::is_same_v<T, double>) {
        return 1;
    } else if constexpr (std::is_same_v<T, std::string>) {
        return 2;
    } else {
        return -1;
    }
}

// tag dispatch 旧式 switch
struct int_tag {};
struct float_tag {};
struct other_tag {};

template <typename T>
using kind_tag = std::conditional_t<std::is_integral_v<T>, int_tag,
                                    std::conditional_t<std::is_floating_point_v<T>, float_tag, other_tag>>;

template <typename T>
int score_impl(T, int_tag) {
    return 1;
}
template <typename T>
int score_impl(T, float_tag) {
    return 2;
}
template <typename T>
int score_impl(T, other_tag) {
    return 0;
}

template <typename T>
int score(T v) {
    return score_impl(v, kind_tag<T>{});
}

// variant 运行时 + 编译期 visitor
using Var = std::variant<int, double, std::string>;

[[maybe_unused]] std::string visit_var(const Var& v) {
    return std::visit(
        [](const auto& x) -> std::string {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, int>) {
                return "i=" + std::to_string(x);
            } else if constexpr (std::is_same_v<T, double>) {
                return "d";
            } else {
                return "s=" + x;
            }
        },
        v);
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== E4 compile-time if / switch ===\n";

    assert(describe(42) == "intish:42");
    assert(describe(1.5) == "floatish");
    assert(describe(std::vector<int>{1, 2, 3}) == "sized:3");
    assert(describe(std::string{"hi"}) == "sized:2");

    static_assert(type_tag<int>() == 0);
    static_assert(type_tag<double>() == 1);
    static_assert(type_tag<void>() == -1);

    assert(score(1) == 1);
    assert(score(1.0) == 2);
    assert(score(std::string{}) == 0);

    assert(visit_var(Var{7}) == "i=7");
    assert(visit_var(Var{std::string{"z"}}) == "s=z");

    std::cout << "  if constexpr discards inactive branches safely\n";
    std::cout << "  tag dispatch still useful for open overload sets\n";
    std::cout << "compile_time_if_switch: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/e/section04/compile_time_if_switch", run>;

}  // namespace

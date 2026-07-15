// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库3 · 3.1 visit / apply
// Stage    : part3_standard_library_systematized
// Section  : section03_utility_types_and_functional
// Item     : std_visit_apply
// Topic id : part3/section03/std_visit_apply
//
// Refs:
//   https://en.cppreference.com/w/cpp/utility/variant/visit
//   https://en.cppreference.com/w/cpp/utility/apply

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <variant>

namespace {

// 通用 overload 模式(C++17)
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section03/std_visit_apply ===\n";

    std::variant<int, std::string> v = 42;
    std::visit([](const auto& x) { std::cout << "[intro] visit: " << x << '\n'; }, v);

    int visited = 0;
    std::visit(overloaded{
                   [&](int x) { visited = x; },
                   [&](const std::string& s) { visited = static_cast<int>(s.size()); },
               },
               v);
    assert(visited == 42);

    v = std::string{"abcd"};
    std::visit(overloaded{
                   [&](int x) { visited = x; },
                   [&](const std::string& s) { visited = static_cast<int>(s.size()); },
               },
               v);
    assert(visited == 4);

    // apply: 展开 tuple 为函数参数
    std::tuple<int, double, std::string> t{1, 2.5, "hi"};
    const int sum_lens = std::apply(
        [](int a, double b, const std::string& c) { return a + static_cast<int>(b) + static_cast<int>(c.size()); }, t);
    assert(sum_lens == 1 + 2 + 2);

    // 多 variant 笛卡尔 visit
    std::variant<int, char> a = 3;
    std::variant<int, char> b = 'x';
    int combo = 0;
    std::visit(
        [&](auto x, auto y) {
            if constexpr (std::is_same_v<decltype(x), int> && std::is_same_v<decltype(y), char>) {
                combo = x + static_cast<int>(y);
            }
        },
        a, b);
    assert(combo == 3 + static_cast<int>('x'));

    // holds_alternative / get_if 对照
    assert(std::holds_alternative<int>(std::variant<int, std::string>{1}));
    const auto* pi = std::get_if<int>(&v);
    assert(pi == nullptr);  // v 当前是 string
    assert(std::get_if<std::string>(&v) != nullptr);

    std::cout << "[advanced] overloaded visitor + apply ok\n";
    std::cout << "[expert] visit multi-variant is cartesian product of alternatives\n";
    std::cout << "std_visit_apply: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section03/std_visit_apply", run>;

}  // namespace

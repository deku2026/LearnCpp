// Topic     : 可变参数模板入门（预告阶段 6）
// Doc       : 第2部分-阶段3 · 步骤 8.2
// cppreference: https://en.cppreference.com/cpp/language/parameter_pack
//               https://en.cppreference.com/cpp/language/fold
//
// 要点: template<class... Args> 参数包；sizeof...(Args)；C++17 折叠表达式展开；
//       类型安全的异构参数列表；完美转发基石（阶段 5/6）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// C++17 折叠：把参数包展开进一个表达式
template <class... Args>
void print_all(const Args&... args) {
    ((std::cout << args << ' '), ...);
    std::cout << '\n';
}

template <class... Args>
auto sum_all(Args... args) {
    return (args + ...);  // 一元右折
}

template <class... Args>
std::size_t count_args(Args&&...) {
    return sizeof...(Args);
}

template <class... Args>
std::string cat_as_strings(const Args&... args) {
    std::ostringstream oss;
    ((oss << args), ...);
    return oss.str();
}

// 递归展开（C++11 风格，对照折叠）
template <class T>
T sum_rec(T v) {
    return v;
}

template <class T, class... Rest>
T sum_rec(T head, Rest... rest) {
    return head + sum_rec(rest...);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [variadic_templates_intro] ===\n";

    // -------------------------------------------------------------------------
    // §入门：参数包 + 折叠打印
    // -------------------------------------------------------------------------
    std::cout << "[intro] print_all: ";
    print_all(1, 2.5, "three", 'x');
    assert(count_args(1, 2, 3) == 3);
    assert(count_args() == 0);
    std::cout << "[intro] sizeof...(Args) counts pack elements\n";

    // -------------------------------------------------------------------------
    // §进阶：折叠求和 vs 递归
    // -------------------------------------------------------------------------
    assert(sum_all(1, 2, 3, 4) == 10);
    assert(sum_rec(1, 2, 3, 4) == 10);
    assert(cat_as_strings("a", 1, 'b') == "a1b");
    assert(count_args(1, 2) == 2);
    std::cout << "[advanced] fold (C++17) vs recursive instantiation (C++11 style)\n";

    // -------------------------------------------------------------------------
    // §专家：与 initializer_list / C 变参对照；转发预告
    // -------------------------------------------------------------------------
    // · initializer_list：同类型运行期序列；
    // · 可变参数模板：编译期包，每元素可不同类型，可做 sizeof...、折叠、转发；
    // · C 变参：运行期、无类型安全。
    // 完美转发模式（阶段 5/6）：
    //   template<class... A> void emplace(A&&... a) { ctor(std::forward<A>(a)...); }
    auto fwd_identity = []<class T>(T&& x) -> T&& { return std::forward<T>(x); };
    int n = 5;
    assert(fwd_identity(n) == 5);
    assert(fwd_identity(std::string{"z"}).size() == 1);

    std::cout << "[expert] packs are type-safe foundation for emplace/forward/tuple\n";
    std::cout << "=== variadic_templates_intro: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section02/variadic_templates_intro", run>;

}  // namespace

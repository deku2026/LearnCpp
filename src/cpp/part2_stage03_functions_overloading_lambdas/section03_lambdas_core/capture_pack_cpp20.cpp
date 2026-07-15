// Topic     : 参数包捕获（C++20）
// Doc       : 第2部分-阶段3 · 步骤 5.3
// cppreference: https://en.cppreference.com/cpp/language/lambda
//               https://en.cppreference.com/cpp/language/parameter_pack
//
// 要点: [...xs = xs] / [&...xs] 捕获包；配合可变参数模板包装器；
//       可 init-capture 包展开。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <tuple>
#include <utility>

namespace {

template <class... Args>
auto make_sum_by_value(Args... args) {
    // 按值 init-capture 整个参数包：语法 [...xs = expr]
    return [... args = std::move(args)] { return (args + ...); };
}

template <class... Args>
auto make_sum_by_ref(Args&... args) {
    // 引用捕获包：名字后接 ...
    return [&args...] { return (args + ...); };
}

template <class F, class... Args>
auto bind_front_lambda(F f, Args... bound) {
    return [f = std::move(f), ... bound = std::move(bound)](auto&&... rest) {
        return f(bound..., std::forward<decltype(rest)>(rest)...);
    };
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [capture_pack_cpp20] ===\n";

    // -------------------------------------------------------------------------
    // §入门：按值捕获包
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto s = make_sum_by_value(1, 2, 3, 4);
    assert(s() == 10);
    std::cout << "[intro] [...args = ...] captures a parameter pack by value\n";

    // -------------------------------------------------------------------------
    // §进阶：引用捕获包
    // -------------------------------------------------------------------------
    int a = 1;
    int b = 2;
    int c = 3;
    [[maybe_unused]] auto r = make_sum_by_ref(a, b, c);
    assert(r() == 6);
    a = 10;
    assert(r() == 15);
    std::cout << "[advanced] [&...args] aliases all pack elements\n";

    // -------------------------------------------------------------------------
    // §专家：模拟 bind_front；与 tuple 对照
    // -------------------------------------------------------------------------
    auto add3 = [](int x, int y, int z) { return x + y + z; };
    [[maybe_unused]] auto add_10_20 = bind_front_lambda(add3, 10, 20);
    assert(add_10_20(5) == 35);

    // 也可用 tuple + apply，但包捕获更直接
    auto t = std::tuple{1, 2, 3};
    [[maybe_unused]] auto from_tuple = [t] { return std::get<0>(t) + std::get<1>(t) + std::get<2>(t); };
    assert(from_tuple() == 6);

    std::cout << "[expert] pack capture enables generic partial application lambdas\n";
    std::cout << "=== capture_pack_cpp20: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/capture_pack_cpp20", run>;

}  // namespace

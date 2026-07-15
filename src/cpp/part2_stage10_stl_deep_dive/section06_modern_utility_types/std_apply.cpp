// Topic     : std::apply —— 把 tuple 展开为函数参数
// Doc       : 第2部分-阶段10 · 步骤 9.3
// cppreference: https://en.cppreference.com/cpp/utility/apply
//
// 要点: apply(f, tuple) ≡ f(get<0>, get<1>, ...); 配 lambda/成员。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

namespace {

int add3(int a, int b, int c) {
    return a + b + c;
}

struct Adder {
    int base;
    int sum(int a, int b) const { return base + a + b; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_apply] ===\n";

    auto t = std::make_tuple(1, 2, 3);
    assert(std::apply(add3, t) == 6);

    // lambda
    const auto prod = std::apply([](int a, int b, int c) { return a * b * c; }, t);
    assert(prod == 6);

    // pair 也行
    assert(std::apply([](int a, int b) { return a - b; }, std::pair{10, 3}) == 7);

    // 带状态
    Adder adder{100};
    auto args = std::make_tuple(1, 2);
    // 需要绑定 this —— 用 lambda 包一层
    const int s = std::apply([&](int a, int b) { return adder.sum(a, b); }, args);
    assert(s == 103);

    // 构造对象
    auto name = std::apply([](std::string a, std::string b) { return a + " " + b; },
                           std::tuple{std::string{"Ada"}, std::string{"Lovelace"}});
    assert(name == "Ada Lovelace");

    std::cout << "[apply] function/lambda/pair expansion OK\n";
    std::cout << "std_apply: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/std_apply", run>;

}  // namespace

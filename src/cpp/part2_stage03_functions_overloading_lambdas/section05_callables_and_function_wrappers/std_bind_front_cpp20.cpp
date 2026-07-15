// Topic     : std::bind_front（C++20）
// Doc       : 第2部分-阶段3 · 步骤 7.5
// cppreference: https://en.cppreference.com/cpp/utility/functional/bind_front
//
// 要点: 绑定最前面的参数；bind_front(f, bound...)(call...) ≡ invoke(f, bound..., call...)；
//       完美转发；意在取代部分 std::bind 场景。

#include "learn/topic_registry.hpp"

#include <cmath>
#include <functional>
#include <iostream>
#include <string>

namespace {

int sub(int a, int b) {
    return a - b;
}

int sum3(int a, int b, int c) {
    return a + b + c;
}

struct Greeter {
    std::string prefix;
    std::string operator()(const std::string& name) const { return prefix + name; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_bind_front_cpp20] ===\n";

    // -------------------------------------------------------------------------
    // §入门：固定第一个参数
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto sub10 = std::bind_front(sub, 10);  // sub10(b) == sub(10, b)
    assert(sub10(3) == 7);
    std::cout << "[intro] bind_front(sub, 10)(3) == 7\n";

    // -------------------------------------------------------------------------
    // §进阶：多参数、成员/仿函数
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto sum_1_2 = std::bind_front(sum3, 1, 2);  // 只再要 c
    assert(sum_1_2(3) == 6);

    Greeter g{"Hi, "};
    auto hi = std::bind_front(g);  // 绑定可调用对象本身的「前端」——此处无额外 bound
    assert(hi("Ada") == "Hi, Ada");

    auto hi2 = std::bind_front(&Greeter::operator(), Greeter{"Yo, "});
    assert(hi2("Bob") == "Yo, Bob");

    // bind_front(pow, 2.0) → power_of_two(exp) = pow(2, exp)；真正 square 用 bind_back
    [[maybe_unused]] auto power_of_two = std::bind_front(static_cast<double (*)(double, double)>(std::pow), 2.0);
    assert(power_of_two(3.0) == 8.0);
    std::cout << "[advanced] multi bound args; member-like callables\n";

    // -------------------------------------------------------------------------
    // §专家：与 lambda 对照、转发
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto sub10_lam = [](int b) { return sub(10, b); };
    assert(sub10_lam(3) == sub10(3));

    // bind_front 传播值类别/异常规格更可预期；不做占位符重排。
    // 需要重排或复杂表达式 → 直接写 lambda。
    int x = 5;
    [[maybe_unused]] auto add_x = std::bind_front(std::plus<>{}, x);
    assert(add_x(7) == 12);

    std::cout << "[expert] prefer bind_front for simple prefix bind; else lambda\n";
    std::cout << "=== std_bind_front_cpp20: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_front_cpp20", run>;

}  // namespace

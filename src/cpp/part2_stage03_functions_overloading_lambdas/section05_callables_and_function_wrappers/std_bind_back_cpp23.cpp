// Topic     : std::bind_back（C++23）
// Doc       : 第2部分-阶段3 · 步骤 7.5
// cppreference: https://en.cppreference.com/cpp/utility/functional/bind_back
//
// 要点: 绑定最后面的参数；bind_back(f, bound...)(call...) ≡ invoke(f, call..., bound...)；
//       适合「固定尾参」、范围适配器管道风格。

#include "learn/topic_registry.hpp"

#include <cassert>
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

std::string wrap(const std::string& s, char L, char R) {
    return L + s + R;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_bind_back_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：固定最后一个参数
    // -------------------------------------------------------------------------
    auto sub_from = std::bind_back(sub, 3);  // sub_from(a) == sub(a, 3)
    assert(sub_from(10) == 7);
    std::cout << "[intro] bind_back(sub, 3)(10) == 7\n";

    // -------------------------------------------------------------------------
    // §进阶：多尾参、与 bind_front 对照
    // -------------------------------------------------------------------------
    auto plus_c = std::bind_back(sum3, 100);  // (a,b) -> a+b+100
    assert(plus_c(1, 2) == 103);

    auto bracket = std::bind_back(wrap, '[', ']');
    assert(bracket("x") == "[x]");

    // pow：bind_back(pow, 2) → square(base) = pow(base, 2)
    auto square = std::bind_back(static_cast<double (*)(double, double)>(std::pow), 2.0);
    assert(square(5.0) == 25.0);

    auto power_of_two = std::bind_front(static_cast<double (*)(double, double)>(std::pow), 2.0);
    assert(power_of_two(3.0) == 8.0);
    std::cout << "[advanced] bind_back vs bind_front on pow: square vs power_of_two\n";

    // -------------------------------------------------------------------------
    // §专家：lambda 等价、管道场景
    // -------------------------------------------------------------------------
    auto sub_from_lam = [](int a) { return sub(a, 3); };
    assert(sub_from_lam(10) == sub_from(10));

    // 范围适配器 / 部分应用尾部选项时常 bind_back；仍优先可读 lambda。
    auto greet = std::bind_back([](const std::string& name, const std::string& mark) { return name + mark; }, "!");
    assert(greet("Ada") == "Ada!");

    std::cout << "[expert] bind_back = suffix partial apply; great with trailing options\n";
    std::cout << "=== std_bind_back_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section05/std_bind_back_cpp23", run>;

}  // namespace

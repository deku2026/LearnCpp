// Topic     : C++23 省略空参数列表 ()（P1102）
// Doc       : 第2部分-阶段3 · 步骤 6.2
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 无参数时，即使有 mutable/constexpr/noexcept/属性，也可省略 ()；
//       C++23 前带说明符时必须写 ()。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [optional_parameter_list_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：无参本就可省 ()
    // -------------------------------------------------------------------------
    auto a = [] { return 1; };  // 历来合法
    assert(a() == 1);
    std::cout << "[intro] []{ body } already omitted empty parameter list\n";

    // -------------------------------------------------------------------------
    // §进阶：C++23 在带说明符时也可省
    // -------------------------------------------------------------------------
    int n = 0;
    // C++23: [] mutable { ... }  等价于 []() mutable { ... }
    auto counter = [n] mutable { return ++n; };
    assert(counter() == 1);
    assert(counter() == 2);

    auto ce = [] constexpr { return 3; };
    static_assert(ce() == 3);

    auto nx = [] noexcept { return 4; };
    assert(nx() == 4);
    std::cout << "[advanced] [] mutable / constexpr / noexcept without ()\n";

    // -------------------------------------------------------------------------
    // §专家：何时仍必须写 ()
    // -------------------------------------------------------------------------
    // 有参数列表时必须写：(int x)
    auto add1 = [](int x) { return x + 1; };
    assert(add1(41) == 42);

    // 有意写成 () 仍完全合法，团队可统一风格
    auto old_style = []() mutable {
        static int k = 0;
        return ++k;
    };
    assert(old_style() == 1);

    // 与 static 组合（无捕获）
    auto five = [] static { return 5; };
    assert(five() == 5);

    std::cout << "[expert] omit () only when parameter list is empty; style is optional\n";
    std::cout << "=== optional_parameter_list_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/optional_parameter_list_cpp23", run>;

}  // namespace

// Topic     : C++23 省略空参数列表 ()（P1102）
// Doc       : 第2部分-阶段3 · 步骤 6.2
// cppreference: https://en.cppreference.com/cpp/language/lambda
// 提案      : P1102R2
//
// 要点: 无参数时，即使有 mutable/constexpr/noexcept/属性，也可省略 ()；
//       C++23 前带说明符时必须写 ()。

#include "learn/topic_registry.hpp"

#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [optional_parameter_list_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：无参本就可省 ()
    // -------------------------------------------------------------------------
    [[maybe_unused]] auto a = [] { return 1; };  // 历来合法
    assert(a() == 1);
    std::cout << "[intro] []{ body } already omitted empty parameter list\n";

    // -------------------------------------------------------------------------
    // §进阶：C++23 在带说明符时也可省
    // -------------------------------------------------------------------------
    int n = 0;
    // C++23: [] mutable { ... }  等价于 []() mutable { ... }
    [[maybe_unused]] auto counter = [n] mutable { return ++n; };
    assert(counter() == 1);
    assert(counter() == 2);

    auto ce = [] constexpr { return 3; };
    static_assert(ce() == 3);

    [[maybe_unused]] auto nx = [] noexcept { return 4; };
    assert(nx() == 4);

    [[maybe_unused]] auto nd = [] [[nodiscard]] { return 5; };
    assert(nd() == 5);
    std::cout << "[advanced] [] mutable / constexpr / noexcept / attribute without ()\n";

    // -------------------------------------------------------------------------
    // §专家：何时仍要写 ()；与 static 组合
    // -------------------------------------------------------------------------
    // 有参数列表时必须写：(int x)
    [[maybe_unused]] auto add1 = [](int x) { return x + 1; };
    assert(add1(41) == 42);

    // 继续写 () 完全合法——风格上可统一保留
    [[maybe_unused]] auto old_style = []() mutable {
        static int k = 0;
        return ++k;
    };
    assert(old_style() == 1);

    // 与 static 组合：无参数
    [[maybe_unused]] auto five = [] static { return 5; };
    assert(five() == 5);

    // 有尾置返回时：无参可 [] -> int { ... }（() 仍可省）
    [[maybe_unused]] auto six = [] -> int { return 6; };
    assert(six() == 6);

    // 对照：C++20 及更早，[] mutable { } / [] constexpr { } 为 ill-formed，必须 []() mutable
    std::cout << "[expert] omit () only when parameter list is empty; style is optional (P1102)\n";
    std::cout << "=== optional_parameter_list_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/optional_parameter_list_cpp23", run>;

}  // namespace

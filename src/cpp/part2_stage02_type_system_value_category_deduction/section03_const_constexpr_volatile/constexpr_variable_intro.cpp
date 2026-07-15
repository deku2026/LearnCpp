// Topic     : constexpr 变量入门（vs const）
// Doc       : 第2部分-阶段2 · 步骤 3.3
// cppreference: https://en.cppreference.com/cpp/language/constexpr
//               https://en.cppreference.com/cpp/language/constant_expression
//
// 要点: constexpr 要求编译期常量表达式初始化；蕴含 const；
//       可用于数组界、模板实参、case 等；普通 const 可能仅是运行期只读。

#include "learn/topic_registry.hpp"

#include <array>
#include <cassert>
#include <iostream>
#include <type_traits>

namespace {

int runtime_value() {
    return 7;  // 假装运行期才知道
}

constexpr int square(int n) {
    return n * n;  // constexpr 函数：参数是常量表达式时可编译期求值
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [constexpr_variable_intro] ===\n";

    // -------------------------------------------------------------------------
    // 入门：const vs constexpr
    // -------------------------------------------------------------------------
    [[maybe_unused]] const int a = runtime_value();  // 运行期初始化，之后不可改
    constexpr int b = 42;                            // 编译期常量
    // constexpr int c = runtime_value(); // ❌ 不是常量表达式

    assert(a == 7 && b == 42);
    static_assert(b == 42);

    int arr[b];  // OK：b 是常量表达式
    arr[0] = 1;
    assert(arr[0] == 1);
    // int arr2[a]; // 在 ISO C++ 中非法（VLA 不是标准）—— a 非常量表达式
    std::cout << "[intro] constexpr is a core constant; const may be runtime-only\n";

    // -------------------------------------------------------------------------
    // 进阶：constexpr 对象与编译期计算
    // -------------------------------------------------------------------------
    constexpr int s = square(5);
    static_assert(s == 25);

    constexpr std::array<int, 3> kvals{1, 2, 3};
    static_assert(kvals.size() == 3);
    static_assert(kvals[2] == 3);

    // constexpr 变量对对象自身蕴含 const（不可重新赋值）
    // b = 0; // ❌

    // if constexpr 依赖编译期条件（预告）
    if constexpr (b > 0) {
        std::cout << "[advanced] if constexpr branch taken at compile time\n";
    }

    // -------------------------------------------------------------------------
    // 专家：字面类型、静态存储、与 constinit
    // -------------------------------------------------------------------------
    // constexpr 变量若在命名空间作用域，隐含静态存储期，且必须静态初始化成功。
    // C++20 constinit 保证静态初始化但不要求对象是 const。
    // 复杂 constexpr 函数/容器见阶段 7。

    constexpr double pi = 3.141592653589793;
    constexpr double two_pi = pi * 2.0;
    static_assert(two_pi > 6.0 && two_pi < 7.0);

    // 引用可以是 constexpr（绑定到静态存储期对象等规则）
    static constexpr int k = 1;
    constexpr const int& rk = k;
    static_assert(rk == 1);

    std::cout << "[expert] use constexpr for values needed in constant expressions\n";
    std::cout << "=== constexpr_variable_intro: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section03/constexpr_variable_intro", run>;

}  // namespace

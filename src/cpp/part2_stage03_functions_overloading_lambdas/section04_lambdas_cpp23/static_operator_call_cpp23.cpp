// Topic     : 静态 lambda / static operator()（C++23, P1169）
// Doc       : 第2部分-阶段3 · 步骤 6.1 / 验收「静态 lambda + 递归 lambda」
// cppreference: https://en.cppreference.com/cpp/language/lambda
// 提案      : P1169R4
//
// 要点: 无捕获 lambda 可写 static 调用运算符，无隐式 this；利于优化；
//       static 与捕获互斥；未做成「无捕获默认 static」以免破坏既有假设。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [static_operator_call_cpp23] ===\n";

    // -------------------------------------------------------------------------
    // §入门：static 调用运算符
    // -------------------------------------------------------------------------
    auto sq = [](int x) static { return x * x; };
    assert(sq(5) == 25);
    assert(sq(0) == 0);
    assert(sq(-3) == 9);
    std::cout << "[intro] [](int x) static { ... } — no implicit this\n";

    // -------------------------------------------------------------------------
    // §进阶：算法中的无状态比较器 + 与普通无捕获对照
    // -------------------------------------------------------------------------
    std::vector<int> v{3, 1, 4, 1, 5};
    std::sort(v.begin(), v.end(), [](int a, int b) static { return a < b; });
    assert(v.front() == 1 && v.back() == 5);

    // 普通无捕获 lambda 也能用；static 把「无状态」写进类型系统
    auto cmp = [](int a, int b) static { return a > b; };
    std::sort(v.begin(), v.end(), cmp);
    assert(v.front() == 5 && v.back() == 1);

    // 仍可转函数指针（无捕获）
    using Fn = int (*)(int);
    [[maybe_unused]] Fn fp = [](int x) static { return x + 1; };
    assert(fp(41) == 42);
    std::cout << "[advanced] static lambdas as pure stateless predicates; still -> fn ptr\n";

    // -------------------------------------------------------------------------
    // §专家：与捕获互斥；手写仿函数对照；设计取舍
    // -------------------------------------------------------------------------
    // int n = 1;
    // auto bad = [n](int x) static { return x + n; }; // ❌ static 不能访问捕获
    // 未做成「无捕获默认 static」：避免破坏对「成员函数指针 / 闭包布局」的既有代码假设。

    // 手写仿函数对照（C++23 亦允许 static call op）
    struct Sq {
        static int operator()(int x) { return x * x; }
    };
    assert(Sq{}(6) == 36);
    assert(Sq::operator()(7) == 49);  // 可不依赖实例

    // 无捕获非 static 闭包：仍有 operator() 的隐式对象参数（即便体内不用）
    [[maybe_unused]] auto plain = [](int x) { return x * x; };
    assert(plain(5) == 25);
    // 语义等价于有状态？无——但调用约定仍可能带 this；static 明确「纯函数」

    // feature-test：支持 P1169 时通常定义
#if defined(__cpp_static_call_operator)
    std::cout << "[expert] __cpp_static_call_operator=" << __cpp_static_call_operator << '\n';
#else
    std::cout << "[expert] __cpp_static_call_operator not defined (still compiled via c++latest)\n";
#endif

    // 与 move_only_function / function 组合：static lambda 可拷贝、无状态，两者都能装
    std::function<int(int)> f = sq;
    assert(f(4) == 16);

    std::cout << "[expert] static ⇔ no captures; opt-in stateless call operator (P1169)\n";
    std::cout << "=== static_operator_call_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/static_operator_call_cpp23", run>;

}  // namespace

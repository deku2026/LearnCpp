// Topic     : 静态 lambda / static operator()（C++23, P1169）
// Doc       : 第2部分-阶段3 · 步骤 6.1
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 无捕获 lambda 可写 static 调用运算符，无隐式 this；利于优化；
//       static 与捕获互斥。

#include "learn/topic_registry.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
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
    std::cout << "[intro] [](int x) static { ... } — no implicit this\n";

    // -------------------------------------------------------------------------
    // §进阶：算法中的无状态比较器
    // -------------------------------------------------------------------------
    std::vector<int> v{3, 1, 4, 1, 5};
    std::sort(v.begin(), v.end(), [](int a, int b) static { return a < b; });
    assert(v.front() == 1 && v.back() == 5);

    // 普通无捕获 lambda 也能用；static 把「无状态」写进类型系统
    auto cmp = [](int a, int b) static { return a > b; };
    std::sort(v.begin(), v.end(), cmp);
    assert(v.front() == 5);
    std::cout << "[advanced] static lambdas as pure stateless predicates\n";

    // -------------------------------------------------------------------------
    // §专家：与捕获互斥；设计意图
    // -------------------------------------------------------------------------
    // int n = 1;
    // auto bad = [n](int x) static { return x + n; }; // ❌ static 不能访问捕获
    // 未做成「无捕获默认 static」以免破坏 ABI/既有代码对成员函数指针等假设。
    // 手写仿函数对照：
    struct Sq {
        static int operator()(int x) { return x * x; }  // C++23 亦允许 static call op
    };
    assert(Sq{}(6) == 36);

    std::cout << "[expert] static ⇔ no captures; opt-in stateless call operator\n";
    std::cout << "=== static_operator_call_cpp23: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section04/static_operator_call_cpp23", run>;

}  // namespace

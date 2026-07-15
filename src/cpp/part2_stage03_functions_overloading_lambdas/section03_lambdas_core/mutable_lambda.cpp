// Topic     : mutable lambda
// Doc       : 第2部分-阶段3 · 步骤 5.4
// cppreference: https://en.cppreference.com/cpp/language/lambda
//
// 要点: 默认 operator() 为 const，值捕获成员不可改；mutable 去掉 const；
//       改的是闭包内副本，不是外部变量（外部要用 & 捕获）。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [mutable_lambda] ===\n";

    // -------------------------------------------------------------------------
    // §入门：mutable 计数器
    // -------------------------------------------------------------------------
    int n = 0;
    auto counter = [n]() mutable {
        return ++n;  // 改闭包自己的 n
    };
    assert(counter() == 1);
    assert(counter() == 2);
    assert(counter() == 3);
    assert(n == 0);  // 外部不变
    std::cout << "[intro] mutable allows modifying by-value captures inside call op\n";

    // -------------------------------------------------------------------------
    // §进阶：对比引用捕获
    // -------------------------------------------------------------------------
    int m = 0;
    auto outer = [&m] {
        ++m;  // 无需 mutable：改的是外部对象，不是 const 成员
        return m;
    };
    assert(outer() == 1);
    assert(outer() == 2);
    assert(m == 2);

    // 无 mutable 时，值捕获只读：
    // auto bad = [n] { return ++n; }; // ❌
    std::cout << "[advanced] ref capture mutates outer without mutable\n";

    // -------------------------------------------------------------------------
    // §专家：const 对象与拷贝
    // -------------------------------------------------------------------------
    int seed = 5;
    auto c1 = [seed]() mutable { return ++seed; };
    auto c2 = c1;  // 拷贝闭包 → 各自一份状态
    assert(c1() == 6);
    assert(c2() == 6);  // 独立副本从 5 再 ++

    // const 限定的 lambda 对象不能调用 mutable 的 operator()（非 const）
    const auto frozen = [seed]() mutable { return ++seed; };
    // frozen(); // ❌ 若取消注释：const 对象调非 const 成员
    (void)frozen;

    std::cout << "[expert] each closure copy has its own captured state; const lambda object blocks mutable call\n";
    std::cout << "=== mutable_lambda: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage03/section03/mutable_lambda", run>;

}  // namespace

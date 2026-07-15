// LearnCpp topic
// Doc      : 第6部分-支线G · G7 UB 与优化器
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : ub_overview_and_optimizer_implications
// Topic id : part6/g/section03/ub_overview_and_optimizer_implications
//
// 要点: UB 允许编译器假定"永不发生"并据此优化; -O0 碰巧能跑不代表正确。
// 本文件不触发 UB, 只演示安全代码 + 优化友好写法。
// 参考: [defns.undefined] [intro.abstract]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <limits>
#include <vector>

namespace {

// 优化器可删除"若空则死"的分支, 若它能证明指针非空
int deref_checked(const int* p) {
    if (p == nullptr) {
        return -1;  // 明确处理, 非 UB
    }
    return *p;
}

// 有符号溢出是 UB —— 用安全检测
bool safe_add(int a, int b, int& out) {
    if ((b > 0 && a > std::numeric_limits<int>::max() - b) || (b < 0 && a < std::numeric_limits<int>::min() - b)) {
        return false;
    }
    out = a + b;
    return true;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G7 UB overview & optimizer implications ===\n";

    int x = 10;
    assert(deref_checked(&x) == 10);
    assert(deref_checked(nullptr) == -1);

    int sum = 0;
    assert(safe_add(100, 200, sum));
    assert(sum == 300);
    assert(!safe_add(std::numeric_limits<int>::max(), 1, sum));

    // 已定义行为让优化器仍可大显身手
    std::vector<int> v{1, 2, 3};
    int acc = 0;
    for (int e : v) {
        acc += e;
    }
    assert(acc == 6);

    std::cout << "  categories: memory, lifetime, concurrency, arithmetic,"
                 " aliasing, contracts...\n";
    std::cout << "  tools: ASan/UBSan/TSan/MSan, constexpr, static analysis\n";
    std::cout << "  rule: never 'test' UB at -O0 and call it fine\n";
    std::cout << "ub_overview_and_optimizer_implications: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/ub_overview_and_optimizer_implications", run>;

}  // namespace

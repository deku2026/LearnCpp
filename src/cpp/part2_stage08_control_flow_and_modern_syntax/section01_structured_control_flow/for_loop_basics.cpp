// Topic    : 经典 for 循环（三段式）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 1
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section01_structured_control_flow
// Item     : for_loop_basics
// Topic id : part2/stage08/section01/for_loop_basics
// Refs     : https://en.cppreference.com/w/cpp/language/for
//            ISO [stmt.for]；范围 for 见 section02

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <vector>

namespace {

int sum_closed(int lo, int hi) {
    int s = 0;
    for (int i = lo; i <= hi; ++i) {
        s += i;
    }
    return s;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [for_loop_basics] 入门：init / condition / iteration ===\n";
    {
        // for (init; cond; iter) stmt
        // 语义近似：{ init; while (cond) { stmt; iter; } }（continue 时仍执行 iter）
        int sum = 0;
        for (int i = 1; i <= 5; ++i) {
            sum += i;
        }
        assert(sum == 15);
        std::cout << "sum 1..5 = " << sum << '\n';

        // init 可声明多个同类型变量；作用域仅限整个 for。
        int product = 1;
        for (int i = 1, j = 5; i <= 3; ++i, --j) {
            product *= (i + j);  // (1+5)*(2+4)*(3+3)=6*6*6
        }
        assert(product == 216);
        std::cout << "multi-decl init product=" << product << '\n';
    }

    std::cout << "=== 进阶：空段、break/continue、逆序与索引遍历 ===\n";
    {
        // 任意段可空：for (;;) 是死循环惯用写法之一。
        int n = 0;
        for (;;) {
            ++n;
            if (n == 4) {
                break;
            }
        }
        assert(n == 4);

        // continue：跳过本轮剩余语句，但仍执行 iter。
        int odd_sum = 0;
        for (int i = 0; i < 6; ++i) {
            if (i % 2 == 0) {
                continue;
            }
            odd_sum += i;  // 1+3+5
        }
        assert(odd_sum == 9);
        std::cout << "odd_sum with continue=" << odd_sum << '\n';

        // 逆序：注意 unsigned 下溢陷阱 —— 教学用有符号下标。
        std::vector<int> v{10, 20, 30, 40};
        std::vector<int> rev;
        for (int i = static_cast<int>(v.size()) - 1; i >= 0; --i) {
            rev.push_back(v[static_cast<std::size_t>(i)]);
        }
        assert((rev == std::vector<int>{40, 30, 20, 10}));
        std::cout << "reverse index walk ok\n";

        // 迭代器版经典 for（与范围 for 对照，范围 for 见 section02）。
        int total = 0;
        for (auto it = v.begin(); it != v.end(); ++it) {
            total += *it;
        }
        assert(total == 100);

        // 步长不为 1。
        int stride_sum = 0;
        for (int i = 0; i < 10; i += 3) {
            stride_sum += i;  // 0+3+6+9
        }
        assert(stride_sum == 18);
    }

    std::cout << "=== 专家：与 while 等价性、作用域、整数环绕、选用指南 ===\n";
    {
        assert(sum_closed(1, 100) == 5050);

        // init 引入的名字在 cond/iter/stmt 可见，循环外不可见。
        for (int scope_probe = 0; scope_probe < 1; ++scope_probe) {
            assert(scope_probe == 0);
        }
        // scope_probe 此处不可用 —— 作用域正确收口。

        // ⚠️ size_t 逆序经典 UB/死循环：
        // for (std::size_t i = v.size()-1; i >= 0; --i) 在 i==0 再 -- 会绕到极大值。
        // 正确做法：有符号下标、i != npos 风格、或 reverse_iterator / ranges::reverse。
        std::cout << "avoid unsigned countdown: i>=0 is always true for size_t\n";

        // 空 condition ≡ true。
        int hits = 0;
        for (int i = 0; /*true*/; ++i) {
            ++hits;
            if (i >= 2) {
                break;
            }
        }
        assert(hits == 3);  // i=0,1,2

        // 选用：
        // - 已知次数 / 索引算法 → 经典 for
        // - 遍历整容器且不需要下标 → 范围 for（section02）
        // - 事件驱动 / 条件不清次数 → while
        std::cout << "classic for: counting & index algorithms\n";
    }

    std::cout << "[for_loop_basics] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section01/for_loop_basics", run>;

}  // namespace

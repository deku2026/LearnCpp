// Topic    : [[likely]] / [[unlikely]]（C++20）
// Doc      : 第2部分-阶段8-控制流与现代语法增强.md · 步骤 7
// Stage    : part2_stage08_control_flow_and_modern_syntax
// Section  : section03_attributes
// Item     : likely_unlikely_cpp20
// Topic id : part2/stage08/section03/likely_unlikely_cpp20
// Refs     : https://en.cppreference.com/w/cpp/language/attributes/likely
//            提案 P0479；ISO [dcl.attr.likelihood]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

// 热路径：正常除法；冷路径：除零抛错。
int safe_divide(int a, int b) {
    if (b == 0) [[unlikely]] {
        throw std::runtime_error("division by zero");
    }
    return a / b;
}

// switch 上的路径提示：0 最常见。
[[maybe_unused]] const char* bucket(int x) {
    switch (x) {
        [[likely]] case 0:
            return "zero";
        case 1:
            return "one";
        [[unlikely]] case 2:
            return "two";
        default:
            return "other";
    }
}

// 解析循环：合法字符是热路径。
[[maybe_unused]] std::size_t count_digits(const std::string& s) {
    std::size_t n = 0;
    for (char ch : s) {
        if (ch >= '0' && ch <= '9') [[likely]] {
            ++n;
        } else [[unlikely]] {
            // 罕见：非数字，跳过
            continue;
        }
    }
    return n;
}

[[maybe_unused]] int clamp_nonneg(int v) {
    if (v < 0) [[unlikely]] {
        return 0;
    }
    return v;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [likely_unlikely_cpp20] 入门：标注冷热分支 ===\n";
    {
        assert(safe_divide(10, 2) == 5);
        try {
            (void)safe_divide(1, 0);
            assert(false && "should throw");
        } catch (const std::runtime_error& ex) {
            assert(std::string{ex.what()} == "division by zero");
        }
        std::cout << "safe_divide hot path / unlikely zero\n";
    }

    std::cout << "=== 进阶：switch case、循环内提示、语义不变 ===\n";
    {
        assert(std::string{bucket(0)} == "zero");
        assert(std::string{bucket(1)} == "one");
        assert(std::string{bucket(2)} == "two");
        assert(std::string{bucket(9)} == "other");

        assert(count_digits("a1b23") == 3);
        assert(clamp_nonneg(5) == 5);
        assert(clamp_nonneg(-3) == 0);

        // 属性不改变可观察语义：只影响优化器的布局/内联/预测倾向。
        std::vector<int> sample{0, 0, 0, 1, 0};
        [[maybe_unused]] int zeros = 0;
        for (int x : sample) {
            if (x == 0) [[likely]] {
                ++zeros;
            }
        }
        assert(zeros == 4);
        std::cout << "likelihood attributes do not change results\n";
    }

    std::cout << "=== 专家：机制、滥用风险、与 PGO / [[assume]] 边界 ===\n";
    {
        // 机制（实现相关）：编译器把 likely 路径当“热代码”——
        // 可能靠近 fall-through、更积极内联/展开；unlikely 路径可能外移。
        // 在 godbolt 上用 -O2 对比代码布局即可观察（非本 topic 硬断言）。

        // ⚠️ 猜错概率可能损害性能（冷路径被当成热路径排）。
        // 多数时候交给 PGO/FDO 更稳；手写提示留给：
        // - 错误处理 / 异常路径
        // - 协议里“几乎总是成功”的检查
        // - 已用画像证实的热点

        // 与 [[assume]]：
        // - likely/unlikely：概率提示，路径仍必须正确生成
        // - assume：逻辑恒真承诺，假则 UB（危险得多，见 assume topic）

        // 可标注：语句（if 分支体）、标签（case）等；不能当魔法加速普通代码。
        std::cout << "hint optimizer only when probability is known\n";
        std::cout << "prefer PGO for large apps; use unlikely on error paths\n";
    }

    std::cout << "[likely_unlikely_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage08/section03/likely_unlikely_cpp20", run>;

}  // namespace

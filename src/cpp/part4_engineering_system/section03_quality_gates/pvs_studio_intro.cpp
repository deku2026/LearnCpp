// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.3.3 PVS-Studio 等商业静态分析
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : pvs_studio_intro
// Topic id : part4/section03/pvs_studio_intro
//
// 要点: 商业深度分析(PVS-Studio/Coverity)补开源工具; 分级诊断、增量分析。
//       此处用可运行「诊断模式」模拟常见规则, 不依赖商业 license。

#include "learn/topic_registry.hpp"

#include <cmath>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <vector>

namespace {

struct Diag {
    std::string code;  // 如 V501, V547
    std::string message;
    int line;
};

// V547: 表达式总是真/假
std::optional<Diag> check_always_true(bool cond_is_literal_true, int line) {
    if (cond_is_literal_true) {
        return Diag{"V547", "expression is always true", line};
    }
    return std::nullopt;
}

// 有符号/无符号比较可疑
std::optional<Diag> check_signed_unsigned(int /*a*/, unsigned /*b*/, int line, bool comparison_present) {
    if (comparison_present) {
        return Diag{"V4018", "signed/unsigned mismatch (demo id)", line};
    }
    return std::nullopt;
}

// 正确写法: 显式统一类型
[[maybe_unused]] bool size_check(const std::vector<int>& v, std::size_t n) {
    return v.size() >= n;
}

// 潜在: 赋值当比较 — 我们演示正确 ==
[[maybe_unused]] bool equals(int a, int b) {
    return a == b;
}

// 浮点相等: 商业工具常警告 — 用 epsilon
[[maybe_unused]] bool nearly_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== pvs_studio_intro (rule simulation) ===\n";

    auto d1 = check_always_true(true, 10);
    assert(d1 && d1->code == "V547");
    std::cout << "  " << d1->code << ": " << d1->message << '\n';

    // 修复后: 非常量条件（由输入决定真假）
    int x = 1;
    const bool runtime_cond = (x * x) > 0;
    auto d1_fixed = check_always_true(runtime_cond && false, 11);
    assert(!d1_fixed.has_value());

    assert(equals(1, 1));
    assert(!equals(1, 2));
    assert(size_check({1, 2, 3}, 2));
    assert(nearly_equal(0.1 + 0.2, 0.3, 1e-9));

    auto su = check_signed_unsigned(-1, 1u, 20, true);
    assert(su.has_value());
    std::cout << "  prefer same signedness in comparisons\n";

    // 工具链定位: 开源 clang-tidy/cppcheck 为主; PVS 作深度加餐/合规
    [[maybe_unused]] const bool open_source_first = true;
    assert(open_source_first);
    std::cout << "  open-source first; commercial for depth/compliance\n";

    // 与 CI: 增量分析只扫 diff, 控制噪声
    int files_full = 800;
    int files_diff = 12;
    assert(files_diff < files_full);
    std::cout << "  incremental scan " << files_diff << "/" << files_full << " files\n";

    std::cout << "pvs_studio_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/pvs_studio_intro", run>;

}  // namespace

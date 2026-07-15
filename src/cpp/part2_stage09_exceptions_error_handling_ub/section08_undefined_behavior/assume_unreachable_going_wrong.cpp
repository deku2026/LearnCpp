// Topic    : [[assume]] / unreachable 走错即 UB —— 安全契约对照
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 8.2 / 11.2
//            阶段 8 [[assume]]
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section08_undefined_behavior
// Item     : assume_unreachable_going_wrong
// Topic id : part2/stage09/section08/assume_unreachable_going_wrong
// Refs     : https://en.cppreference.com/w/cpp/language/attributes/assume
//            https://en.cppreference.com/w/cpp/utility/unreachable
//            P1774 P0627

#include "learn/topic_registry.hpp"

#include <cassert>
#include <expected>
#include <iostream>
#include <string>
#include <utility>

namespace {

// ✅ 契约用错误处理表达，而不是 assume/unreachable
std::expected<int, std::string> index_of_flag(int flag) {
    switch (flag) {
        case 0:
            return 10;
        case 1:
            return 20;
        case 2:
            return 30;
        default:
            return std::unexpected(std::string{"flag out of domain"});
    }
}

// 仅在已由上层保证 flag∈{0,1,2} 的热路径示意（本文件仍做 assert 护栏）
[[maybe_unused]] int index_hot(int flag) {
    // 教学注释：若写 [[assume(flag>=0 && flag<=2)]]; 或 default: unreachable()
    // 则非法 flag 变成 UB。这里用 assert 保持可运行。
    assert(flag >= 0 && flag <= 2);
    switch (flag) {
        case 0:
            return 10;
        case 1:
            return 20;
        case 2:
            return 30;
    }
    // 逻辑上不可达；若改 std::unreachable()，走错即 UB
    assert(false);
    return -1;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [assume_unreachable_going_wrong] 主干：expected 域检查 ===\n";
    {
        assert(index_of_flag(1) && *index_of_flag(1) == 20);
        auto bad = index_of_flag(5);
        assert(!bad && bad.error().find("domain") != std::string::npos);
        std::cout << "invalid flag -> expected error, not UB\n";
    }

    std::cout << "=== 对抗：热路径在断言护栏下运行 ===\n";
    {
        assert(index_hot(0) == 10);
        assert(index_hot(2) == 30);
        std::cout << "hot path only with validated inputs\n";
    }

    std::cout << "=== 专节：走错即 UB（不触发）===\n";
    // [[assume(cond)]]：编译器可认为 cond 恒真并删检查。
    // std::unreachable()：编译器可认为该点不可达。
    // 两者都是优化契约；输入不满足时不是“返回错误”，而是 UB。
    // 哲学对立：
    //   expected/异常 = “出错了怎么办”
    //   assume/unreachable = “我保证不会错”
    // 只对真正的不变量用；用户输入 / I/O 结果绝不要 assume。
    std::cout << "assume/unreachable wrong path = UB; prefer expected at edges\n";
    (void)&std::unreachable;

    std::cout << "[assume_unreachable_going_wrong] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section08/assume_unreachable_going_wrong", run>;

}  // namespace

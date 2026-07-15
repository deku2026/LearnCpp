// LearnCpp topic
// Doc      : 第6部分-支线G · G10 [[assume]] / unreachable
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : assume_unreachable_misuse_cpp23
// Topic id : part6/g/section03/assume_unreachable_misuse_cpp23
//
// 要点: [[assume(expr)]] 与 std::unreachable() 若条件不成立/到达 = UB。
// 只演示正确用法; 不故意 misuse。
// 参考: P1774; P0627

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <utility>

namespace {

// 调用方保证 n != 0
[[maybe_unused]] int divide_positive(int n, int d) {
    if (d == 0) {
        // 正确: 我们处理了, 不 declare unreachable
        return 0;
    }
    // 已知 d != 0
#if defined(__cpp_lib_unreachable)
    if (d == 0) {
        std::unreachable();  // 对优化器: 此路径不存在
    }
#endif
    return n / d;
}

[[maybe_unused]] int clamp_nonneg(int x) {
    if (x < 0) {
        x = 0;
    }
#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(assume)
    [[assume(x >= 0)]];
#endif
#endif
    return x + 1;
}

enum class Color { Red, Green, Blue };

[[maybe_unused]] const char* color_name(Color c) {
    switch (c) {
        case Color::Red:
            return "red";
        case Color::Green:
            return "green";
        case Color::Blue:
            return "blue";
    }
#if defined(__cpp_lib_unreachable)
    std::unreachable();  // 所有枚举值已覆盖
#else
    return "?";
#endif
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G10 [[assume]] / std::unreachable (correct use) ===\n";

    assert(divide_positive(10, 2) == 5);
    assert(divide_positive(10, 0) == 0);
    assert(clamp_nonneg(-3) == 1);
    assert(clamp_nonneg(2) == 3);
    assert(std::string(color_name(Color::Green)) == "green");

    std::cout << "  misuse: assume false path / reachable unreachable → UB\n";
    std::cout << "  only after you proven invariants\n";
    std::cout << "assume_unreachable_misuse_cpp23: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/assume_unreachable_misuse_cpp23", run>;

}  // namespace

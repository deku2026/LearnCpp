// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.5.1 UBSan
// Stage    : part4_engineering_system
// Section  : section05_sanitizers_and_diagnostics
// Item     : ubsan_overview
// Topic id : part4/section05/ubsan_overview
//
// 要点: 有符号溢出、空指针、移位、对齐等 UB。Linux GCC/Clang 通道。
// 仓库: LEARNCPP_ENABLE_UBSAN; linux-ci preset ON; clang-cl 上常禁用。

#include "learn/topic_registry.hpp"

#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <string_view>

namespace {

bool ubsan_built_in() {
#if defined(__has_feature)
#if __has_feature(undefined_behavior_sanitizer)
    return true;
#endif
#endif
#if defined(__SANITIZE_UNDEFINED__)
    return true;
#endif
    return false;
}

// 安全有符号加: 溢出返回 nullopt(避免 UB)
[[maybe_unused]] std::optional<int> sat_add(int a, int b) {
    if (b > 0 && a > std::numeric_limits<int>::max() - b) {
        return std::nullopt;
    }
    if (b < 0 && a < std::numeric_limits<int>::min() - b) {
        return std::nullopt;
    }
    return a + b;
}

// 安全移位
[[maybe_unused]] std::optional<std::uint32_t> shl_u32(std::uint32_t v, unsigned s) {
    if (s >= 32) {
        return std::nullopt;
    }
    return v << s;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== ubsan_overview ===\n";
    std::cout << "  ubsan instrumented: " << (ubsan_built_in() ? "YES" : "NO") << '\n';

    assert(sat_add(1, 2) == 3);
    assert(!sat_add(std::numeric_limits<int>::max(), 1).has_value());
    std::cout << "  INT_MAX+1 caught by checked add (UBSan would trap raw +)\n";

    assert(shl_u32(1u, 3) == 8u);
    assert(!shl_u32(1u, 40).has_value());

    // 空指针: 不解引用
    [[maybe_unused]] int* p = nullptr;
    assert(p == nullptr);
    // if (*p) UB — UBSan: null pointer

    // 仓库纪律: Windows 本机 ASan; UBSan 放 Linux CI
    std::cout << "  LearnCpp: UBSAN on linux-ci preset, not clang-cl by default\n";

    [[maybe_unused]] const auto flags = std::string_view{"-fsanitize=undefined"};
    assert(flags.find("undefined") != std::string_view::npos);

    std::cout << "ubsan_overview: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section05/ubsan_overview", run>;

}  // namespace

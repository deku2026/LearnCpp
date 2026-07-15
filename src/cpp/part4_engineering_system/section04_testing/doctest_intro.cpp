// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.1 doctest
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : doctest_intro
// Topic id : part4/section04/doctest_intro
//
// 要点: 最轻量、编译快, 可嵌入生产代码旁; CHECK/REQUIRE 风格类似 Catch。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>

namespace {

// 被测: 字符串工具(可与业务代码同 TU 的「嵌入测试」思想)
constexpr bool starts_with_ci(std::string_view s, std::string_view prefix) {
    if (prefix.size() > s.size()) {
        return false;
    }
    for (std::size_t i = 0; i < prefix.size(); ++i) {
        char a = s[i];
        char b = prefix[i];
        if (a >= 'A' && a <= 'Z') {
            a = static_cast<char>(a - 'A' + 'a');
        }
        if (b >= 'A' && b <= 'Z') {
            b = static_cast<char>(b - 'A' + 'a');
        }
        if (a != b) {
            return false;
        }
    }
    return true;
}

struct DoctestStats {
    int checks = 0;
    int failed = 0;
};

#define DT_CHECK(e)                                   \
    do {                                              \
        ++st.checks;                                  \
        if (!(e)) {                                   \
            ++st.failed;                              \
            std::cout << "  CHECK fail: " #e << '\n'; \
        }                                             \
    } while (0)

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== doctest_intro ===\n";
    DoctestStats st{};

    DT_CHECK(starts_with_ci("Hello", "he"));
    DT_CHECK(starts_with_ci("Hello", "HELLO"));
    DT_CHECK(!starts_with_ci("Hello", "hex"));
    DT_CHECK(!starts_with_ci("Hi", "Hello"));

    assert(st.failed == 0);
    assert(st.checks == 4);
    std::cout << "  checks=" << st.checks << '\n';

    // 嵌入思想: DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN 可放 tests;
    // 或 DOCTEST_CONFIG_DISABLE 在生产关闭
    [[maybe_unused]] const bool can_disable_in_prod = true;
    assert(can_disable_in_prod);

    // 编译速度: 单头 + 轻宏 → 适合大量小测试 TU
    std::cout << "  trait: lightest compile among gtest/catch/doctest\n";

    // constexpr 友好: 部分检查可编译期做
    static_assert(starts_with_ci("ABC", "ab"));
    std::cout << "doctest_intro: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/doctest_intro", run>;

}  // namespace

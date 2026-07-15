// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.4.1–4.4.2 GoogleTest
// Stage    : part4_engineering_system
// Section  : section04_testing
// Item     : googletest_basics
// Topic id : part4/section04/googletest_basics
//
// 要点: TEST / EXPECT_* vs ASSERT_*; 失败继续 vs 失败中止。
//       本文件用迷你断言宏模拟 gtest 语义, 不链 libgtest。

#include "learn/topic_registry.hpp"

#include <cmath>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

struct TestStats {
    int passed = 0;
    int failed = 0;
    bool aborted = false;
};

TestStats g_stats{};

#define MINI_EXPECT(cond)                                   \
    do {                                                    \
        if (cond) {                                         \
            ++g_stats.passed;                               \
        } else {                                            \
            ++g_stats.failed;                               \
            std::cout << "  EXPECT failed: " #cond << '\n'; \
        }                                                   \
    } while (0)

#define MINI_ASSERT(cond)                                   \
    do {                                                    \
        if (cond) {                                         \
            ++g_stats.passed;                               \
        } else {                                            \
            ++g_stats.failed;                               \
            g_stats.aborted = true;                         \
            std::cout << "  ASSERT failed: " #cond << '\n'; \
            return;                                         \
        }                                                   \
    } while (0)

// 被测: 简易 parser
std::optional<int> parse_int(std::string_view s) {
    if (s.empty()) {
        return std::nullopt;
    }
    int sign = 1;
    std::size_t i = 0;
    if (s[0] == '-') {
        sign = -1;
        ++i;
    }
    if (i >= s.size()) {
        return std::nullopt;
    }
    int v = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return std::nullopt;
        }
        v = v * 10 + (s[i] - '0');
    }
    return v * sign;
}

void test_parses_valid() {
    auto r = parse_int("42");
    MINI_ASSERT(r.has_value());
    MINI_EXPECT(*r == 42);
}

void test_rejects_invalid() {
    MINI_EXPECT(!parse_int("abc").has_value());
    MINI_EXPECT(!parse_int("").has_value());
}

void test_throw_style() {
    try {
        throw std::runtime_error("boom");
    } catch (const std::runtime_error&) {
        MINI_EXPECT(true);
        return;
    }
    MINI_EXPECT(false);
}

void test_near() {
    double a = 0.1 + 0.2;
    MINI_EXPECT(std::fabs(a - 0.3) < 1e-9);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== googletest_basics (mini EXPECT/ASSERT) ===\n";
    g_stats = {};

    test_parses_valid();
    test_rejects_invalid();
    test_throw_style();
    test_near();

    assert(g_stats.failed == 0);
    assert(g_stats.passed >= 5);
    std::cout << "  passed=" << g_stats.passed << " failed=" << g_stats.failed << '\n';

    // EXPECT 失败不中止: 演示
    g_stats = {};
    MINI_EXPECT(1 == 2);
    MINI_EXPECT(2 == 2);
    assert(g_stats.failed == 1 && g_stats.passed == 1 && !g_stats.aborted);
    std::cout << "  EXPECT continues after failure\n";

    // ASSERT 中止测试体
    g_stats = {};
    auto assert_demo = []() {
        MINI_ASSERT(1 == 2);
        MINI_EXPECT(true);  // 不应执行
    };
    assert_demo();
    assert(g_stats.aborted && g_stats.passed == 0);
    std::cout << "  ASSERT aborts the test body\n";

    std::cout << "googletest_basics: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section04/googletest_basics", run>;

}  // namespace

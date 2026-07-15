// LearnCpp topic
// Doc      : 第4部分-工程系统.md · 4.3.2 clang-tidy
// Stage    : part4_engineering_system
// Section  : section03_quality_gates
// Item     : clang_tidy_setup
// Topic id : part4/section03/clang_tidy_setup
//
// 要点: 分层启用 bugprone/modernize/performance; 需 compile_commands.json。
// 仓库: StaticAnalysis.cmake LEARNCPP_ENABLE_CLANG_TIDY; CMAKE_EXPORT_COMPILE_COMMANDS ON

#include "learn/topic_registry.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace {

// 模拟检查集
struct TidyConfig {
    std::vector<std::string> checks;
    std::vector<std::string> warnings_as_errors;
};

[[maybe_unused]] bool check_enabled(const TidyConfig& c, std::string_view name) {
    for (const auto& ch : c.checks) {
        if (ch == name) {
            return true;
        }
        // 前缀通配 bugprone-*
        if (!ch.empty() && ch.back() == '*') {
            auto prefix = ch.substr(0, ch.size() - 1);
            if (name.starts_with(prefix)) {
                return true;
            }
        }
    }
    return false;
}

// modernize-use-nullptr: 0 作空指针 → 应改 nullptr
[[maybe_unused]] bool looks_like_null_zero(const void* p) {
    return p == nullptr;
}

// performance-unnecessary-copy: 按值传大对象
int sum_bad(std::vector<int> v) {  // 拷贝
    int s = 0;
    for (int x : v) {
        s += x;
    }
    return s;
}
[[maybe_unused]] int sum_good(const std::vector<int>& v) {
    int s = 0;
    for (int x : v) {
        s += x;
    }
    return s;
}

// bugprone: 可疑的悬垂 — 用 string 返回避免
[[maybe_unused]] std::string safe_name() {
    return "ok";
}

int run(int /*argc*/, char** /*argv*/) {
    (void)sum_bad(std::vector<int>{1, 2, 3});

    std::cout << "=== clang_tidy_setup ===\n";

    // --- 入门: 分层配置(文档) ---
    TidyConfig cfg{
        {"bugprone-*", "modernize-*", "performance-*", "cppcoreguidelines-*", "readability-*", "concurrency-*"},
        {"bugprone-*", "performance-*"},
    };
    assert(check_enabled(cfg, "bugprone-use-after-move"));
    assert(check_enabled(cfg, "performance-unnecessary-copy-initialization"));
    assert(check_enabled(cfg, "modernize-use-nullptr"));
    std::cout << "  layered checks enabled (bugprone/modernize/performance)\n";

    // --- 进阶: 现代化写法对照 ---
    [[maybe_unused]] int* p = nullptr;  // 而非 0
    assert(looks_like_null_zero(p));
    auto up = std::make_unique<int>(42);  // modernize-make-unique
    assert(*up == 42);

    std::vector<int> data{1, 2, 3, 4};
    assert(sum_good(data) == sum_bad(data));
    std::cout << "  prefer const& over copy for large objects\n";

    // --- 专家: 仓库接入 ---
    // LEARNCPP_ENABLE_CLANG_TIDY → CXX_CLANG_TIDY property
    // compile_commands.json 由 CMAKE_EXPORT_COMPILE_COMMANDS + sync target
    [[maybe_unused]] const bool needs_compile_commands = true;
    assert(needs_compile_commands);
    assert(safe_name() == "ok");

    // 逐步启用: 一次全开会淹没 — 先 bugprone + performance
    std::vector<std::string> phase1{"bugprone-*", "performance-*"};
    assert(phase1.size() == 2);
    std::cout << "  phase1: bugprone + performance as Werror candidates\n";

    std::cout << "clang_tidy_setup: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part4/section03/clang_tidy_setup", run>;

}  // namespace

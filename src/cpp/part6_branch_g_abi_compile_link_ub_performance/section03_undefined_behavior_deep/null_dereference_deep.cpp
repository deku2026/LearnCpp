// LearnCpp topic
// Doc      : 第6部分-支线G · 空指针解引用
// Stage    : part6_branch_g_abi_compile_link_ub_performance
// Section  : section03_undefined_behavior_deep
// Item     : null_dereference_deep
// Topic id : part6/g/section03/null_dereference_deep
//
// 要点: 解引用空指针 = UB; 用检查 / optional / 引用保证非空。
// 不触发 UB。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>

namespace {

[[maybe_unused]] int value_or(const int* p, int fallback) {
    if (p == nullptr) {
        return fallback;
    }
    return *p;
}

[[maybe_unused]] int require_nonnull(const int& r) {
    return r;
}

std::optional<int> parse_positive(const char* s) {
    if (s == nullptr || *s == '\0') {
        return std::nullopt;
    }
    int v = 0;
    for (const char* p = s; *p; ++p) {
        if (*p < '0' || *p > '9') {
            return std::nullopt;
        }
        v = v * 10 + (*p - '0');
    }
    return v;
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== G null dereference (safe patterns) ===\n";

    [[maybe_unused]] int x = 5;
    assert(value_or(&x, -1) == 5);
    assert(value_or(nullptr, -1) == -1);
    assert(require_nonnull(x) == 5);

    [[maybe_unused]] auto a = parse_positive("42");
    assert(a && *a == 42);
    assert(!parse_positive(nullptr));
    assert(!parse_positive(""));

    // 危险 (不要做): int* p = nullptr; *p;
    std::cout << "  prefer references / optional / gsl::not_null style APIs\n";
    std::cout << "  UBSan: -fsanitize=null\n";
    std::cout << "null_dereference_deep: OK\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part6/g/section03/null_dereference_deep", run>;

}  // namespace

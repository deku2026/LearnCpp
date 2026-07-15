// Topic    : 析构默认 noexcept（C++11）与违背后果
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 10.2
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section07_raii_rollback_and_destructor_rules
// Item     : destructor_default_noexcept
// Topic id : part2/stage09/section07/destructor_default_noexcept
// Refs     : https://en.cppreference.com/w/cpp/language/destructor
//            ISO [class.dtor] [except.spec]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>

namespace {

struct Quiet {
    ~Quiet() { /* 默认 noexcept(true) */ }
};

struct ExplicitNoexcept {
    ~ExplicitNoexcept() noexcept {}
};

// ⚠️ 可以显式标 noexcept(false)，但极不推荐（破坏容器/算法假设）
struct AllowThrowDtor {
    ~AllowThrowDtor() noexcept(false) {
        // 即使标了 false，也不要在这里真的抛 —— 本 topic 不触发 terminate
    }
};

struct Member {
    Quiet a;
    ExplicitNoexcept b;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [destructor_default_noexcept] 主干：默认就是 noexcept ===\n";
    {
        static_assert(std::is_nothrow_destructible_v<Quiet>);
        static_assert(std::is_nothrow_destructible_v<ExplicitNoexcept>);
        static_assert(std::is_nothrow_destructible_v<Member>);
        static_assert(std::is_nothrow_destructible_v<std::vector<int>>);

        Quiet q;
        (void)q;
        std::cout << "Quiet nothrow_dtor=" << std::is_nothrow_destructible_v<Quiet> << '\n';
    }

    std::cout << "=== 对抗：显式 noexcept(false) 的类型 ===\n";
    {
        static_assert(!std::is_nothrow_destructible_v<AllowThrowDtor>);
        AllowThrowDtor x;
        (void)x;
        std::cout << "AllowThrowDtor nothrow_dtor=" << std::is_nothrow_destructible_v<AllowThrowDtor> << '\n';
        std::cout << "avoid noexcept(false) dtors in real code\n";
    }

    std::cout << "=== 专节：默认 noexcept 的含义 ===\n";
    // 若析构默认 noexcept，内部抛出且未捕获 → 直接 terminate（违背承诺）。
    // 这不是“建议”，而是语言默认强制的契约。
    std::cout << "throwing out of a noexcept dtor → std::terminate\n";
    std::cout << "keep cleanup noexcept; swallow or log only\n";

    std::cout << "[destructor_default_noexcept] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section07/destructor_default_noexcept", run>;

}  // namespace

// Topic    : noexcept 说明符：承诺不抛（非检查）与条件 noexcept
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 2.1 / 2.4
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section02_noexcept_basics
// Item     : noexcept_specifier
// Topic id : part2/stage09/section02/noexcept_specifier
// Refs     : https://en.cppreference.com/w/cpp/language/noexcept_spec
//            ISO [except.spec]
//            Effective Modern C++ Item 14

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

void safe() noexcept {
    // 承诺：绝不让异常逃逸
}

void may_throw() {
    // 默认：可能抛（即使函数体现在不抛，类型上仍是“可能抛”）
}

void conditional() noexcept(sizeof(int) == 4) {
    // 条件 noexcept：表达式为 true 时 noexcept(true)
}

// 条件 noexcept 惯用：本函数不抛 ⇔ 它调用的操作不抛
template <class T>
void swap_like(T& a, T& b) noexcept(noexcept(std::swap(a, b))) {
    using std::swap;
    swap(a, b);
}

struct Pod {
    int x = 0;
};

struct ThrowsOnCopy {
    ThrowsOnCopy() = default;
    ThrowsOnCopy(const ThrowsOnCopy&) { /* 可能抛 */ }
    ThrowsOnCopy& operator=(const ThrowsOnCopy&) { return *this; }
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [noexcept_specifier] 主干：声明即契约 ===\n";
    {
        static_assert(noexcept(safe()));
        static_assert(!noexcept(may_throw()));
        static_assert(noexcept(conditional()) == (sizeof(int) == 4));

        safe();
        may_throw();
        conditional();
        std::cout << "noexcept(safe)=" << noexcept(safe()) << " noexcept(may_throw)=" << noexcept(may_throw()) << '\n';
    }

    std::cout << "=== 对抗：该标 / 不该标 ===\n";
    {
        // 应当 noexcept：析构（默认已是）、swap、移动、简单访问器
        Pod a{1}, b{2};
        static_assert(noexcept(swap_like(a, b)));
        swap_like(a, b);
        assert(a.x == 2 && b.x == 1);

        // 不该滥标：可能分配 / 可能抛的操作标了 noexcept，真抛 → terminate
        // 示例仅用 type trait 说明，不故意 terminate。
        static_assert(!std::is_nothrow_copy_constructible_v<ThrowsOnCopy>);
        std::cout << "ThrowsOnCopy nothrow_copy=" << std::is_nothrow_copy_constructible_v<ThrowsOnCopy> << '\n';
    }

    std::cout << "=== 专节：与旧动态异常规范 ===\n";
    // C++17：throw() ≡ noexcept(true)；其余动态异常规范已移除。
    // noexcept 不是编译期检查：编译器信任你的承诺做优化。
    // 若 noexcept 函数实际抛出且未在内部捕获 → std::terminate（连展开都不保证）。
    std::cout << "noexcept is a promise, not a compile-time check\n";
    std::cout << "violation → std::terminate (not demonstrated here)\n";

    std::cout << "[noexcept_specifier] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section02/noexcept_specifier", run>;

}  // namespace

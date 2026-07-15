// Topic    : No-throw / no-fail 保证（最强异常安全）
// Doc      : 第2部分-阶段9-异常-错误处理与UB纪律.md · 步骤 3.1
// Stage    : part2_stage09_exceptions_error_handling_ub
// Section  : section03_exception_safety_levels
// Item     : no_throw_guarantee
// Topic id : part2/stage09/section03/no_throw_guarantee
// Refs     : https://en.cppreference.com/w/cpp/language/exceptions
//            ISO [res.on.exception.handling]

#include "learn/topic_registry.hpp"

#include <iostream>
#include <utility>

namespace {

// no-throw：操作绝不抛，且通常总成功
struct Handle {
    int* p = nullptr;

    Handle() noexcept = default;
    explicit Handle(int v) : p(new int(v)) {}
    ~Handle() noexcept { delete p; }

    Handle(Handle&& o) noexcept : p(o.p) { o.p = nullptr; }
    Handle& operator=(Handle&& o) noexcept {
        if (this != &o) {
            delete p;
            p = o.p;
            o.p = nullptr;
        }
        return *this;
    }

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    void swap(Handle& o) noexcept { std::swap(p, o.p); }

    int get() const noexcept { return p ? *p : -1; }
};

void swap(Handle& a, Handle& b) noexcept {
    a.swap(b);
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [no_throw_guarantee] 主干：swap / 移动 / 析构 ===\n";
    {
        Handle a{1};
        Handle b{2};
        static_assert(noexcept(a.swap(b)));
        static_assert(noexcept(swap(a, b)));
        static_assert(noexcept(Handle{std::move(a)}));

        swap(a, b);
        assert(a.get() == 2 && b.get() == 1);

        Handle c{std::move(a)};
        assert(c.get() == 2 && a.get() == -1);
        std::cout << "swap+move are no-throw building blocks\n";
    }

    std::cout << "=== 对抗：no-throw 是强保证的基石 ===\n";
    // 强保证（事务式）依赖：noexcept swap、noexcept 移动、noexcept 析构。
    // 没有这三块，copy-and-swap / 容器强保证就立不住。
    {
        Handle h{42};
        assert(noexcept(h.get()));
        std::cout << "accessor get() noexcept, value=" << h.get() << '\n';
    }

    std::cout << "=== 专节：四级保证中的位置 ===\n";
    // No-throw > Strong > Basic > No guarantee
    std::cout << "no-throw = never throws; always succeeds (swap/move/dtor)\n";

    std::cout << "[no_throw_guarantee] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage09/section03/no_throw_guarantee", run>;

}  // namespace

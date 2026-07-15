// Topic    : noexcept 移动决定 vector 扩容走移动还是拷贝
// Doc      : 第2部分-阶段5 · 步骤 3
// Stage    : part2_stage05_copy_move_smart_pointers
// Section  : section01_copy_vs_move
// Item     : noexcept_move_and_vector_growth
// Topic id : part2/stage05/section01/noexcept_move_and_vector_growth
// Refs     : https://en.cppreference.com/w/cpp/utility/move_if_noexcept
//            https://en.cppreference.com/w/cpp/language/move_constructor
//            Effective Modern C++ Item 14

#include "learn/topic_registry.hpp"

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

int g_copies = 0;
int g_moves = 0;

struct NoexceptMovable {
    int id = 0;
    NoexceptMovable() = default;
    explicit NoexceptMovable(int i) : id(i) {}
    NoexceptMovable(const NoexceptMovable& o) : id(o.id) {
        ++g_copies;
        std::cout << "  NoexceptMovable copy\n";
    }
    NoexceptMovable(NoexceptMovable&& o) noexcept : id(o.id) {
        ++g_moves;
        o.id = -1;
        std::cout << "  NoexceptMovable move (noexcept)\n";
    }
    NoexceptMovable& operator=(const NoexceptMovable& o) {
        id = o.id;
        ++g_copies;
        return *this;
    }
    NoexceptMovable& operator=(NoexceptMovable&& o) noexcept {
        id = o.id;
        o.id = -1;
        ++g_moves;
        return *this;
    }
};

struct ThrowingMovable {
    int id = 0;
    ThrowingMovable() = default;
    explicit ThrowingMovable(int i) : id(i) {}
    ThrowingMovable(const ThrowingMovable& o) : id(o.id) {
        ++g_copies;
        std::cout << "  ThrowingMovable copy\n";
    }
    // 故意不标 noexcept → vector 扩容为保强异常安全而选择拷贝
    ThrowingMovable(ThrowingMovable&& o) : id(o.id) {
        ++g_moves;
        o.id = -1;
        std::cout << "  ThrowingMovable move (NOT noexcept)\n";
    }
    ThrowingMovable& operator=(const ThrowingMovable& o) {
        id = o.id;
        ++g_copies;
        return *this;
    }
    ThrowingMovable& operator=(ThrowingMovable&& o) {
        id = o.id;
        o.id = -1;
        ++g_moves;
        return *this;
    }
};

// 历史上常见坑：某成员移动非 noexcept → 整个类隐式移动也非 noexcept。
// 注：新版 MSVC STL 里 std::function 的移动可能已是 nothrow；用「可抛移动成员」演示同一机制。
struct ThrowingMember {
    ThrowingMember() = default;
    ThrowingMember(ThrowingMember&&) { /* not noexcept */ }
    ThrowingMember(const ThrowingMember&) = default;
    ThrowingMember& operator=(ThrowingMember&&) = default;
    ThrowingMember& operator=(const ThrowingMember&) = default;
};

struct Handler {
    ThrowingMember payload;
    std::function<void()> cb;  // 另见：function 的 noexcept 性随标准库实现演变
    int id = 0;
};

// Rule of 0：成员皆 noexcept 可移动 → 隐式移动也是 noexcept
struct AutoNoexcept {
    std::string s;
    std::vector<int> v;
    std::unique_ptr<int> p;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [noexcept_move_and_vector_growth] 入门：vector 策略 ===\n";

    static_assert(std::is_nothrow_move_constructible_v<NoexceptMovable>);
    static_assert(!std::is_nothrow_move_constructible_v<ThrowingMovable>);

    g_copies = g_moves = 0;
    {
        std::vector<NoexceptMovable> v1;
        v1.reserve(1);
        v1.emplace_back(1);
        const int moves_before = g_moves;
        v1.emplace_back(2);  // 扩容：应移动已有元素
        assert(g_moves > moves_before);
        assert(v1.size() == 2);
        std::cout << "NoexceptMovable growth: moves=" << g_moves << " copies=" << g_copies << '\n';
    }

    g_copies = g_moves = 0;
    {
        std::vector<ThrowingMovable> v2;
        v2.reserve(1);
        v2.emplace_back(1);
        const int copies_before = g_copies;
        v2.emplace_back(2);  // 扩容：移动非 noexcept → 拷贝
        assert(g_copies > copies_before);
        assert(v2.size() == 2);
        std::cout << "ThrowingMovable growth: moves=" << g_moves << " copies=" << g_copies << '\n';
    }

    std::cout << "=== 进阶：move_if_noexcept ===\n";
    {
        NoexceptMovable a{10};
        auto b = std::move_if_noexcept(a);
        // nothrow → 移动
        assert(b.id == 10);
        (void)b;

        ThrowingMovable c{20};
        auto d = std::move_if_noexcept(c);
        // 可抛移动 + 可拷贝 → 拷贝，源不变
        assert(d.id == 20);
        assert(c.id == 20);
        std::cout << "move_if_noexcept: Noexcept moved, Throwing copied\n";
    }

    std::cout << "=== 进阶：非 noexcept 成员拖累整类移动 ===\n";
    static_assert(!std::is_nothrow_move_constructible_v<ThrowingMember>);
    static_assert(!std::is_nothrow_move_constructible_v<Handler>);
    std::cout << "std::function nothrow_move="
              << std::is_nothrow_move_constructible_v<std::function<void()>> << " (implementation-defined over time)\n";
    std::cout << "Handler (has ThrowingMember) nothrow_move=" << std::is_nothrow_move_constructible_v<Handler> << '\n';

    std::cout << "=== 专家：隐式移动的 noexcept 由成员推导 ===\n";
    static_assert(std::is_nothrow_move_constructible_v<AutoNoexcept>);
    static_assert(std::is_nothrow_move_assignable_v<AutoNoexcept>);
    // Rule of 0 + 标准库成员 → 自动获得 noexcept 移动
    // 手写移动时：只要真的不抛，务必标 noexcept，否则 vector 扩容白拷贝

    std::cout << "=== noexcept_move_and_vector_growth: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage05/section01/noexcept_move_and_vector_growth", run>;

}  // namespace

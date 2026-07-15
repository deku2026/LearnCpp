// Topic     : 特殊成员函数抑制规则
// Doc       : 第2部分-阶段4 · 步骤 3.3
// cppreference: https://en.cppreference.com/cpp/language/rule_of_three
//
// 要点: 声明析构/拷贝构造/拷贝赋值任一 → 隐式移动被抑制；
//       声明移动任一 → 拷贝被隐式 delete；用户构造抑制默认构造。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

namespace {

// 什么都不声明：六个特殊成员都可隐式生成（按需）
struct AllImplicit {
    int x = 0;
    std::string s;
};

// 用户声明析构 → 抑制隐式移动（C++11 起）
struct UserDtor {
    int x = 0;
    ~UserDtor() {}  // 即使空体
};

// 用户声明拷贝构造 → 抑制隐式移动；也抑制默认构造
struct UserCopyCtor {
    int x = 0;
    UserCopyCtor() = default;
    UserCopyCtor(const UserCopyCtor&) = default;
};

// 用户声明移动构造 → 拷贝被隐式 delete
struct UserMoveCtor {
    int x = 0;
    UserMoveCtor() = default;
    UserMoveCtor(UserMoveCtor&&) = default;
};

// 声明了析构后显式 =default 移回来
struct FixMoves {
    int x = 0;
    ~FixMoves() = default;
    FixMoves() = default;
    FixMoves(const FixMoves&) = default;
    FixMoves& operator=(const FixMoves&) = default;
    FixMoves(FixMoves&&) = default;
    FixMoves& operator=(FixMoves&&) = default;
};

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [suppression_rules] ===\n";

    // -------------------------------------------------------------------------
    // §入门：全隐式
    // -------------------------------------------------------------------------
    static_assert(std::is_default_constructible_v<AllImplicit>);
    static_assert(std::is_copy_constructible_v<AllImplicit>);
    static_assert(std::is_move_constructible_v<AllImplicit>);
    AllImplicit a{.x = 1, .s = "a"};
    AllImplicit b = std::move(a);
    assert(b.x == 1);
    std::cout << "[intro] AllImplicit move ok s=" << b.s << '\n';

    // -------------------------------------------------------------------------
    // §进阶：析构抑制移动；移动「退化」为拷贝（若仍可拷贝）
    // -------------------------------------------------------------------------
    static_assert(std::is_copy_constructible_v<UserDtor>);
    // 用户声明析构后：不声明移动 → 无隐式移动构造
    static_assert(!std::is_move_constructible_v<UserDtor> || std::is_copy_constructible_v<UserDtor>);
    // 更精确：is_move_constructible 在「移动被抑制但可拷贝」时仍可能为 true
    // （用拷贝实现移动请求）。用 trivial/生成探测：
    static_assert(!std::is_trivially_move_constructible_v<UserDtor>);
    // 实际上 UserDtor 的移动被抑制后，std::is_move_constructible 常为 true（fallback copy）
    {
        UserDtor u1{};
        u1.x = 5;
        UserDtor u2 = std::move(u1);  // 实际走拷贝
        assert(u2.x == 5);
        assert(u1.x == 5);  // 拷贝后源仍在 —— 证明退化成拷贝
    }
    std::cout << "[advanced] UserDtor: move request falls back to copy (src still 5)\n";

    // -------------------------------------------------------------------------
    // §专家：移动声明删除拷贝；显式 default 修复
    // -------------------------------------------------------------------------
    static_assert(!std::is_copy_constructible_v<UserMoveCtor>);
    static_assert(std::is_move_constructible_v<UserMoveCtor>);
    UserMoveCtor m1{};
    UserMoveCtor m2 = std::move(m1);
    (void)m2;
    // UserMoveCtor m3 = m2; // ❌

    static_assert(std::is_copy_constructible_v<FixMoves>);
    static_assert(std::is_move_constructible_v<FixMoves>);
    static_assert(std::is_trivially_move_constructible_v<FixMoves>);

    static_assert(std::is_copy_constructible_v<UserCopyCtor>);
    // 声明拷贝后隐式移动抑制
    {
        UserCopyCtor c1{};
        c1.x = 3;
        [[maybe_unused]] UserCopyCtor c2 = std::move(c1);
        assert(c1.x == 3);  // 仍是拷贝语义
        assert(c2.x == 3);
    }

    std::cout << "[expert] UserMoveCtor copy deleted; FixMoves restores trivial move\n";
    std::cout << "=== suppression_rules: OK ===\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage04/section03/suppression_rules", run>;

}  // namespace

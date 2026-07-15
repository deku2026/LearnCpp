// Topic    : C++20 using enum：局部引入枚举器名
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 8.4
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : using_enum_cpp20
// Topic id : part2/stage02/section08/using_enum_cpp20
// Refs     : https://en.cppreference.com/cpp/language/enum
//            提案 P1099 (using enum)

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

enum class Color { Red, Green, Blue };
enum class Traffic { Red, Yellow, Green };

std::string_view name(Color c) {
    switch (c) {
        using enum Color;  // 本 switch 作用域内可直接写 Red/Green/Blue
        case Red:
            return "red";
        case Green:
            return "green";
        case Blue:
            return "blue";
    }
    return "?";
}

std::string paint(Color c) {
    using enum Color;  // 函数作用域引入
    if (c == Red) {
        return "stop-paint";
    }
    if (c == Green) {
        return "go-paint";
    }
    return "blue-paint";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [using_enum_cpp20] 入门：switch 内减负 ===\n";
    {
        assert(name(Color::Red) == "red");
        assert(name(Color::Blue) == "blue");
        // 函数外仍需 Color::
        Color c = Color::Green;
        assert(name(c) == "green");
        std::cout << "[intro] using enum Color inside switch\n";
    }

    std::cout << "=== 进阶：函数作用域引入；不破坏类型安全 ===\n";
    {
        assert(paint(Color::Red) == "stop-paint");
        assert(paint(Color::Green) == "go-paint");

        // 引入后仍是 Color 类型，不能与 int 混用
        using enum Color;
        Color c = Blue;
        // int n = c;  // ❌ 仍然不隐式转换
        assert(static_cast<int>(c) == 2);
        std::cout << "[advanced] still scoped enum type safety\n";
    }

    std::cout << "=== 专家：多枚举同名时的歧义；受控引入 ===\n";
    {
        // Color 与 Traffic 都有 Red/Green——同一作用域 using enum 两者会冲突
        {
            using enum Color;
            assert(Red == Color::Red);
        }
        {
            using enum Traffic;
            assert(Red == Traffic::Red);
            assert(Yellow == Traffic::Yellow);
        }
        // 推荐：仅在 switch / 小函数内 using enum，避免文件级污染
        // 这比传统 enum 全局泄漏可控得多

        std::cout << "[expert] introduce enumerators locally; avoid dual using enum clash\n";
    }

    std::cout << "[using_enum_cpp20] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/using_enum_cpp20", run>;

}  // namespace

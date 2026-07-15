// Topic    : enum class：强类型、带作用域、不隐式转换
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 8.2
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : enum_class_strong_typed
// Topic id : part2/stage02/section08/enum_class_strong_typed
// Refs     : https://en.cppreference.com/cpp/language/enum
//            ISO [dcl.enum]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace {

enum class Color { Red, Green, Blue };
enum class Fruit { Apple, Banana, Red };  // ✅ 与 Color::Red 互不冲突

// enum struct 与 enum class 等价
enum struct Direction { North, South, East, West };

[[maybe_unused]] std::string_view color_name(Color c) {
    switch (c) {
        case Color::Red:
            return "red";
        case Color::Green:
            return "green";
        case Color::Blue:
            return "blue";
    }
    return "?";
}

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [enum_class_strong_typed] 入门：必须带作用域 ===\n";
    {
        [[maybe_unused]] Color c = Color::Red;
        // Color c2 = Red;  // ❌ 不泄漏
        assert(c == Color::Red);
        assert(color_name(c) == "red");

        [[maybe_unused]] Fruit f = Fruit::Red;  // 与 Color::Red 共存
        assert(static_cast<int>(f) == 2);

        [[maybe_unused]] Direction d = Direction::North;
        assert(d != Direction::South);
        std::cout << "[intro] Color::Red vs Fruit::Red OK; no leak\n";
    }

    std::cout << "=== 进阶：无隐式 int；只能同类型比较 ===\n";
    {
        Color c = Color::Green;
        // int n = c;           // ❌
        // if (c == 0) {}       // ❌
        // if (c == Fruit::Red) // ❌ 不同类型
        [[maybe_unused]] int n = static_cast<int>(c);
        assert(n == 1);
        assert(c == Color::Green);
        assert(c != Color::Blue);

        // 算术也不行，除非显式底层转换
        // auto x = c + 1;  // ❌
        [[maybe_unused]] auto x = static_cast<int>(c) + 1;
        assert(x == 2);

        static_assert(std::is_enum_v<Color>);
        static_assert(std::is_scoped_enum_v<Color>);
        std::cout << "[advanced] no implicit int; explicit cast when needed\n";
    }

    std::cout << "=== 专家：相对传统 enum 的类型安全收益（验收改造）===\n";
    {
        // 改造对照：
        //   enum Color { Red, Green, Blue };  →  enum class Color { ... };
        //   Color c = Red;                    →  Color c = Color::Red;
        //   int n = c;                        →  int n = static_cast<int>(c);
        //                                       或 C++23 std::to_underlying(c)
        // 收益：命名不污染、不能和浮点/任意 int 乱比、重载不会误选

        Color traffic = Color::Red;
        if (traffic == Color::Red) {
            traffic = Color::Green;
        }
        assert(traffic == Color::Green);
        assert(color_name(traffic) == "green");

        // 默认底层类型是 int（scoped enum）
        static_assert(std::is_same_v<std::underlying_type_t<Color>, int>);

        std::cout << "[expert] migrate bare enum → enum class for safety\n";
    }

    std::cout << "[enum_class_strong_typed] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/enum_class_strong_typed", run>;

}  // namespace

// Topic    : 传统 unscoped enum 的三宗罪（对照题）
// Doc      : 第2部分-阶段2-类型系统-值类别与类型推导.md · 步骤 8.1
// Stage    : part2_stage02_type_system_value_category_deduction
// Section  : section08_enums
// Item     : legacy_enum_compare
// Topic id : part2/stage02/section08/legacy_enum_compare
// Refs     : https://en.cppreference.com/cpp/language/enum
//            ISO [dcl.enum]

#include "learn/topic_registry.hpp"

#include <cstring>
#include <iostream>
#include <type_traits>
#include <version>

namespace {

// 传统 enum：枚举器泄漏到外层作用域
enum Color { Red, Green, Blue };
// enum Fruit { Apple, Banana, Red };  // ❌ Red 已在外层冲突（刻意不写，避免编译失败）
enum Fruit { Apple, Banana, FruitRed };

// 另一个泄漏例子：状态码与整数搅在一起
enum Status { Ok = 0, Warn = 1, Error = 2 };

int run(int argc, char** argv) {
    (void)argc;
    (void)argv;

    std::cout << "=== [legacy_enum_compare] 入门：泄漏作用域 + 隐式转 int ===\n";
    {
        Color c = Red;               // 无需 Color:: —— 泄漏
        [[maybe_unused]] int n = c;  // 隐式转 int，毫无阻拦
        assert(n == 0);
        assert(c == Red);

// 能和浮点比较——完全失去类型语义（合法但可怕）
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdeprecated-enum-float-conversion"
#endif
        const bool weird = (static_cast<int>(Red) < 3);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
        assert(weird);

        // unscoped enum 提升为整数后参与算术/比较
        assert(Ok == 0);
        assert(static_cast<int>(Warn) + 1 == Error);

        std::cout << "[intro] unscoped: leak + implicit int + weak typing\n";
    }

    std::cout << "=== 进阶：命名冲突与「假类型安全」===\n";
    {
        // 若两个 enum 都叫 Red，无法共存——迫使 FruitRed 这种前缀污染
        [[maybe_unused]] Fruit f = Apple;
        assert(static_cast<int>(f) == 0);

        // switch 里写裸枚举器，看起来像「全局常量」
        Color sample = Blue;
        [[maybe_unused]] const char* label = "?";
        switch (sample) {
            case Red:
                label = "R";
                break;
            case Green:
                label = "G";
                break;
            case Blue:
                label = "B";
                break;
        }
        assert(std::strcmp(label, "B") == 0);

        // 底层类型未指定时由实现选择 → 难可靠前向声明（见 enum_underlying_type）
        std::cout << "[advanced] name clashes force ugly prefixes; ABI size fuzzy\n";
    }

    std::cout << "=== 专家：为何新代码弃用；迁移到 enum class 的收益预告 ===\n";
    {
        // 三宗罪小结：
        // 1) 枚举器泄漏 → 命名空间污染 / 冲突
        // 2) 隐式转整数 → 逻辑错误、错误重载、与浮点比较
        // 3) 底层类型不确定 → 前向声明与 ABI 布局不稳
        // 对策：enum class（下一题）+ 显式底层类型 + to_underlying

        Color c = Green;
        // 模拟「误用」：把颜色当循环上界
        [[maybe_unused]] int sum = 0;
        for (int i = 0; i < Blue; ++i) {  // Blue 当 2 用——语义崩坏却能编译
            sum += i;
        }
        assert(sum == 1);  // 0+1

        static_assert(std::is_enum_v<Color>);
#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum
        static_assert(!std::is_scoped_enum_v<Color>);  // C++23：确认 unscoped
#endif

        std::cout << "[expert] rewrite with enum class for scope + no implicit int\n";
        (void)c;
    }

    std::cout << "[legacy_enum_compare] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage02/section08/legacy_enum_compare", run>;

}  // namespace

// Topic     : std::to_underlying —— 枚举 → 底层整型 (C++23)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 5.5
// Stage     : part2_stage10_stl_deep_dive
// Section   : section05_algorithms
// Item      : std_to_underlying_cpp23
// Topic id  : part2/stage10/section05/std_to_underlying_cpp23
// Refs      : https://en.cppreference.com/w/cpp/utility/to_underlying
//             ISO [utility.underlying]

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>
#include <version>

namespace {

enum class Color : int { Red = 1, Green = 2, Blue = 4 };
enum class Flag : std::uint8_t { None = 0, Read = 1, Write = 2, Exec = 4 };
enum Legacy { LegacyA = 10, LegacyB = 20 };  // 无作用域枚举

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_to_underlying_cpp23] 入门：enum class → 底层整型 ===\n";
    {
        auto g = std::to_underlying(Color::Green);
        static_assert(std::is_same_v<decltype(g), int>);
        assert(g == 2);

        auto f = std::to_underlying(Flag::Write);
        static_assert(std::is_same_v<decltype(f), std::uint8_t>);
        assert(f == 2);

        assert(std::to_underlying(Color::Blue) == 4);
        std::cout << "Color::Green=" << g << " Flag::Write=" << static_cast<int>(f) << '\n';
    }

    std::cout << "=== 进阶：位标志组合 + 无作用域枚举 ===\n";
    {
        [[maybe_unused]] const auto rw =
            static_cast<std::uint8_t>(std::to_underlying(Flag::Read) | std::to_underlying(Flag::Write));
        assert(rw == 3);
        assert((rw & std::to_underlying(Flag::Read)) != 0);

        // 无作用域枚举同样可用（底层默认 int）
        assert(std::to_underlying(LegacyB) == 20);
        static_assert(std::is_same_v<decltype(std::to_underlying(LegacyA)), int>);

        // 替代 static_cast 的教学对照（意图更清晰）
        assert(std::to_underlying(Color::Blue) == static_cast<int>(Color::Blue));
        std::cout << "bit-flags + unscoped enum OK\n";
    }

    std::cout << "=== 专家：类型安全日志 / 与 magic_enum 对比点 ===\n";
    {
        // 序列化/日志：需要底层值但不想写 static_cast 噪声
        [[maybe_unused]] auto dump = [](Color c) { return std::to_underlying(c); };
        assert(dump(Color::Red) == 1);

        // ⚠️ to_underlying 不做范围检查；非法底层值照样转换
        [[maybe_unused]] const Color forged = static_cast<Color>(99);
        assert(std::to_underlying(forged) == 99);

        // 返回类型是 underlying_type_t，便于模板元编程
        using U = std::underlying_type_t<Flag>;
        static_assert(std::is_same_v<U, std::uint8_t>);
        [[maybe_unused]] U bits = std::to_underlying(Flag::Exec);
        assert(bits == 4);

#if defined(__cpp_lib_to_underlying)
        std::cout << "__cpp_lib_to_underlying=" << __cpp_lib_to_underlying << '\n';
#endif
        std::cout << "logging + forged-enum + underlying_type_t OK\n";
    }

    std::cout << "[std_to_underlying_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_to_underlying_cpp23", run>;

}  // namespace

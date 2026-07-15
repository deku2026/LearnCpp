// Topic     : std::to_underlying —— 枚举 → 底层整型 (C++23)
// Doc       : 第2部分-阶段10 · 步骤 5.5
// cppreference: https://en.cppreference.com/cpp/utility/to_underlying
//
// 要点: <utility>; 比 static_cast 更清晰; 返回 underlying_type_t。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>

namespace {

enum class Color : int { Red = 1, Green = 2, Blue = 4 };
enum class Flag : std::uint8_t { None = 0, Read = 1, Write = 2, Exec = 4 };
enum Legacy { LegacyA = 10, LegacyB = 20 };  // 无作用域枚举

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [std_to_underlying_cpp23] ===\n";

    // 用非 const auto，避免 decltype 带上顶层 const
    auto g = std::to_underlying(Color::Green);
    static_assert(std::is_same_v<decltype(g), int>);
    assert(g == 2);

    auto f = std::to_underlying(Flag::Write);
    static_assert(std::is_same_v<decltype(f), std::uint8_t>);
    assert(f == 2);

    // 位组合后还原检查
    const auto rw = static_cast<std::uint8_t>(std::to_underlying(Flag::Read) | std::to_underlying(Flag::Write));
    assert(rw == 3);

    // 无作用域枚举也可用
    assert(std::to_underlying(LegacyB) == 20);

    // 替代 static_cast 的教学对照
    assert(std::to_underlying(Color::Blue) == static_cast<int>(Color::Blue));

    std::cout << "[to_underlying] Color::Green=" << g << " Flag::Write=" << static_cast<int>(f) << '\n';
    std::cout << "std_to_underlying_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section05/std_to_underlying_cpp23", run>;

}  // namespace

// Topic     : string_view::contains (C++23, P1679)
// Doc       : 第2部分-阶段10 · 步骤 6.3
// cppreference: https://en.cppreference.com/cpp/string/basic_string_view/contains
//
// 要点: 子串/字符 contains 返回 bool; 等价 find != npos, 更可读。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string_view>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_contains_cpp23] ===\n";

    constexpr std::string_view sv = "Hello, C++23 world";

    assert(sv.contains("C++23"));
    assert(sv.contains('w'));
    assert(!sv.contains("Java"));
    assert(!sv.contains('z'));

    // 空串: 任何串都 contains ""
    assert(sv.contains(""));
    assert(std::string_view{}.contains(""));
    assert(!std::string_view{}.contains("x"));

    // 与 find 等价
    assert(sv.contains("world") == (sv.find("world") != std::string_view::npos));

    std::cout << "[contains] \"C++23\" in sv? " << std::boolalpha << sv.contains("C++23") << '\n';
    std::cout << "string_view_contains_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_contains_cpp23", run>;

}  // namespace

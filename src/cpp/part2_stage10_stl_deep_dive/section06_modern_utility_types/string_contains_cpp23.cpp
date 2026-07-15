// Topic     : std::string::contains (C++23)
// Doc       : 第2部分-阶段10 · 步骤 7.1
// cppreference: https://en.cppreference.com/cpp/string/basic_string/contains
//
// 要点: 与 string_view::contains 对称; 子串/字符/string_view 重载。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_contains_cpp23] ===\n";

    std::string s = "std::string now has contains";
    assert(s.contains("contains"));
    assert(s.contains(':'));
    assert(s.contains(std::string_view{"string"}));
    assert(!s.contains("vector"));

    // 大小写敏感
    assert(!s.contains("Contains"));

    std::cout << "[string::contains] OK for substr/char/string_view\n";
    std::cout << "string_contains_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_contains_cpp23", run>;

}  // namespace

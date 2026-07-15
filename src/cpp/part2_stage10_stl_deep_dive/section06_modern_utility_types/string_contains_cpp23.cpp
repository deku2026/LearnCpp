// Topic     : std::string::contains (C++23, P1679)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 7.1
// Stage     : part2_stage10_stl_deep_dive
// Section   : section06_modern_utility_types
// Item      : string_contains_cpp23
// Topic id  : part2/stage10/section06/string_contains_cpp23
// Refs      : https://en.cppreference.com/w/cpp/string/basic_string/contains
//             P1679R3

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_contains_cpp23] 入门：子串 / 字符 / string_view 重载 ===\n";
    {
        std::string s = "std::string now has contains";
        assert(s.contains("contains"));
        assert(s.contains(':'));
        assert(s.contains(std::string_view{"string"}));
        assert(!s.contains("vector"));
        // 大小写敏感（无 locale 折叠）
        assert(!s.contains("Contains"));
        std::cout << "substr/char/string_view overloads OK\n";
    }

    std::cout << "=== 进阶：与 find != npos 等价 + 空串边界 ===\n";
    {
        const std::string s = "abc";
        assert(s.contains("bc") == (s.find("bc") != std::string::npos));
        assert(s.contains('z') == (s.find('z') != std::string::npos));
        // 空 needle：任何串（含空串）都 contains ""
        assert(s.contains(""));
        assert(std::string{}.contains(""));
        assert(!std::string{}.contains("x"));
        // 自身完整匹配
        assert(s.contains(s));
        assert(s.contains(std::string_view{s}));
        std::cout << "find-equivalence + empty-needle edges OK\n";
    }

    std::cout << "=== 专家：UTF-8 按字节、与 starts_with/ends_with 对照 ===\n";
    {
        // contains 是按 char/字节序列；多字节 UTF-8 不做字素聚类
        const std::string u8 = "C++23 标准";  // 可能含多字节序列
        assert(u8.contains("C++"));
        assert(u8.contains("23"));

        const std::string path = "/usr/local/bin/clang++";
        assert(path.contains("local"));
        assert(path.starts_with("/usr"));  // C++20
        assert(path.ends_with("clang++"));
        // contains 不承诺位置；需要前缀/后缀用 starts/ends_with
        assert(path.contains("bin") && !path.starts_with("bin"));

#if defined(__cpp_lib_string_contains)
        std::cout << "__cpp_lib_string_contains=" << __cpp_lib_string_contains << '\n';
#endif
        std::cout << "byte-wise contains vs starts/ends_with OK\n";
    }

    std::cout << "[string_contains_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_contains_cpp23", run>;

}  // namespace

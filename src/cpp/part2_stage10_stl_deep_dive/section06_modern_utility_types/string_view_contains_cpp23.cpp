// Topic     : string_view::contains (C++23, P1679)
// Doc       : 第2部分-阶段10-STL深潜.md · 步骤 6.3
// Stage     : part2_stage10_stl_deep_dive
// Section   : section06_modern_utility_types
// Item      : string_view_contains_cpp23
// Topic id  : part2/stage10/section06/string_view_contains_cpp23
// Refs      : https://en.cppreference.com/w/cpp/string/basic_string_view/contains
//             P1679R3

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <version>

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== [string_view_contains_cpp23] 入门：子串 / 字符 contains ===\n";
    {
        [[maybe_unused]] constexpr std::string_view sv = "Hello, C++23 world";
        assert(sv.contains("C++23"));
        assert(sv.contains('w'));
        assert(!sv.contains("Java"));
        assert(!sv.contains('z'));
        static_assert(std::string_view{"abc"}.contains('b'));
        std::cout << "literal view contains OK\n";
    }

    std::cout << "=== 进阶：空串边界 + 与 find 等价 ===\n";
    {
        constexpr std::string_view sv = "Hello, C++23 world";
        assert(sv.contains(""));
        assert(std::string_view{}.contains(""));
        assert(!std::string_view{}.contains("x"));
        assert(sv.contains("world") == (sv.find("world") != std::string_view::npos));
        // 切片后再 contains（零拷贝）
        [[maybe_unused]] const auto mid = sv.substr(7, 5);  // "C++23"
        assert(mid.contains("++") && mid.contains('3'));
        assert(!mid.contains("Hello"));
        std::cout << "empty + find-equiv + substr slice OK\n";
    }

    std::cout << "=== 专家：不拥有语义 + 与 string::contains 对称 ===\n";
    {
        std::string owned = "needle in a haystack";
        std::string_view sv = owned;
        assert(sv.contains("needle") && owned.contains("needle"));
        assert(sv.contains(std::string_view{"hay"}));
        // ⚠️ view 指向 owned；owned 析构后 contains 结果不可用（勿存长命 view）
        // 教学：先拷贝 needle 再改 owned，view 仍指向原 buffer
        [[maybe_unused]] const bool before = sv.contains("needle");
        owned[0] = 'N';  // 修改底层 → view 立刻看到
        assert(before);
        assert(sv.starts_with('N'));
        assert(!sv.contains("needle"));  // 首字母已变

#if defined(__cpp_lib_string_contains)
        std::cout << "__cpp_lib_string_contains=" << __cpp_lib_string_contains << '\n';
#endif
        std::cout << "non-owning symmetry with string::contains OK\n";
    }

    std::cout << "[string_view_contains_cpp23] all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part2/stage10/section06/string_view_contains_cpp23", run>;

}  // namespace

// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.2 string_view::contains (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : string_view_contains_cpp23
// Topic id : part3/section01/string_view_contains_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string_view/contains
//   Feature-test: __cpp_lib_string_contains >= 202011L
//
// 要点: C++23 子串/字符探测; 字节语义(非 Unicode 字形);
//       与 find!=npos 等价; 不拥有数据 — 仅在底层存活时调用。

#include "learn/topic_registry.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

// 可移植等价实现(feature 缺失时 / 对照 find 语义)
[[maybe_unused]] constexpr bool contains_fallback(std::string_view hay, std::string_view needle) {
    return hay.find(needle) != std::string_view::npos;
}
[[maybe_unused]] constexpr bool contains_char_fallback(std::string_view hay, char c) {
    return hay.find(c) != std::string_view::npos;
}

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/string_view_contains_cpp23 ===\n";

    constexpr std::string_view sv = "the quick brown fox";

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    std::cout << "[intro] __cpp_lib_string_contains=" << __cpp_lib_string_contains << '\n';

    // --- 入门: 子串 / 字符 / string_view 重载 ---
    assert(sv.contains("quick"));
    assert(sv.contains('q'));
    assert(sv.contains(std::string_view{"fox"}));
    assert(!sv.contains("dog"));
    assert(!sv.contains('z'));
    static_assert(std::string_view{"abc"}.contains('b'));
    static_assert(!std::string_view{"abc"}.contains("z"));

    // --- 进阶: 与 find 完全等价; 空串约定 ---
    assert(sv.contains("quick") == (sv.find("quick") != std::string_view::npos));
    assert(sv.contains(""));                  // 空 needle: 总为 true(与 find 一致)
    assert(std::string_view{}.contains(""));  // 空 hay + 空 needle
    assert(!std::string_view{}.contains("x"));

    // 大小写敏感 — 字节匹配, 非 locale
    assert(!sv.contains("Quick"));
    assert(sv.contains("quick"));

    // --- 专家: 底层销毁后 view 悬垂(不演示 UB); UTF-8 按字节 ---
    {
        std::string owner = "cafeé";  // é 多字节
        std::string_view v = owner;
        assert(v.contains("cafe"));
        // 可能不包含单字节 'é' 的错误假设: contains 看字节序列
        assert(v.size() >= 4);
        // 截断 owner 后勿再读 v — 文档 1.2 悬垂坑
    }
    // 前缀/后缀可用 starts_with/ends_with(C++20); contains 是任意位置
    assert(sv.starts_with("the"));
    assert(sv.ends_with("fox"));
    assert(sv.contains("brown") && !sv.starts_with("brown"));

    std::cout << "[advanced] empty-needle true; case-sensitive byte match\n";
    std::cout << "[expert] dangling: never contains() after owner destroyed\n";
#else
    assert(contains_fallback(sv, "quick"));
    assert(!contains_fallback(sv, "dog"));
    assert(contains_char_fallback(sv, 'q'));
    assert(contains_fallback(sv, ""));
    static_assert(contains_fallback("abc", "b"));
    std::cout << "[intro] contains unavailable; find fallback\n";
#endif

    std::cout << "string_view_contains_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/string_view_contains_cpp23", run>;

}  // namespace

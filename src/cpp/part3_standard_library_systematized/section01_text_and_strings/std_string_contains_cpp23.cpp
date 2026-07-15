// LearnCpp topic
// Doc      : 第3部分-标准库系统化.md · 库1 · 1.1 contains (C++23)
// Stage    : part3_standard_library_systematized
// Section  : section01_text_and_strings
// Item     : std_string_contains_cpp23
// Topic id : part3/section01/std_string_contains_cpp23
//
// Refs:
//   https://en.cppreference.com/w/cpp/string/basic_string/contains
//   Feature-test: __cpp_lib_string_contains >= 202011L
//
// 要点: string::contains 是 find!=npos 的可读封装; 字节语义;
//       与 string_view::contains 同 feature-test 宏。

#include "learn/topic_registry.hpp"

#include <iostream>
#include <string>
#include <string_view>

#if defined(__has_include)
#if __has_include(<version>)
#include <version>
#endif
#endif

namespace {

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_string_contains_cpp23 ===\n";

    const std::string s = "Hello, World";

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    std::cout << "[intro] __cpp_lib_string_contains=" << __cpp_lib_string_contains << '\n';

    // --- 入门: 子串 / 单字符 / string_view ---
    assert(s.contains("World"));
    assert(s.contains('H'));
    assert(s.contains(std::string_view{"ello"}));
    assert(!s.contains("WORLD"));  // 大小写敏感 · 字节匹配
    assert(!s.contains('z'));
    std::cout << "[intro] contains(\"World\")=" << std::boolalpha << s.contains("World") << '\n';

    // --- 进阶: 与 find 等价; 空串; 与 starts_with 分工 ---
    assert((s.find("World") != std::string::npos) == s.contains("World"));
    assert(s.contains(""));
    assert(s.starts_with("Hello") && s.contains("lo, W"));
    assert(s.ends_with("World") && !s.contains("world"));

    // 修改后重新探测(拥有型 string 安全)
    std::string mut = s;
    assert(mut.contains("Hello"));
    mut.replace(0, 5, "Hi");
    assert(!mut.contains("Hello"));
    assert(mut.contains("Hi"));

    // --- 专家: UTF-8 字节 vs “字符”; Qt QString::contains 心智不同 ---
    const std::string utf8 = "héllo";
    assert(utf8.contains("ll"));
    // size 是字节; contains 不会做 Unicode 规范化
    assert(utf8.size() >= 5);
    std::cout << "[expert] QString::contains is Unicode-aware; std::string is bytes\n";
#else
    auto contains = [](const std::string& str, std::string_view sub) { return str.find(sub) != std::string::npos; };
    assert(contains(s, "World"));
    assert(!contains(s, "WORLD"));
    assert(contains(s, ""));
    std::cout << "[intro] __cpp_lib_string_contains unavailable; used find\n";
#endif

    std::cout << "std_string_contains_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_contains_cpp23", run>;

}  // namespace

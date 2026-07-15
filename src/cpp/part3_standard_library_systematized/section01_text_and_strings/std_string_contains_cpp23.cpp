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

int run(int /*argc*/, char** /*argv*/) {
    std::cout << "=== part3/section01/std_string_contains_cpp23 ===\n";

    const std::string s = "Hello, World";

#if defined(__cpp_lib_string_contains) && __cpp_lib_string_contains >= 202011L
    // 子串 / 单字符 / string_view 重载
    assert(s.contains("World"));
    assert(s.contains('H'));
    assert(s.contains(std::string_view{"ello"}));
    assert(!s.contains("WORLD"));  // 大小写敏感、字节匹配
    assert(!s.contains('z'));

    std::cout << "[intro] contains(\"World\")=" << std::boolalpha << s.contains("World") << '\n';
    std::cout << "[intro] __cpp_lib_string_contains=" << __cpp_lib_string_contains << '\n';

    // 等价旧写法: find != npos
    assert((s.find("World") != std::string::npos) == s.contains("World"));
#else
    // 降级: find != npos
    auto contains = [](const std::string& str, std::string_view sub) { return str.find(sub) != std::string::npos; };
    assert(contains(s, "World"));
    assert(!contains(s, "WORLD"));
    std::cout << "[intro] __cpp_lib_string_contains unavailable; used find\n";
#endif

    std::cout << "std_string_contains_cpp23: all checks passed\n";
    return 0;
}

[[maybe_unused]] const auto& _ = ::learn::topic<"part3/section01/std_string_contains_cpp23", run>;

}  // namespace
